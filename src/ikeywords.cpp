// $Id$
// Author: John Wu <John.Wu@ACM.org>
// Copyright 2006-2008 the Regents of the University of California
//
// This file implements the ibis::keywords class.
#include "ikeywords.h"
#include "iroster.h"	// ibis::iroster
#include "part.h"	// ibis::table
#include <fstream>	// std::ifstream

#include <algorithm>	// std::sort
#include <string.h>	// strlen


/// Constructor.  It first tries to read the terms (@c .terms) and the
/// tdmat (@c .idx) files if they both exist.  If that fails, it tries to
/// read the tdlist (@c .tdlist) file generated by another program and 
ibis::keywords::keywords(const ibis::column* c,
			 const ibis::column* idcol, const char* f)
    : ibis::index(c) {
    if (c == 0) return; // does nothing
    if (c->type() != ibis::CATEGORY &&
	c->type() != ibis::TEXT) {
	LOGGER(0) << "ibis::keywords::keywords -- can only index categorical "
	    "values or string values";
	throw ibis::bad_alloc("wrong column type for ibis::keywords");
    }

    // try to read an existing term-document matrix
    std::string fdic, fmat;
    dataFileName(f, fdic);
    fmat = fdic;
    fdic += ".terms";
    fmat += ".idx";
    if (ibis::util::getFileSize(fdic.c_str()) > 0 &&
	ibis::util::getFileSize(fmat.c_str()) > 0) {
	read(f);
	if (ibis::gVerbose > 4) {
	    ibis::util::logger lg(4);
	    print(lg.buffer());
	}
	return;
    }

    // need to read a tdlist file generated externally, first check that id
    // column is a valid one
    if (idcol != 0 &&
	(idcol->type() == ibis::FLOAT ||
	 idcol->type() == ibis::DOUBLE)) {
	LOGGER(0) << "ibis::keywords::keywords -- the id column of "
	    "ibis::keywords can only be integers";
	throw ibis::bad_alloc("ibis::keywords can only use "
			      "integers as ids");
    }
    fmat.erase(fmat.size()-3);
    fmat += "tdlist";
    int ierr = readTermDocFile(idcol, fmat.c_str());
    if (ierr == -1 && f != 0 && *f != 0)
	ierr = readTermDocFile(idcol, f);
    if (ierr >= 0) { // write out the dictionary and the boolean matrix
	write(f);
	if (ibis::gVerbose > 4) {
	    ibis::util::logger lg(4);
	    print(lg.buffer());
	}
    }
    else {
	LOGGER(0) << "ibis::keywords::keywords -- readTermDocFile failed "
	    "with error code " << ierr;
	throw ibis::bad_alloc("ibis::keywords failed to read tdlist file");
    }
} // ibis::keywords::keywords

ibis::keywords::keywords(const ibis::column* c, ibis::fileManager::storage* st,
			 uint32_t offset) : ibis::index(c, st) {
    read(st);
} // ibis::keywords::keywords

int ibis::keywords::readTermDocFile(const ibis::column* idcol, const char* f) {
    int ierr = 0;

    std::ifstream tdf(f);
    if (! tdf) {
	LOGGER(3) << "ibis::keywords::readTermDocFile -- failed to open \""
		  << f << "\" for reading";
	return -1;
    }

    char* buf;
    uint32_t nbuf = ibis::util::getBuffer(buf);
    if (nbuf == 0 || buf == 0) {
	LOGGER(2) << "ibis::keywords::readTermDocFile(" << f
		  << ") -- failed to acquire a buffer to reading";
	delete [] buf;
	return -2;
    }
    nrows = col->partition()->nRows();
    std::string kw;
    std::vector<uint32_t> idlist;
    typedef std::map<char*, ibis::bitvector*, ibis::lessi> TBMap;
    TBMap tbmap;
    if (idcol != 0) {
	ibis::roster ros(idcol);
	while ((ierr = readLine(tdf, kw, idlist, buf, nbuf)) == 0) {
	    ibis::bitvector bvec;
	    ierr = ros.locate(idlist, bvec);
	    if (ierr < 0)
		setBits(idlist, bvec);
	    bvec.adjustSize(0, nrows);
	    if (bvec.cnt() > 0) {
		tbmap[ibis::util::strnewdup(kw.c_str())] =
		    new ibis::bitvector(bvec);
		if (ibis::gVerbose > 1 && tbmap.size() % 100000 == 0)
		    col->logMessage("keywords::readTermDocFile",
				    "reading keywords from %s, got %lu", f,
				    static_cast<long unsigned>(tbmap.size()));
	    }
	} // reading a line of the term-document list file
    }
    else {
	while ((ierr = readLine(tdf, kw, idlist, buf, nbuf)) == 0) {
	    ibis::bitvector bvec;
	    setBits(idlist, bvec);
	    bvec.adjustSize(0, nrows);
	    if (bvec.cnt() > 0) {
		tbmap[ibis::util::strnewdup(kw.c_str())] =
		    new ibis::bitvector(bvec);
		if (ibis::gVerbose > 1 && tbmap.size() % 100000 == 0)
		    col->logMessage("keywords::readTermDocFile",
				    "reading keywords from %s, got %lu", f,
				    static_cast<long unsigned>(tbmap.size()));
	    }
	} // reading a line of the term-document list file
    }
    tdf.close();
    delete buf;
    if (tbmap.empty())
	return ierr;
    if (ierr > 0) // ignore warnings
	ierr = 0;

    if (ibis::gVerbose > 1) {
	col->logMessage("keywords::readTermDocFile",
			"read %lu keyword%s from \"%s\" using %s as ID column",
			static_cast<long unsigned>(tbmap.size()),
			(tbmap.size()>1U ? "s" : ""), f,
			(idcol ? idcol->name() : "the row number"));
    }
    // translates the tbmap into a dictionary and a vector of pointers
    bits.resize(tbmap.size()+1);
    bits[0] = new ibis::bitvector;
    bits[0]->adjustSize(0, nrows);
    std::vector<char*> toDelete;
    for (TBMap::const_iterator it = tbmap.begin();
	 it != tbmap.end(); ++ it) {
	uint32_t sz0 = terms.size();
	uint32_t pos = terms.insertRaw((*it).first);
	*(bits[0]) |= *((*it).second);
	if (pos >= sz0) { // a new word in the dictionary
	    bits[pos] = (*it).second;
	}
	else {
	    *(bits[pos]) |= *((*it).second);
	    delete (*it).second;
	    toDelete.push_back((*it).first);
	}
    }
    for (unsigned i = 0; i < toDelete.size(); ++ i)
	delete [] toDelete[i];
    bits[0]->compress();
    bits[0]->flip();
    return ierr;
} // ibis::keywords::readTermDocFile

// read one line from the input stream, extract the keyword and the list of ids
int ibis::keywords::readLine(std::istream &in,
			     std::string &key,
			     std::vector<uint32_t> &idlist,
			     char* linebuf, uint32_t nbuf) const {
    int ierr = 0;
    char *str1, *str2;
    key.erase(); // empty the content of keyword
    idlist.clear(); // empty the content of the id list
    in.get(linebuf, nbuf);
    if (in.eof())
	return 3;
    if (linebuf[0] == 0)
	return 2;

    str1 = linebuf;
    char c = readKeyword(const_cast<const char*&>(str1), key);
    if (c != ':') { // failed to find the required delimiter after keyword
	LOGGER(4)
	    << "ibis::keywords::readLine -- failed to find the "
	    "required delimiter ':' after the keyword \"" << key
	    << "\".  Skip the line";
	ierr = -1;
	return ierr;
    }

    while (*str1) {
	str2 = str1;
	unsigned int id = readUInt(const_cast<const char*&>(str2));
	if (*str2) { // not null, assume to be a correct delimiter
	    idlist.push_back(id);
	    str1 = str2;
	    if (*str2 == '\n') // end of line, done
		return ierr;
	}
	else { // end of current buffer
	    char eol;
	    in.get(eol);
	    if (eol == '\n') {
		idlist.push_back(id);
#if defined(DEBUG)
		LOGGER(0)
		    << "ibis::keywords::readLine -- keyword:" << key
		    << ", count:" << idlist.size() << " ("
		    << idlist[0] << (idlist.size()>1 ? ", ...)" : ")");
#endif
		return ierr;
	    }
	    else {
		for (str2=linebuf; *str1 != 0; ++ str1, ++ str2)
		    *str2 = *str1;
		in.unget();
	    }
	    in.get(str2, nbuf-(str2-linebuf));
	    if (in.eof())
		return ierr;
	    if (linebuf[0] == 0)
		return ierr;
	    str1 = linebuf;
	}
    }
    ierr = 1; // terminating without seeing the end of a line
    return ierr;
} // ibis::keywords::readLine

// turn on the specified positions in a bitvector
void ibis::keywords::setBits(std::vector<uint32_t>& pos,
			     ibis::bitvector& bvec) const {
    bvec.clear(); // clear the current content
    std::sort(pos.begin(), pos.end());
    for (std::vector<uint32_t>::const_iterator it = pos.begin();
	 it != pos.end(); ++ it) {
	bvec.setBit(*it, 1);
    }
} // ibis::keywords::setBits

void ibis::keywords::binWeights(std::vector<uint32_t>& bw) const {
    bw.resize(bits.size());
    activate();
    for (uint32_t i = 0; i < bits.size(); ++ i)
	bw[i] = (bits[i] ? bits[i]->cnt() : 0);
} // ibis::keywords::binWeights

void ibis::keywords::print(std::ostream& out) const {
    if (ibis::gVerbose < 0) return;
    const uint32_t nobs = bits.size();
    if (terms.size()+1 == bits.size() && terms.size() > 0) {
	out << "The boolean term-document matrix for " << col->name()
	    << " contains the following terms (optionally followed "
	    "by term frequencies)\n";
	uint32_t skip = 0;
	if (ibis::gVerbose <= 0) {
	    skip = nobs;
	}
	else if ((nobs >> 2*ibis::gVerbose) > 2) {
	    skip = static_cast<uint32_t>
		(ibis::util::compactValue
		 (static_cast<double>(nobs >> (1+2*ibis::gVerbose)),
		  static_cast<double>(nobs >> (2*ibis::gVerbose))));
	    if (skip < 1)
		skip = 1;
	}
	else {
	    skip = 1;
	}
	if (skip > 1) {
	    out << " (printing 1 out of every " << skip << ")\n";
	}
	for (uint32_t i = 1; i < bits.size(); i += skip) {
	    out << terms[i] << "\t";
	    if (bits[i])
		out << bits[i]->cnt();
	    out << "\n";
	}
    }
    else if (col != 0) {
	out << "The boolean term-document matrix for " << col->name()
	    << " is empty";
    }
    out << std::endl;
} // ibis::keywords::print

/// Write the boolean term-document matrix as two files, xxx.terms
/// for the terms and xxx.idx for the bitmaps that marks the positions.
void ibis::keywords::write(const char* dt) const {
    std::string fnm;
    dataFileName(dt, fnm);
    fnm += ".terms";
    terms.write(fnm.c_str());

    fnm.erase(fnm.size()-5);
    fnm += "idx";
    if (fname != 0 || str != 0)
	activate();
    int fdes = UnixOpen(fnm.c_str(), OPEN_WRITEONLY, OPEN_FILEMODE);
    if (fdes < 0) {
	col->logWarning("keywords::write", "unable to open \"%s\" for write",
			fnm.c_str());
	return;
    }
#if defined(_WIN32) && defined(_MSC_VER)
    (void)_setmode(fdes, _O_BINARY);
#endif

    int32_t ierr = 0;
    const uint32_t nobs = bits.size();

    array_t<int32_t> offs(nobs+1);
    char header[] = "#IBIS\7\0\0";
    header[5] = (char)ibis::index::KEYWORDS;
    header[6] = (char)sizeof(int32_t);
    ierr = UnixWrite(fdes, header, 8);
    ierr = UnixWrite(fdes, &nrows, sizeof(uint32_t));
    ierr = UnixWrite(fdes, &nobs, sizeof(uint32_t));
    ierr = UnixSeek(fdes, sizeof(int32_t)*(nobs+1), SEEK_CUR);
    for (uint32_t i = 0; i < nobs; ++ i) {
	offs[i] = UnixSeek(fdes, 0, SEEK_CUR);
	if (bits[i]) {
	    bits[i]->write(fdes);
	    if (bits[i]->size() != nrows)
		col->logWarning("keywords::write", "bits[%lu] has %lu bits, "
				"expected %lu", static_cast<long unsigned>(i),
				static_cast<long unsigned>(bits[i]->size()),
				static_cast<long unsigned>(nrows));
	}
#if defined(DEBUG)
	int id0 = -1;
	if (bits[i]) {
	    ibis::bitvector::indexSet ixs = bits[i]->firstIndexSet();
	    id0 = *(ixs.indices());
	}
	col->logMessage("keywords::write", "keyword:%s, count:%lu (%d)",
			(terms[i] ? terms[i] : "<NULL>"),
			static_cast<long unsigned>(bits[i] ? bits[i]->cnt()
						   : 0),
			id0);
#endif
    }
    offs[nobs] = UnixSeek(fdes, 0, SEEK_CUR);
    ierr = UnixSeek(fdes, 8+sizeof(uint32_t)*2, SEEK_SET);
    ierr = UnixWrite(fdes, offs.begin(), sizeof(int32_t)*(nobs+1));
#if _POSIX_FSYNC+0 > 0
    (void) fsync(fdes); // write to disk
#endif
    ierr = UnixClose(fdes);

    if (ibis::gVerbose > 5)
	col->logMessage("keywords::write", "wrote %lu bitmap%s to %s",
			static_cast<long unsigned>(nobs),
			(nobs>1?"s":""), fnm.c_str());
} // ibis::keywords::write

void ibis::keywords::read(const char* f) {
    std::string fnm;
    dataFileName(f, fnm);
    fnm += ".terms";
    terms.read(fnm.c_str());

    fnm.erase(fnm.size()-5);
    fnm += "idx";
    int fdes = UnixOpen(fnm.c_str(), OPEN_READONLY);
    if (fdes < 0) return;

    char header[8];
#if defined(_WIN32) && defined(_MSC_VER)
    (void)_setmode(fdes, _O_BINARY);
#endif
    if (8 != UnixRead(fdes, static_cast<void*>(header), 8)) {
	UnixClose(fdes);
	return;
    }

    if (false == (header[0] == '#' && header[1] == 'I' &&
		  header[2] == 'B' && header[3] == 'I' &&
		  header[4] == 'S' &&
		  header[5] == static_cast<char>(ibis::index::KEYWORDS) &&
		  header[6] == static_cast<char>(sizeof(int32_t)) &&
		  header[7] == static_cast<char>(0))) {
	UnixClose(fdes);
	return;
    }

    uint32_t dim[2];
    uint32_t begin, end;
    ibis::index::clear(); // clear the current bit vectors
    fname = ibis::util::strnewdup(fnm.c_str());

    int ierr = UnixRead(fdes, static_cast<void*>(dim), 2*sizeof(uint32_t));
    if (ierr < static_cast<int>(2*sizeof(uint32_t))) {
	UnixClose(fdes);
	return;
    }
    nrows = dim[0];
    bool trymmap = false;
#if defined(HAS_FILE_MAP)
    trymmap = (dim[1] > ibis::fileManager::pageSize());
#endif
    // read offsets
    begin = 8 + 2*sizeof(uint32_t);
    end = 8 + 2*sizeof(uint32_t) + sizeof(int32_t) * (dim[1] + 1);
    if (trymmap) {
	array_t<int32_t> tmp(fnm.c_str(), begin, end);
	offsets.swap(tmp);
    }
    else {
	array_t<int32_t> tmp(fdes, begin, end);
	offsets.swap(tmp);
    }
    ibis::fileManager::instance().recordPages(0, end);
#if defined(DEBUG)
    if (ibis::gVerbose > 5) {
	unsigned nprt = (ibis::gVerbose < 30 ? (1 << ibis::gVerbose) : dim[1]);
	if (nprt > dim[1])
	    nprt = dim[1];
	ibis::util::logger lg(5);
	lg.buffer() << "DEBUG -- ibis::keywords::read(" << fnm
		    << ") got nobs = " << dim[1]
		    << ", the offsets of the bit vectors are\n";
	for (unsigned i = 0; i < nprt; ++ i)
	    lg.buffer() << offsets[i] << " ";
	if (nprt < dim[1])
	    lg.buffer() << "... (skipping " << dim[1]-nprt << ") ... ";
	lg.buffer() << offsets[dim[1]];
    }
#endif

    bits.resize(dim[1]);
    for (uint32_t i = 0; i < dim[1]; ++i)
	bits[i] = 0;
#if defined(ALWAY_READ_BITVECTOR0)
    // read the first bitvector
    if (offsets[1] > offsets[0]) {
	array_t<ibis::bitvector::word_t> a0(fdes, offsets[0], offsets[1]);
	bits[0] = new ibis::bitvector(a0);
#if defined(WAH_CHECK_SIZE)
	const uint32_t len = strlen(col->partition()->currentDataDir());
	if (0 == strncmp(f, col->partition()->currentDataDir(), len)
	    && bits[0]->size() != nrows) {
	    col->logWarning("readIndex", "the size (%lu) of the 1st "
			    "bitvector (from \"%s\") differs from "
			    "nRows (%lu)",
			    static_cast<long unsigned>(bits[0]->size()),
			    fnm.c_str(), static_cast<long unsigned>(nrows));
	}
#else
	bits[0]->setSize(nrows);
#endif
    }
    else {
	bits[0] = new ibis::bitvector;
	bits[0]->set(0, nrows);
    }
#endif
    UnixClose(fdes);
    str = 0;
    if (ibis::gVerbose > 7)
	col->logMessage("readIndex", "finished reading '%s' header from %s",
			name(), fnm.c_str());
} // ibis::keywords::read

// attempt to reconstruct an index from a piece of consecutive memory
void ibis::keywords::read(ibis::fileManager::storage* st) {
    if (st == 0) return;
    if (str != st && str != 0)
	delete str;
    if (fname) { // previously connected to a file, clean it up
	delete [] fname;
	offsets.clear();
	fname = 0;
    }

    nrows = *(reinterpret_cast<uint32_t*>(st->begin()+8));
    uint32_t pos = 8 + sizeof(uint32_t);
    const uint32_t nobs = *(reinterpret_cast<uint32_t*>(st->begin()+pos));
    pos += sizeof(uint32_t);
    array_t<int32_t> offs(st, pos, nobs+1);
    offsets.copy(offs);

    for (uint32_t i = 0; i < bits.size(); ++ i)
	delete bits[i];
    bits.resize(nobs);
    for (uint32_t i = 0; i < nobs; ++i)
	bits[i] = 0;
    if (st->isFileMap()) { // only restore the first bitvector
#if defined(ALWAY_READ_BITVECTOR0)
	if (offs[1] > offs[0]) {
	    array_t<ibis::bitvector::word_t>
		a0(st, offs[0], (offs[1]-offs[0])/
		   sizeof(ibis::bitvector::word_t));
	    bits[0] = new ibis::bitvector(a0);
#if defined(WAH_CHECK_SIZE)
	    if (bits[0]->size() != nrows) {
		col->logWarning("readIndex", "the length (%lu) of the 1st "
				"bitvector differs from nRows(%lu)",
				static_cast<long unsigned>(bits[0]->size()),
				static_cast<long unsigned>(nrows));
	    }
#else
	    bits[0]->setSize(nrows);
#endif
	}
	else {
	    bits[0] = new ibis::bitvector;
	    bits[0]->adjustSize(0, nrows);
	}
#endif
	str = st;
    }
    else { // regenerate all the bitvectors
	for (uint32_t i = 0; i < nobs; ++i) {
	    if (offs[i+1] > offs[i]) {
		array_t<ibis::bitvector::word_t>
		    a(st, offs[i], (offs[i+1]-offs[i])/
		      sizeof(ibis::bitvector::word_t));
		ibis::bitvector* btmp = new ibis::bitvector(a);
		bits[i] = btmp;
#if defined(WAH_CHECK_SIZE)
		if (btmp->size() != nrows) {
		    col->logWarning("readIndex", "the length (%lu) of the "
				    "%lu-th bitvector differs from "
				    "nRows(%lu)",
				    static_cast<long unsigned>(btmp->size()),
				    static_cast<long unsigned>(i),
				    static_cast<long unsigned>(nrows));
		}
#else
		btmp->setSize(nrows);
#endif
	    }
	}
	str = 0;
    }
    if (terms.size() < bits.size()) { // need to read the dictionary
	std::string fnm;
	dataFileName(0, fnm);
	fnm += ".terms";
	terms.read(fnm.c_str());
    }
} // ibis::keywords::read

void ibis::keywords::clear() {
    terms.clear();
    ibis::index::clear();
} // ibis::keywords::clear

long ibis::keywords::append(const char* dt, const char* df, uint32_t nnew) {
    throw "ibis::keywords::append not implemented yet";
} // ibis::keywords::append

long ibis::keywords::evaluate(const ibis::qContinuousRange& expr,
			      ibis::bitvector& lower) const {
    return -1L;
} // ibis::keywords::evaluate

void ibis::keywords::estimate(const ibis::qContinuousRange& expr,
			      ibis::bitvector& lower,
			      ibis::bitvector& upper) const {
    throw "ibis::keywords::estimate not implemented yet";
} // ibis::keywords::estimate

uint32_t ibis::keywords::estimate(const ibis::qContinuousRange& expr) const {
    throw "ibis::keywords::estimate not implemented yet";
    return 0;
} // ibis::keywords::estimate

long ibis::keywords::search(const char* kw,
			    ibis::bitvector& hits) const {
    hits.clear();
    uint32_t pos = terms[kw];
    if (pos < bits.size()) {
	if (bits[pos] == 0)
	    activate(pos);
	if (bits[pos] != 0)
	    hits.copy(*bits[pos]);
	else
	    hits.set(0, nrows);
    }
    else {
	hits.set(0, nrows);
    }
    return hits.cnt();
} // ibis::keywords::search

long ibis::keywords::search(const char* kw) const {
    long cnt = 0;
    uint32_t pos = terms[kw];
    if (pos < bits.size()) {
	if (bits[pos] == 0)
	    activate(pos);
	if (bits[pos] != 0)
	    cnt = bits[pos]->cnt();
    }
    return cnt;
} // ibis::keywords::search

double ibis::keywords::estimateCost(const ibis::qContinuousRange& expr) const {
    double ret = 0.0;
    if (offsets.size() > bits.size()) {
	if (expr.leftOperator() == ibis::qExpr::OP_EQ) {
	    uint32_t h0 = static_cast<uint32_t>(expr.leftBound());
	    if (h0 < bits.size())
		ret = offsets[h0+1] - offsets[h0];
	}
	else if (expr.leftOperator() == ibis::qExpr::OP_EQ) {
	    uint32_t h1 = static_cast<uint32_t>(expr.rightBound());
	    if (h1 < bits.size())
		ret = offsets[h1+1] - offsets[h1];
	}
    }
    return ret;
} // ibis::keywords::estimateCost

double ibis::keywords::estimateCost(const ibis::qDiscreteRange& expr) const {
    double ret = 0.0;
    if (offsets.size() > bits.size()) {
	const std::vector<double>& vals = expr.getValues();
	for (unsigned j = 0; j < vals.size(); ++ j) {
	    uint32_t itmp = static_cast<uint32_t>(vals[j]);
	    if (itmp < bits.size())
		ret += offsets[itmp+1] - offsets[itmp];
	}
    }
    return ret;
} // ibis::keywords::estimateCost
