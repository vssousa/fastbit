// File: $id$
// Author: John Wu <John.Wu at ACM.org> Lawrence Berkeley National Laboratory
// Copyright 2007-2011 the Regents of the University of California
//
#if defined(_WIN32) && defined(_MSC_VER)
#pragma warning(disable:4786)	// some identifier longer than 256 characters
#endif

#include "tab.h"	// ibis::tabula and ibis::tabele
#include "bord.h"	// ibis::bord
#include "query.h"	// ibis::query
#include "countQuery.h"	// ibis::countQuery
#include "bundle.h"	// ibis::bundle

#include <limits>	// std::numeric_limits
#include <sstream>	// std::ostringstream
#include <typeinfo>	// std::typeid
#include <memory>	// std::auto_ptr
#include <algorithm>	// std::reverse, std::copy

/// Constructor.  The responsibility of freeing the memory pointed by the
/// elements of buf is transferred to this object.
ibis::bord::bord(const char *tn, const char *td, uint64_t nr,
		 ibis::table::bufferList       &buf,
		 const ibis::table::typeList   &ct,
		 const ibis::table::stringList &cn,
		 const ibis::table::stringList *cdesc)
    : ibis::table(tn, td), mypart(tn, td, nr, cn, ct, buf, cdesc) {
} // ibis::bord::bord

void ibis::bord::clear() {
} // ibis::bord::clear

/// @note The pointers returned are pointing to names stored internally.
/// The caller should not attempt to free these pointers.
ibis::table::stringList ibis::bord::columnNames() const {
    return mypart.columnNames();
} // ibis::bord::columnNames

ibis::table::typeList ibis::bord::columnTypes() const {
    return mypart.columnTypes();
} // ibis::bord::columnTypes

int64_t ibis::bord::getColumnAsBytes(const char *cn, char *vals) const {
    const ibis::bord::column *col =
	dynamic_cast<const ibis::bord::column*>(mypart.getColumn(cn));
    if (col == 0)
	return -1;

    if (col->type() == ibis::BYTE || col->type() == ibis::UBYTE) {
	const array_t<signed char>* arr =
	    static_cast<const array_t<signed char>*>(col->getArray());
	if (arr == 0) return -3;
	const uint32_t sz = (mypart.nRows() <= arr->size() ? 
			   mypart.nRows() : arr->size());
	std::copy(arr->begin(), arr->begin()+sz, vals);
	return sz;
    }
    else {
	return -2;
    }
} // ibis::bord::getColumnAsBytes

int64_t
ibis::bord::getColumnAsUBytes(const char *cn, unsigned char *vals) const {
    const ibis::bord::column *col =
	dynamic_cast<const ibis::bord::column*>(mypart.getColumn(cn));
    if (col == 0)
	return -1;

    if (col->type() == ibis::BYTE || col->type() == ibis::UBYTE) {
	const array_t<unsigned char>* arr =
	    static_cast<const array_t<unsigned char>*>(col->getArray());
	if (arr == 0) return -3;
	const uint32_t sz = (mypart.nRows() <= arr->size() ? 
			   mypart.nRows() : arr->size());
	std::copy(arr->begin(), arr->begin()+sz, vals);
	return sz;
    }
    else {
	return -2;
    }
} // ibis::bord::getColumnAsUBytes

int64_t ibis::bord::getColumnAsShorts(const char *cn, int16_t *vals) const {
    const ibis::bord::column *col =
	dynamic_cast<const ibis::bord::column*>(mypart.getColumn(cn));
    if (col == 0)
	return -1;

    if (col->type() == ibis::SHORT || col->type() == ibis::USHORT) {
	const array_t<int16_t>* arr =
	    static_cast<const array_t<int16_t>*>(col->getArray());
	if (arr == 0) return -3;
	const uint32_t sz = (mypart.nRows() <= arr->size() ? 
			   mypart.nRows() : arr->size());
	std::copy(arr->begin(), arr->begin()+sz, vals);
	return sz;
    }
    else if (col->type() == ibis::BYTE) {
	const array_t<signed char>* arr =
	    static_cast<const array_t<signed char>*>(col->getArray());
	if (arr == 0) return -3;
	const uint32_t sz = (mypart.nRows() <= arr->size() ? 
			   mypart.nRows() : arr->size());
	std::copy(arr->begin(), arr->begin()+sz, vals);
	return sz;
    }
    else if (col->type() == ibis::UBYTE) {
	const array_t<unsigned char>* arr =
	    static_cast<const array_t<unsigned char>*>(col->getArray());
	if (arr == 0) return -3;
	const uint32_t sz = (mypart.nRows() <= arr->size() ? 
			   mypart.nRows() : arr->size());
	std::copy(arr->begin(), arr->begin()+sz, vals);
	return sz;
    }
    else {
	return -2;
    }
} // ibis::bord::getColumnAsShorts

int64_t ibis::bord::getColumnAsUShorts(const char *cn, uint16_t *vals) const {
    const ibis::bord::column *col =
	dynamic_cast<const ibis::bord::column*>(mypart.getColumn(cn));
    if (col == 0)
	return -1;

    if (col->type() == ibis::SHORT || col->type() == ibis::USHORT) {
	const array_t<uint16_t>* arr =
	    static_cast<const array_t<uint16_t>*>(col->getArray());
	if (arr == 0) return -3;
	const uint32_t sz = (mypart.nRows() <= arr->size() ? 
			   mypart.nRows() : arr->size());
	std::copy(arr->begin(), arr->begin()+sz, vals);
	return sz;
    }
    else if (col->type() == ibis::BYTE || col->type() == ibis::UBYTE) {
	const array_t<unsigned char>* arr =
	    static_cast<const array_t<unsigned char>*>(col->getArray());
	if (arr == 0) return -3;
	const uint32_t sz = (mypart.nRows() <= arr->size() ? 
			   mypart.nRows() : arr->size());
	std::copy(arr->begin(), arr->begin()+sz, vals);
	return sz;
    }
    else {
	return -2;
    }
} // ibis::bord::getColumnAsUShorts

int64_t ibis::bord::getColumnAsInts(const char *cn, int32_t *vals) const {
    const ibis::bord::column *col =
	dynamic_cast<const ibis::bord::column*>(mypart.getColumn(cn));
    if (col == 0)
	return -1;

    if (col->type() == ibis::INT || col->type() == ibis::UINT) {
	const array_t<int32_t>* arr =
	    static_cast<const array_t<int32_t>*>(col->getArray());
	if (arr == 0) return -3;
	const uint32_t sz = (mypart.nRows() <= arr->size() ? 
			     mypart.nRows() : arr->size());
	std::copy(arr->begin(), arr->begin()+sz, vals);
	return sz;
    }
    else if (col->type() == ibis::SHORT) {
	const array_t<int16_t>* arr =
	    static_cast<const array_t<int16_t>*>(col->getArray());
	if (arr == 0) return -3;
	const uint32_t sz = (mypart.nRows() <= arr->size() ? 
			     mypart.nRows() : arr->size());
	std::copy(arr->begin(), arr->begin()+sz, vals);
	return sz;
    }
    else if (col->type() == ibis::USHORT) {
	const array_t<uint16_t>* arr =
	    static_cast<const array_t<uint16_t>*>(col->getArray());
	if (arr == 0) return -3;
	const uint32_t sz = (mypart.nRows() <= arr->size() ? 
			     mypart.nRows() : arr->size());
	std::copy(arr->begin(), arr->begin()+sz, vals);
	return sz;
    }
    else if (col->type() == ibis::BYTE) {
	const array_t<signed char>* arr =
	    static_cast<const array_t<signed char>*>(col->getArray());
	if (arr == 0) return -3;
	const uint32_t sz = (mypart.nRows() <= arr->size() ? 
			     mypart.nRows() : arr->size());
	std::copy(arr->begin(), arr->begin()+sz, vals);
	return sz;
    }
    else if (col->type() == ibis::UBYTE) {
	const array_t<unsigned char>* arr =
	    static_cast<const array_t<unsigned char>*>(col->getArray());
	if (arr == 0) return -3;
	const uint32_t sz = (mypart.nRows() <= arr->size() ? 
			     mypart.nRows() : arr->size());
	std::copy(arr->begin(), arr->begin()+sz, vals);
	return sz;
    }
    else {
	return -2;
    }
} // ibis::bord::getColumnAsInts

int64_t ibis::bord::getColumnAsUInts(const char *cn, uint32_t *vals) const {
    const ibis::bord::column *col =
	dynamic_cast<const ibis::bord::column*>(mypart.getColumn(cn));
    if (col == 0)
	return -1;

    if (col->type() == ibis::INT || col->type() == ibis::UINT) {
	const array_t<uint32_t>* arr =
	    static_cast<const array_t<uint32_t>*>(col->getArray());
	if (arr == 0) return -3;
	const uint32_t sz = (mypart.nRows() <= arr->size() ? 
			   mypart.nRows() : arr->size());
	std::copy(arr->begin(), arr->begin()+sz, vals);
	return sz;
    }
    else if (col->type() == ibis::SHORT || col->type() == ibis::USHORT) {
	const array_t<uint16_t>* arr =
	    static_cast<const array_t<uint16_t>*>(col->getArray());
	if (arr == 0) return -3;
	const uint32_t sz = (mypart.nRows() <= arr->size() ? 
			   mypart.nRows() : arr->size());
	std::copy(arr->begin(), arr->begin()+sz, vals);
	return sz;
    }
    else if (col->type() == ibis::BYTE || col->type() == ibis::UBYTE) {
	const array_t<unsigned char>* arr =
	    static_cast<const array_t<unsigned char>*>(col->getArray());
	if (arr == 0) return -3;
	const uint32_t sz = (mypart.nRows() <= arr->size() ? 
			   mypart.nRows() : arr->size());
	std::copy(arr->begin(), arr->begin()+sz, vals);
	return sz;
    }
    else {
	return -2;
    }
} // ibis::bord::getColumnAsUInts

int64_t ibis::bord::getColumnAsLongs(const char *cn, int64_t *vals) const {
    const ibis::bord::column *col =
	dynamic_cast<const ibis::bord::column*>(mypart.getColumn(cn));
    if (col == 0)
	return -1;

    if (col->type() == ibis::LONG || col->type() == ibis::ULONG) {
	const array_t<int64_t>* arr =
	    static_cast<const array_t<int64_t>*>(col->getArray());
	if (arr == 0) return -3;
	const uint32_t sz = (mypart.nRows() <= arr->size() ? 
			   mypart.nRows() : arr->size());
	std::copy(arr->begin(), arr->begin()+sz, vals);
	return sz;
    }
    else if (col->type() == ibis::INT) {
	const array_t<int32_t>* arr =
	    static_cast<const array_t<int32_t>*>(col->getArray());
	if (arr == 0) return -3;
	const uint32_t sz = (mypart.nRows() <= arr->size() ? 
			   mypart.nRows() : arr->size());
	std::copy(arr->begin(), arr->begin()+sz, vals);
	return sz;
    }
    else if (col->type() == ibis::UINT) {
	const array_t<uint32_t>* arr =
	    static_cast<const array_t<uint32_t>*>(col->getArray());
	if (arr == 0) return -3;
	const uint32_t sz = (mypart.nRows() <= arr->size() ? 
			   mypart.nRows() : arr->size());
	std::copy(arr->begin(), arr->begin()+sz, vals);
	return sz;
    }
    else if (col->type() == ibis::SHORT) {
	const array_t<int16_t>* arr =
	    static_cast<const array_t<int16_t>*>(col->getArray());
	if (arr == 0) return -3;
	const uint32_t sz = (mypart.nRows() <= arr->size() ? 
			   mypart.nRows() : arr->size());
	std::copy(arr->begin(), arr->begin()+sz, vals);
	return sz;
    }
    else if (col->type() == ibis::USHORT) {
	const array_t<uint16_t>* arr =
	    static_cast<const array_t<uint16_t>*>(col->getArray());
	if (arr == 0) return -3;
	const uint32_t sz = (mypart.nRows() <= arr->size() ? 
			   mypart.nRows() : arr->size());
	std::copy(arr->begin(), arr->begin()+sz, vals);
	return sz;
    }
    else if (col->type() == ibis::BYTE) {
	const array_t<signed char>* arr =
	    static_cast<const array_t<signed char>*>(col->getArray());
	if (arr == 0) return -3;
	const uint32_t sz = (mypart.nRows() <= arr->size() ? 
			   mypart.nRows() : arr->size());
	std::copy(arr->begin(), arr->begin()+sz, vals);
	return sz;
    }
    else if (col->type() == ibis::UBYTE) {
	const array_t<unsigned char>* arr =
	    static_cast<const array_t<unsigned char>*>(col->getArray());
	if (arr == 0) return -3;
	const uint32_t sz = (mypart.nRows() <= arr->size() ? 
			   mypart.nRows() : arr->size());
	std::copy(arr->begin(), arr->begin()+sz, vals);
	return sz;
    }
    else {
	return -2;
    }
} // ibis::bord::getColumnAsLongs

int64_t ibis::bord::getColumnAsULongs(const char *cn, uint64_t *vals) const {
    const ibis::bord::column *col =
	dynamic_cast<const ibis::bord::column*>(mypart.getColumn(cn));
    if (col == 0)
	return -1;

    if (col->type() == ibis::LONG || col->type() == ibis::ULONG) {
	const array_t<uint64_t>* arr =
	    static_cast<const array_t<uint64_t>*>(col->getArray());
	if (arr == 0) return -3;
	const uint32_t sz = (mypart.nRows() <= arr->size() ? 
			   mypart.nRows() : arr->size());
	std::copy(arr->begin(), arr->begin()+sz, vals);
	return sz;
    }
    else if (col->type() == ibis::INT || col->type() == ibis::UINT) {
	const array_t<uint32_t>* arr =
	    static_cast<const array_t<uint32_t>*>(col->getArray());
	if (arr == 0) return -3;
	const uint32_t sz = (mypart.nRows() <= arr->size() ? 
			   mypart.nRows() : arr->size());
	std::copy(arr->begin(), arr->begin()+sz, vals);
	return sz;
    }
    else if (col->type() == ibis::SHORT || col->type() == ibis::USHORT) {
	const array_t<uint16_t>* arr =
	    static_cast<const array_t<uint16_t>*>(col->getArray());
	if (arr == 0) return -3;
	const uint32_t sz = (mypart.nRows() <= arr->size() ? 
			   mypart.nRows() : arr->size());
	std::copy(arr->begin(), arr->begin()+sz, vals);
	return sz;
    }
    else if (col->type() == ibis::BYTE || col->type() == ibis::UBYTE) {
	const array_t<unsigned char>* arr =
	    static_cast<const array_t<unsigned char>*>(col->getArray());
	if (arr == 0) return -3;
	const uint32_t sz = (mypart.nRows() <= arr->size() ? 
			   mypart.nRows() : arr->size());
	std::copy(arr->begin(), arr->begin()+sz, vals);
	return sz;
    }
    else {
	return -2;
    }
} // ibis::bord::getColumnAsULongs

int64_t ibis::bord::getColumnAsFloats(const char *cn, float *vals) const {
    const ibis::bord::column *col =
	dynamic_cast<const ibis::bord::column*>(mypart.getColumn(cn));
    if (col == 0)
	return -1;

    switch (col->type()) {
    case ibis::FLOAT: {
	const array_t<float>* arr =
	    static_cast<const array_t<float>*>(col->getArray());
	if (arr == 0) return -3;
	const uint32_t sz = (mypart.nRows() <= arr->size() ? 
			   mypart.nRows() : arr->size());
	std::copy(arr->begin(), arr->begin()+sz, vals);
	return sz;}
    case ibis::SHORT: {
	const array_t<int16_t>* arr =
	    static_cast<const array_t<int16_t>*>(col->getArray());
	if (arr == 0) return -3;
	const uint32_t sz = (mypart.nRows() <= arr->size() ? 
			   mypart.nRows() : arr->size());
	std::copy(arr->begin(), arr->begin()+sz, vals);
	return sz;}
    case ibis::USHORT: {
	const array_t<uint16_t>* arr =
	    static_cast<const array_t<uint16_t>*>(col->getArray());
	if (arr == 0) return -3;
	const uint32_t sz = (mypart.nRows() <= arr->size() ? 
			   mypart.nRows() : arr->size());
	std::copy(arr->begin(), arr->begin()+sz, vals);
	return sz;}
    case ibis::BYTE: {
	const array_t<signed char>* arr =
	    static_cast<const array_t<signed char>*>(col->getArray());
	if (arr == 0) return -3;
	const uint32_t sz = (mypart.nRows() <= arr->size() ? 
			   mypart.nRows() : arr->size());
	std::copy(arr->begin(), arr->begin()+sz, vals);
	return sz;}
    case ibis::UBYTE: {
	const array_t<unsigned char>* arr =
	    static_cast<const array_t<unsigned char>*>(col->getArray());
	if (arr == 0) return -3;
	const uint32_t sz = (mypart.nRows() <= arr->size() ? 
			   mypart.nRows() : arr->size());
	std::copy(arr->begin(), arr->begin()+sz, vals);
	return sz;}
    default:
	break;
    }
    return -2;
} // ibis::bord::getColumnAsFloats

int64_t ibis::bord::getColumnAsDoubles(const char *cn, double *vals) const {
    const ibis::bord::column *col =
	dynamic_cast<const ibis::bord::column*>(mypart.getColumn(cn));
    if (col == 0)
	return -1;

    switch (col->type()) {
    case ibis::DOUBLE: {
	const array_t<double>* arr =
	    static_cast<const array_t<double>*>(col->getArray());
	if (arr == 0) return -3;
	const uint32_t sz = (mypart.nRows() <= arr->size() ? 
			   mypart.nRows() : arr->size());
	std::copy(arr->begin(), arr->begin()+sz, vals);
	return sz;}
    case ibis::FLOAT: {
	const array_t<float>* arr =
	    static_cast<const array_t<float>*>(col->getArray());
	if (arr == 0) return -3;
	const uint32_t sz = (mypart.nRows() <= arr->size() ? 
			   mypart.nRows() : arr->size());
	std::copy(arr->begin(), arr->begin()+sz, vals);
	return sz;}
    case ibis::INT: {
	const array_t<int32_t>* arr =
	    static_cast<const array_t<int32_t>*>(col->getArray());
	if (arr == 0) return -3;
	const uint32_t sz = (mypart.nRows() <= arr->size() ? 
			   mypart.nRows() : arr->size());
	std::copy(arr->begin(), arr->begin()+sz, vals);
	return sz;}
    case ibis::UINT: {
	const array_t<uint32_t>* arr =
	    static_cast<const array_t<uint32_t>*>(col->getArray());
	if (arr == 0) return -3;
	const uint32_t sz = (mypart.nRows() <= arr->size() ? 
			   mypart.nRows() : arr->size());
	std::copy(arr->begin(), arr->begin()+sz, vals);
	return sz;}
    case ibis::SHORT: {
	const array_t<int16_t>* arr =
	    static_cast<const array_t<int16_t>*>(col->getArray());
	if (arr == 0) return -3;
	const uint32_t sz = (mypart.nRows() <= arr->size() ? 
			   mypart.nRows() : arr->size());
	std::copy(arr->begin(), arr->begin()+sz, vals);
	return sz;}
    case ibis::USHORT: {
	const array_t<uint16_t>* arr =
	    static_cast<const array_t<uint16_t>*>(col->getArray());
	if (arr == 0) return -3;
	const uint32_t sz = (mypart.nRows() <= arr->size() ? 
			   mypart.nRows() : arr->size());
	std::copy(arr->begin(), arr->begin()+sz, vals);
	return sz;}
    case ibis::BYTE: {
	const array_t<signed char>* arr =
	    static_cast<const array_t<signed char>*>(col->getArray());
	if (arr == 0) return -3;
	const uint32_t sz = (mypart.nRows() <= arr->size() ? 
			   mypart.nRows() : arr->size());
	std::copy(arr->begin(), arr->begin()+sz, vals);
	return sz;}
    case ibis::UBYTE: {
	const array_t<unsigned char>* arr =
	    static_cast<const array_t<unsigned char>*>(col->getArray());
	if (arr == 0) return -3;
	const uint32_t sz = (mypart.nRows() <= arr->size() ? 
			   mypart.nRows() : arr->size());
	std::copy(arr->begin(), arr->begin()+sz, vals);
	return sz;}
    default:
	break;
    }
    return -2;
} // ibis::bord::getColumnAsDoubles

int64_t ibis::bord::getColumnAsDoubles(const char* cn,
				       std::vector<double>& vals) const {
    const ibis::bord::column *col =
	dynamic_cast<const ibis::bord::column*>(mypart.getColumn(cn));
    if (col == 0)
	return -1;

    switch (col->type()) {
    case ibis::DOUBLE: {
	const array_t<double>* arr =
	    static_cast<const array_t<double>*>(col->getArray());
	if (arr == 0) return -3;
	const uint32_t sz = (mypart.nRows() <= arr->size() ? 
			   mypart.nRows() : arr->size());
	vals.resize(sz);
	std::copy(arr->begin(), arr->begin()+sz, vals.begin());
	return sz;}
    case ibis::FLOAT: {
	const array_t<float>* arr =
	    static_cast<const array_t<float>*>(col->getArray());
	if (arr == 0) return -3;
	const uint32_t sz = (mypart.nRows() <= arr->size() ? 
			   mypart.nRows() : arr->size());
	vals.resize(sz);
	std::copy(arr->begin(), arr->begin()+sz, vals.begin());
	return sz;}
    case ibis::INT: {
	const array_t<int32_t>* arr =
	    static_cast<const array_t<int32_t>*>(col->getArray());
	if (arr == 0) return -3;
	const uint32_t sz = (mypart.nRows() <= arr->size() ? 
			   mypart.nRows() : arr->size());
	vals.resize(sz);
	std::copy(arr->begin(), arr->begin()+sz, vals.begin());
	return sz;}
    case ibis::UINT: {
	const array_t<uint32_t>* arr =
	    static_cast<const array_t<uint32_t>*>(col->getArray());
	if (arr == 0) return -3;
	const uint32_t sz = (mypart.nRows() <= arr->size() ? 
			   mypart.nRows() : arr->size());
	vals.resize(sz);
	std::copy(arr->begin(), arr->begin()+sz, vals.begin());
	return sz;}
    case ibis::SHORT: {
	const array_t<int16_t>* arr =
	    static_cast<const array_t<int16_t>*>(col->getArray());
	if (arr == 0) return -3;
	const uint32_t sz = (mypart.nRows() <= arr->size() ? 
			   mypart.nRows() : arr->size());
	vals.resize(sz);
	std::copy(arr->begin(), arr->begin()+sz, vals.begin());
	return sz;}
    case ibis::USHORT: {
	const array_t<uint16_t>* arr =
	    static_cast<const array_t<uint16_t>*>(col->getArray());
	if (arr == 0) return -3;
	const uint32_t sz = (mypart.nRows() <= arr->size() ? 
			   mypart.nRows() : arr->size());
	vals.resize(sz);
	std::copy(arr->begin(), arr->begin()+sz, vals.begin());
	return sz;}
    case ibis::BYTE: {
	const array_t<signed char>* arr =
	    static_cast<const array_t<signed char>*>(col->getArray());
	if (arr == 0) return -3;
	const uint32_t sz = (mypart.nRows() <= arr->size() ? 
			   mypart.nRows() : arr->size());
	vals.resize(sz);
	std::copy(arr->begin(), arr->begin()+sz, vals.begin());
	return sz;}
    case ibis::UBYTE: {
	const array_t<unsigned char>* arr =
	    static_cast<const array_t<unsigned char>*>(col->getArray());
	if (arr == 0) return -3;
	const uint32_t sz = (mypart.nRows() <= arr->size() ? 
			   mypart.nRows() : arr->size());
	vals.resize(sz);
	std::copy(arr->begin(), arr->begin()+sz, vals.begin());
	return sz;}
    default:
	break;
    }
    return -2;
} // ibis::bord::getColumnAsDoubles

int64_t ibis::bord::getColumnAsStrings(const char *cn,
				       std::vector<std::string> &vals) const {
    const ibis::bord::column *col =
	dynamic_cast<ibis::bord::column*>(mypart.getColumn(cn));
    if (col == 0)
	return -1;

    switch (col->type()) {
    case ibis::TEXT:
    case ibis::CATEGORY: {
	const std::vector<std::string>* arr =
	    static_cast<const std::vector<std::string>*>(col->getArray());
	if (arr == 0) return -3;

	const uint32_t sz = arr->size();
	vals.resize(sz);
	std::copy(arr->begin(), arr->end(), vals.begin());
	return sz;}
    case ibis::DOUBLE: {
	const array_t<double> *arr =
	    static_cast<const array_t<double>*>(col->getArray());
	if (arr == 0) return -3;

	const uint32_t sz = arr->size();
	vals.resize(sz);
	for (uint32_t i = 0; i < sz; ++ i) {
	    std::ostringstream oss;
	    oss << (*arr)[i];
	    vals[i] = oss.str();
	}
	return sz;}
    case ibis::FLOAT: {
	const array_t<float> *arr =
	    static_cast<const array_t<float>*>(col->getArray());
	if (arr == 0) return -3;

	const uint32_t sz = arr->size();
	vals.resize(sz);
	for (uint32_t i = 0; i < sz; ++ i) {
	    std::ostringstream oss;
	    oss << (*arr)[i];
	    vals[i] = oss.str();
	}
	return sz;}
    case ibis::LONG: {
	const array_t<int64_t> *arr =
	    static_cast<const array_t<int64_t>*>(col->getArray());
	if (arr == 0) return -3;

	const uint32_t sz = arr->size();
	vals.resize(sz);
	for (uint32_t i = 0; i < sz; ++ i) {
	    std::ostringstream oss;
	    oss << (*arr)[i];
	    vals[i] = oss.str();
	}
	return sz;}
    case ibis::ULONG: {
	const array_t<uint64_t> *arr =
	    static_cast<const array_t<uint64_t>*>(col->getArray());
	if (arr == 0) return -3;

	const uint32_t sz = arr->size();
	vals.resize(sz);
	for (uint32_t i = 0; i < sz; ++ i) {
	    std::ostringstream oss;
	    oss << (*arr)[i];
	    vals[i] = oss.str();
	}
	return sz;}
    case ibis::INT: {
	const array_t<int32_t> *arr =
	    static_cast<const array_t<int32_t>*>(col->getArray());
	if (arr == 0) return -3;

	const uint32_t sz = arr->size();
	vals.resize(sz);
	for (uint32_t i = 0; i < sz; ++ i) {
	    std::ostringstream oss;
	    oss << (*arr)[i];
	    vals[i] = oss.str();
	}
	return sz;}
    case ibis::UINT: {
	const array_t<uint32_t> *arr =
	    static_cast<const array_t<uint32_t>*>(col->getArray());
	if (arr == 0) return -3;

	const uint32_t sz = arr->size();
	vals.resize(sz);
	for (uint32_t i = 0; i < sz; ++ i) {
	    std::ostringstream oss;
	    oss << (*arr)[i];
	    vals[i] = oss.str();
	}
	return sz;}
    case ibis::SHORT: {
	const array_t<int16_t> *arr =
	    static_cast<const array_t<int16_t>*>(col->getArray());
	if (arr == 0) return -3;

	const uint32_t sz = arr->size();
	vals.resize(sz);
	for (uint32_t i = 0; i < sz; ++ i) {
	    std::ostringstream oss;
	    oss << (*arr)[i];
	    vals[i] = oss.str();
	}
	return sz;}
    case ibis::USHORT: {
	const array_t<uint16_t> *arr =
	    static_cast<const array_t<uint16_t>*>(col->getArray());
	if (arr == 0) return -3;

	const uint32_t sz = arr->size();
	vals.resize(sz);
	for (uint32_t i = 0; i < sz; ++ i) {
	    std::ostringstream oss;
	    oss << (*arr)[i];
	    vals[i] = oss.str();
	}
	return sz;}
    case ibis::BYTE: {
	const array_t<signed char> *arr =
	    static_cast<const array_t<signed char>*>(col->getArray());
	if (arr == 0) return -3;

	const uint32_t sz = arr->size();
	vals.resize(sz);
	for (uint32_t i = 0; i < sz; ++ i) {
	    std::ostringstream oss;
	    oss << static_cast<int>((*arr)[i]);
	    vals[i] = oss.str();
	}
	return sz;}
    case ibis::UBYTE: {
	const array_t<unsigned char> *arr =
	    static_cast<const array_t<unsigned char>*>(col->getArray());
	if (arr == 0) return -3;

	const uint32_t sz = arr->size();
	vals.resize(sz);
	for (uint32_t i = 0; i < sz; ++ i) {
	    std::ostringstream oss;
	    oss << static_cast<int>((*arr)[i]);
	    vals[i] = oss.str();
	}
	return sz;}
    default:
	break;
    }
    return -2;
} // ibis::bord::getColumnAsDoubles

long ibis::bord::getHistogram(const char *constraints,
			     const char *cname,
			     double begin, double end, double stride,
			     std::vector<uint32_t>& counts) const {
    if (sizeof(uint32_t) == sizeof(uint32_t)) {
	return mypart.get1DDistribution
	    (constraints, cname, begin, end, stride,
	     reinterpret_cast<std::vector<uint32_t>&>(counts));
    }
    else {
	std::vector<uint32_t> tmp;
	long ierr = mypart.get1DDistribution(constraints, cname, begin, end,
					     stride, tmp);
	if (ierr >= 0) {
	    counts.resize(tmp.size());
	    std::copy(tmp.begin(), tmp.end(), counts.begin());
	}
	return ierr;
    }
} // ibis::bord::getHistogram

long ibis::bord::getHistogram2D(const char *constraints,
				const char *cname1,
				double begin1, double end1, double stride1,
				const char* cname2,
				double begin2, double end2, double stride2,
				std::vector<uint32_t>& counts) const {
    if (sizeof(uint32_t) == sizeof(uint32_t)) {
	return mypart.get2DDistribution
	    (constraints, cname1, begin1, end1, stride1,
	     cname2, begin2, end2, stride2,
	     reinterpret_cast<std::vector<uint32_t>&>(counts));
    }
    else {
	std::vector<uint32_t> tmp;
	long ierr = mypart.get2DDistribution
	    (constraints, cname1, begin1, end1, stride1,
	     cname2, begin2, end2, stride2, tmp);
	if (ierr >= 0) {
	    counts.resize(tmp.size());
	    std::copy(tmp.begin(), tmp.end(), counts.begin());
	}
	return ierr;
    }
} // ibis::bord::getHistogram2D

long ibis::bord::getHistogram3D(const char *constraints,
				const char *cname1,
				double begin1, double end1, double stride1,
				const char *cname2,
				double begin2, double end2, double stride2,
				const char *cname3,
				double begin3, double end3, double stride3,
				std::vector<uint32_t>& counts) const {
    if (sizeof(uint32_t) == sizeof(uint32_t)) {
	return mypart.get3DDistribution
	    (constraints, cname1, begin1, end1, stride1,
	     cname2, begin2, end2, stride2,
	     cname3, begin3, end3, stride3,
	     reinterpret_cast<std::vector<uint32_t>&>(counts));
    }
    else {
	std::vector<uint32_t> tmp;
	long ierr = mypart.get3DDistribution
	    (constraints, cname1, begin1, end1, stride1,
	     cname2, begin2, end2, stride2,
	     cname3, begin3, end3, stride3, tmp);
	if (ierr >= 0) {
	    counts.resize(tmp.size());
	    std::copy(tmp.begin(), tmp.end(), counts.begin());
	}
	return ierr;
    }
} // ibis::bord::getHistogram3D

void ibis::bord::estimate(const char *cond,
			  uint64_t &nmin, uint64_t &nmax) const {
    nmin = 0;
    nmax = mypart.nRows();
    ibis::countQuery q;
    int ierr = q.setWhereClause(cond);
    if (ierr < 0) return;

    ierr = q.setPartition(&mypart);
    if (ierr < 0) return;

    ierr = q.estimate();
    if (ierr >= 0) {
	nmin = q.getMinNumHits();
	nmax = q.getMaxNumHits();
    }
} // ibis::bord::estimate

void ibis::bord::estimate(const ibis::qExpr *cond,
			  uint64_t &nmin, uint64_t &nmax) const {
    nmin = 0;
    nmax = mypart.nRows();
    ibis::countQuery q;
    int ierr = q.setWhereClause(cond);
    if (ierr < 0) return;

    ierr = q.setPartition(&mypart);
    if (ierr < 0) return;

    ierr = q.estimate();
    if (ierr >= 0) {
	nmin = q.getMinNumHits();
	nmax = q.getMaxNumHits();
    }
} // ibis::bord::estimate

ibis::table* ibis::bord::select(const char *sel, const char *cond) const {
    if (cond == 0 || *cond == 0) return 0;
    if (sel != 0) // skip leading space
	while (isspace(*sel)) ++ sel;

    std::vector<const ibis::part*> prts(1);
    prts[0] = &mypart;
    return ibis::table::select(prts, sel, cond);
} // ibis::bord::select

int64_t ibis::bord::computeHits(const char *cond) const {
    int64_t res = -1;
    ibis::query q(ibis::util::userName(), &mypart);
    q.setWhereClause(cond);
    res = q.evaluate();
    if (res >= 0)
	res = q.getNumHits();
    return res;
} // ibis::bord::computeHits

int ibis::bord::getPartitions(std::vector<const ibis::part*> &lst) const {
    lst.resize(1);
    lst[0] = &mypart;
    return 1;
} // ibis::bord::getPartitions

ibis::table* 
ibis::bord::groupby(const char* keys) const {
    ibis::selectClause sel(keys);
    return mypart.groupby(sel);
} // ibis::bord::groupby

ibis::table* 
ibis::bord::groupby(const ibis::table::stringList& keys) const {
    ibis::selectClause sel(keys);
    return mypart.groupby(sel);
} // ibis::bord::groupby

/// Allocate a buffer of the specified type and size.
void* ibis::table::allocateBuffer(ibis::TYPE_T type, size_t sz) {
    void* ret = 0;
    switch (type) {
    default:
	LOGGER(ibis::gVerbose > 1)
	    << "Warning -- table::allocateBuffer("
	    << ibis::TYPESTRING[(int)type] << ", "
	    << sz << ") unable to handle the data type";
	break;
    case ibis::OID:
	ret = new array_t<rid_t>(sz);
	break;
    case ibis::BYTE:
	ret = new array_t<signed char>(sz);
	break;
    case ibis::UBYTE:
	ret = new array_t<unsigned char>(sz);
	break;
    case ibis::SHORT:
	ret = new array_t<int16_t>(sz);
	break;
    case ibis::USHORT:
	ret = new array_t<uint16_t>(sz);
	break;
    case ibis::INT:
	ret = new array_t<int32_t>(sz);
	break;
    case ibis::UINT:
	ret = new array_t<uint32_t>(sz);
	break;
    case ibis::LONG:
	ret = new array_t<int64_t>(sz);
	break;
    case ibis::ULONG:
	ret = new array_t<uint64_t>(sz);
	break;
    case ibis::FLOAT:
	ret = new array_t<float>(sz);
	break;
    case ibis::DOUBLE:
	ret = new array_t<double>(sz);
	break;
    case ibis::TEXT:
    case ibis::CATEGORY:
	ret = new std::vector<std::string>(sz);
        break;
    }
    return ret;
} // ibis::table::allocateBuffer

/// Freeing a buffer for storing in-memory values.
void ibis::table::freeBuffer(void *buffer, ibis::TYPE_T type) {
    if (buffer != 0) {
	switch (type) {
	default:
	    LOGGER(ibis::gVerbose > 1)
		<< "Warning -- table::freeBuffer(" << buffer << ", "
		<< (int) type << ") unable to handle data type "
		<< ibis::TYPESTRING[(int)type];
	    break;
	case ibis::OID:
	    delete static_cast<array_t<rid_t>*>(buffer);
	    break;
	case ibis::BYTE:
	    delete static_cast<array_t<signed char>*>(buffer);
	    break;
	case ibis::UBYTE:
	    delete static_cast<array_t<unsigned char>*>(buffer);
	    break;
	case ibis::SHORT:
	    delete static_cast<array_t<int16_t>*>(buffer);
	    break;
	case ibis::USHORT:
	    delete static_cast<array_t<uint16_t>*>(buffer);
	    break;
	case ibis::INT:
	    delete static_cast<array_t<int32_t>*>(buffer);
	    break;
	case ibis::UINT:
	    delete static_cast<array_t<uint32_t>*>(buffer);
	    break;
	case ibis::LONG:
	    delete static_cast<array_t<int64_t>*>(buffer);
	    break;
	case ibis::ULONG:
	    delete static_cast<array_t<uint64_t>*>(buffer);
	    break;
	case ibis::FLOAT:
	    delete static_cast<array_t<float>*>(buffer);
	    break;
	case ibis::DOUBLE:
	    delete static_cast<array_t<double>*>(buffer);
	    break;
	case ibis::TEXT:
	case ibis::CATEGORY:
	    delete static_cast<std::vector<std::string>*>(buffer);
	    break;
	}
    }
} // ibis::table::freeBuffer

/// Freeing a list of buffers.
void ibis::table::freeBuffers(ibis::table::bufferList& buf,
			     ibis::table::typeList& typ) {
    LOGGER(ibis::gVerbose > 3)
	<< "table::freeBuffers to free buf[" << buf.size() << "] and typ["
	<< typ.size() << "]";
    const size_t nbt = (buf.size() <= typ.size() ? buf.size() : typ.size());
    LOGGER((nbt < buf.size() || nbt < typ.size()) && ibis::gVerbose > 1)
	<< "Warning -- freeBuffers expects buf[" << buf.size()
	<< "] and typ["	<< typ.size()
	<< "] to be the same size, but they are not";

    for (size_t j = 0; j < buf.size(); ++ j) {
	if (buf[j] != 0) {
	    switch (typ[j]) {
	    default:
		LOGGER(ibis::gVerbose > 1)
		    << "Warning -- table::freeBuffers cann't free "
		    << buf[j] << " of type " << ibis::TYPESTRING[(int)typ[j]];
		break;
	    case ibis::OID:
		delete static_cast<array_t<ibis::rid_t>*>(buf[j]);
		break;
	    case ibis::BYTE:
		delete static_cast<array_t<signed char>*>(buf[j]);
		break;
	    case ibis::UBYTE:
		delete static_cast<array_t<unsigned char>*>(buf[j]);
		break;
	    case ibis::SHORT:
		delete static_cast<array_t<int16_t>*>(buf[j]);
		break;
	    case ibis::USHORT:
		delete static_cast<array_t<uint16_t>*>(buf[j]);
		break;
	    case ibis::INT:
		delete static_cast<array_t<int32_t>*>(buf[j]);
		break;
	    case ibis::UINT:
		delete static_cast<array_t<uint32_t>*>(buf[j]);
		break;
	    case ibis::LONG:
		delete static_cast<array_t<int64_t>*>(buf[j]);
		break;
	    case ibis::ULONG:
		delete static_cast<array_t<uint64_t>*>(buf[j]);
		break;
	    case ibis::FLOAT:
		delete static_cast<array_t<float>*>(buf[j]);
		break;
	    case ibis::DOUBLE:
		delete static_cast<array_t<double>*>(buf[j]);
		break;
	    case ibis::TEXT:
	    case ibis::CATEGORY:
		delete static_cast<std::vector<std::string>*>(buf[j]);
		break;
	    }
	}
    }
    buf.clear();
    typ.clear();
} // ibis::table::freeBuffers

/// Constructor.  This object can only store upto 4 billion rows because it
/// uses a 32-bit unsigned integer to store the number of rows.
///
/// @note The new object will take ownership of the arrays listed in buf.
/// All pointers in buf will be set to nil afterwards.
ibis::bord::part::part(const char *tn, const char *td, uint64_t nr,
		       const ibis::table::stringList &cn,
		       const ibis::table::typeList   &ct,
		       ibis::table::bufferList       &buf,
		       const ibis::table::stringList *cdesc)
    : ibis::part("in-core") {
    m_name = ibis::util::strnewdup(tn?tn:"unnamed");
    m_desc = (td ? td : tn ? tn : "");
    nEvents = static_cast<uint32_t>(nr);
    if (nEvents != nr) {
	LOGGER(ibis::gVerbose >= 0)
	    << "bord::part::part can not handle " << nr
	    << " rows in an in-memory table";
	throw "Too many rows for an in-memory table";
    }

    const uint32_t nc = (cn.size()<=ct.size() ? cn.size() : ct.size());
    for (uint32_t i = 0; i < nc; ++ i) {
	if (columns.find(cn[i]) == columns.end()) { // a new column
	    ibis::column *tmp;
	    if (cdesc != 0 && cdesc->size() > i)
		tmp = new ibis::bord::column(this, ct[i], cn[i], buf[i],
					     (*cdesc)[i]);
	    else
		tmp = new ibis::bord::column(this, ct[i], cn[i], buf[i]);
	    columns[tmp->name()] = tmp;
	    colorder.push_back(tmp);
	}
	else { // dupplicate name
	    LOGGER(ibis::gVerbose > 0)
		<< "Warning -- bord::part::ctor found column " << i << " ("
		<< cn[i] << ") to be a duplicate, discarding it...";
	    // free the buffer because it will not be freed anywhere else
	    freeBuffer(buf[i], ct[i]);
	}
	buf[i] = 0;
    }

    amask.set(1, nEvents);
    state = ibis::part::STABLE_STATE;
    LOGGER(ibis::gVerbose > 0)
	<< "bord::part constructed in-memory data partition "
	<< (m_name != 0 ? m_name : "<unnamed>") << " -- " << m_desc
	<< "\nwith " << nr << " row" << (nr > 1U ? "s" : "") << " and "
	<< columns.size() << " column" << (columns.size() > 1U ? "s" : "");
} // ibis::bord::part::part

void ibis::bord::part::describe(std::ostream& out) const {
    out << "Table (in memory) " << m_name << " (" << m_desc
	<< ") contsists of " << columns.size() << " column"
	<< (columns.size()>1 ? "s" : "") << " and " << nEvents
	<< " row" << (nEvents>1 ? "s" : "");
    if (colorder.empty()) {
	for (ibis::part::columnList::const_iterator it = columns.begin();
	     it != columns.end(); ++ it)
	    out << "\n" << (*it).first << "\t"
		<< ibis::TYPESTRING[(int)(*it).second->type()];
    }
    else if (colorder.size() == columns.size()) {
	for (uint32_t i = 0; i < columns.size(); ++ i) {
	    out << "\n" << colorder[i]->name() << "\t"
		<< ibis::TYPESTRING[(int)colorder[i]->type()];
	    if (colorder[i]->description() != 0)
		out << "\t" << colorder[i]->description();
	}
    }
    else {
	std::set<const char*, ibis::lessi> names;
	for (ibis::part::columnList::const_iterator it = columns.begin();
	     it != columns.end(); ++ it)
	    names.insert((*it).first);
	for (uint32_t i = 0; i < colorder.size(); ++ i) {
	    out << "\n" << colorder[i]->name() << "\t"
		<< ibis::TYPESTRING[(int)colorder[i]->type()];
	    names.erase(colorder[i]->name());
	}
	for (std::set<const char*, ibis::lessi>::const_iterator it =
		 names.begin(); it != names.end(); ++ it) {
	    ibis::part::columnList::const_iterator cit = columns.find(*it);
	    out << "\n" << (*cit).first << "\t"
		<< ibis::TYPESTRING[(int)(*cit).second->type()];
	    if (cit->second->description() != 0)
		out << "\t" << (*cit).second->description();
	}
    }
    out << std::endl;
} // ibis::bord::part::describe

void ibis::bord::part::dumpNames(std::ostream& out, const char* del) const {
    if (columns.empty()) return;

    if (colorder.empty()) {
	for (ibis::part::columnList::const_iterator it = columns.begin();
	     it != columns.end(); ++ it) {
	    if (it != columns.begin())
		out << del;
	    out << (*it).first;
	}
    }
    else if (colorder.size() == columns.size()) {
	out << colorder[0]->name();
	for (uint32_t i = 1; i < columns.size(); ++ i)
	    out << del << colorder[i]->name();
    }
    else {
	std::set<const char*, ibis::lessi> names;
	for (ibis::part::columnList::const_iterator it = columns.begin();
	     it != columns.end(); ++ it)
	    names.insert((*it).first);
	for (uint32_t i = 0; i < colorder.size(); ++ i) {
	    if (i > 0)
		out << del;
	    out << colorder[i]->name();
	    names.erase(colorder[i]->name());
	}
	for (std::set<const char*, ibis::lessi>::const_iterator it =
		 names.begin(); it != names.end(); ++ it) {
	    out << del << *it;
	}
    }
    out << std::endl;
} // ibis::bord::part::dumpNames

/**
   Print the first nr rows of the data to the given output stream.

   The return values:
@code
   0  -- normal (successful) completion
  -1  -- no data in-memory
  -2  -- unknown data type
  -3  -- some columns not ibis::bord::column (not in-memory)
  -4  -- error in the output stream
@endcode
 */
int ibis::bord::part::dump(std::ostream& out, uint32_t nr,
			   const char* del) const {
    const uint32_t ncol = columns.size();
    if (ncol == 0 || nr == 0) return 0;
    if (del == 0) del = ",";

    std::vector<const ibis::bord::column*> clist;
    if (colorder.empty()) { // alphabetic ordering
	for (ibis::part::columnList::const_iterator it = columns.begin();
	     it != columns.end(); ++ it) {
	    const ibis::bord::column* col =
		dynamic_cast<const ibis::bord::column*>((*it).second);
	    if (col != 0)
		clist.push_back(col);
	}
    }
    else if (colorder.size() == ncol) { // use external order
	for (uint32_t i = 0; i < ncol; ++ i) {
	    const ibis::bord::column* col =
		dynamic_cast<const ibis::bord::column*>(colorder[i]);
	    if (col != 0)
		clist.push_back(col);
	}
    }
    else { // externally specified ones are ordered first
	std::set<const char*, ibis::lessi> names;
	for (ibis::part::columnList::const_iterator it = columns.begin();
	     it != columns.end(); ++ it)
	    names.insert((*it).first);
	for (uint32_t i = 0; i < colorder.size(); ++ i) {
	    const ibis::bord::column* col =
		dynamic_cast<const ibis::bord::column*>(colorder[i]);
	    if (col != 0) {
		clist.push_back(col);
		names.erase(col->name());
	    }
	}
	for (std::set<const char*, ibis::lessi>::const_iterator it =
		 names.begin();
	     it != names.end(); ++ it) {
	    ibis::part::columnList::const_iterator cit = columns.find(*it);
	    const ibis::bord::column* col =
		dynamic_cast<const ibis::bord::column*>((*cit).second);
	    if (col != 0)
		clist.push_back(col);
	}
    }
    if (clist.size() < ncol) return -3;

    int ierr = 0;
    // print the first row with error checking
    ierr = clist[0]->dump(out, 0U);
    if (ierr < 0) return ierr;
    for (uint32_t j = 1; j < ncol; ++ j) {
	out << del;
	ierr = clist[j]->dump(out, 0U);
	if (ierr < 0) return ierr;
    }
    out << "\n";
    if (! out) return -4;
    // print the remaining rows without checking the return values from
    // functions called
    if (nr > nEvents) nr = nEvents;
    for (uint32_t i = 1; i < nr; ++ i) {
	(void) clist[0]->dump(out, i);
	for (uint32_t j = 1; j < ncol; ++ j) {
	    out << del;
	    (void) clist[j]->dump(out, i);
	}
	out << "\n";
    }
    if (! out)
	ierr = -4;
    return ierr;
} // ibis::bord::part::dump

/// Print nr rows starting with row offset.  Note that the row number
/// starts with 0, i.e., the first row is row 0.
int ibis::bord::part::dump(std::ostream& out, uint32_t offset, uint32_t nr,
			   const char* del) const {
    const uint32_t ncol = columns.size();
    if (ncol == 0 || nr == 0 || offset >= nEvents) return 0;
    if (del == 0) del = ",";

    std::vector<const ibis::bord::column*> clist;
    if (colorder.empty()) { // alphabetic ordering
	for (ibis::part::columnList::const_iterator it = columns.begin();
	     it != columns.end(); ++ it) {
	    const ibis::bord::column* col =
		dynamic_cast<const ibis::bord::column*>((*it).second);
	    if (col != 0)
		clist.push_back(col);
	}
    }
    else if (colorder.size() == ncol) { // use external order
	for (uint32_t i = 0; i < ncol; ++ i) {
	    const ibis::bord::column* col =
		dynamic_cast<const ibis::bord::column*>(colorder[i]);
	    if (col != 0)
		clist.push_back(col);
	}
    }
    else { // externally specified ones are ordered first
	std::set<const char*, ibis::lessi> names;
	for (ibis::part::columnList::const_iterator it = columns.begin();
	     it != columns.end(); ++ it)
	    names.insert((*it).first);
	for (uint32_t i = 0; i < colorder.size(); ++ i) {
	    const ibis::bord::column* col =
		dynamic_cast<const ibis::bord::column*>(colorder[i]);
	    if (col != 0) {
		clist.push_back(col);
		names.erase(col->name());
	    }
	}
	for (std::set<const char*, ibis::lessi>::const_iterator it =
		 names.begin();
	     it != names.end(); ++ it) {
	    ibis::part::columnList::const_iterator cit = columns.find(*it);
	    const ibis::bord::column* col =
		dynamic_cast<const ibis::bord::column*>((*cit).second);
	    if (col != 0)
		clist.push_back(col);
	}
    }
    if (clist.size() < ncol) return -3;

    int ierr = 0;
    // print row offset with error checking
    ierr = clist[0]->dump(out, offset);
    if (ierr < 0) return ierr;
    for (uint32_t j = 1; j < ncol; ++ j) {
	out << del;
	ierr = clist[j]->dump(out, offset);
	if (ierr < 0) return ierr;
    }
    out << "\n";
    if (! out) return -4;
    // print the remaining rows without checking the return values from
    // functions called
    nr += offset;
    if (nr > nEvents) nr = nEvents;
    for (uint32_t i = offset+1; i < nr; ++ i) {
	(void) clist[0]->dump(out, i);
	for (uint32_t j = 1; j < ncol; ++ j) {
	    out << del;
	    (void) clist[j]->dump(out, i);
	}
	out << "\n";
    }
    if (! out)
	ierr = -4;
    return ierr;
} // ibis::bord::part::dump

/// Write the content of partition into the specified directory dir.  The
/// directory dir must be writable.  If the second and third arguments are
/// valid, the output data will use them as the name and the description of
/// the data partition.
int ibis::bord::part::backup(const char* dir, const char* tname,
			     const char* tdesc) const {
    if (dir == 0 || *dir == 0) return -1;
    int ierr = ibis::util::makeDir(dir);
    if (ierr < 0) return ierr;

    if (tname == 0 || *tname == 0)
	tname = m_name;
    if (tdesc == 0 || *tdesc == 0)
	tdesc = m_desc.c_str();
    LOGGER(ibis::gVerbose > 1)
	<< "bord::backup starting to write " << nEvents << " row"
	<< (nEvents>1?"s":"") << " and " << columns.size() << " column"
	<< (columns.size()>1?"s":"") << " to " << dir << " as data partition "
	<< tname << " to " << dir;
    ibis::horometer timer;
    if (ibis::gVerbose > 0)
	timer.start();

    time_t currtime = time(0); // current time
    char stamp[28];
    ibis::util::secondsToString(currtime, stamp);
    std::string mdfile = dir;
    mdfile += FASTBIT_DIRSEP;
    mdfile += "-part.txt";
    std::ofstream md(mdfile.c_str());
    if (! md) {
	LOGGER(ibis::gVerbose > 0)
	    << "bord::backup(" << dir << ") failed to open metadata file "
	    "\"-part.txt\"";
	return -3; // metadata file not ready
    }

    ibis::bitvector msk0, msk1;
    msk0.set(0, nEvents);
    md << "# meta data for data partition " << tname
       << " written by bord::backup on " << stamp << "\n\n"
       << "BEGIN HEADER\nName = " << tname << "\nDescription = "
       << tdesc << "\nNumber_of_rows = " << nEvents
       << "\nNumber_of_columns = " << columns.size()
       << "\nTimestamp = " << currtime;
    if (idxstr != 0 && *idxstr != 0) {
	md << "\nindex = " << idxstr;
    }
    else { // try to find the default index specification
	std::string idxkey = "ibis.";
	idxkey += tname;
	idxkey += ".index";
	const char* str = ibis::gParameters()[idxkey.c_str()];
	if (str != 0 && *str != 0)
	    md << "\nindex = " << str;
    }
    md << "\nEND HEADER\n";

    for (columnList::const_iterator it = columns.begin();
	 it != columns.end(); ++ it) {
	const column& col = *(static_cast<column*>((*it).second));
	std::string cnm = dir;
	cnm += FASTBIT_DIRSEP;
	cnm += (*it).first;
	int fdes = UnixOpen(cnm.c_str(), OPEN_WRITEADD, OPEN_FILEMODE);
	if (fdes < 0) {
	    LOGGER(ibis::gVerbose >= 0)
		<< "bord::backup(" << dir << ") failed to open file "
		<< cnm << " for writing";
	    return -4;
	}
	ibis::util::guard gfdes = ibis::util::makeGuard(UnixClose, fdes);
#if defined(_WIN32) && defined(_MSC_VER)
	(void)_setmode(fdes, _O_BINARY);
#endif
	LOGGER(ibis::gVerbose > 2)
	    << "bord::backup opened file " << cnm
	    << " to write data for column " << (*it).first;
	std::string mskfile = cnm; // mask file name
	mskfile += ".msk";
	msk1.clear();

	switch (col.type()) {
	case ibis::BYTE: {
	    array_t<signed char> *values = col.selectBytes(msk0);
	    if (values != 0) {
		ierr = ibis::part::writeColumn
		    (fdes, 0, nEvents, *values,
		     (signed char)0x7F, msk1, msk0);
	    }
	    else {
		ierr = -4;
	    }
	    break;}
	case ibis::UBYTE: {
	    array_t<unsigned char> *values = col.selectUBytes(msk0);
	    if (values != 0)
		ierr = ibis::part::writeColumn
		    (fdes, 0, nEvents, *values,
		     (unsigned char)0xFF, msk1, msk0);
	    else
		ierr = -4;
	    break;}
	case ibis::SHORT: {
	    array_t<int16_t> *values = col.selectShorts(msk0);
	    if (values != 0) 
		ierr = ibis::part::writeColumn
		    (fdes, 0, nEvents, *values,
		     (int16_t)0x7FFF, msk1, msk0);
	    else
		ierr = -4;
	    break;}
	case ibis::USHORT: {
	    array_t<uint16_t> *values = col.selectUShorts(msk0);
	    if (values != 0)
		ierr = ibis::part::writeColumn
		    (fdes, 0, nEvents, *values,
		     (uint16_t)0xFFFF, msk1, msk0);
	    else
		ierr = -4;
	    break;}
	case ibis::INT: {
	    array_t<int32_t> *values = col.selectInts(msk0);
	    if (values != 0)
		ierr = ibis::part::writeColumn
		    (fdes, 0, nEvents, *values,
		     (int32_t)0x7FFFFFFF, msk1, msk0);
	    else
		ierr = -4;
	    break;}
	case ibis::UINT: {
	    array_t<uint32_t> *values = col.selectUInts(msk0);
	    if (values != 0)
		ierr = ibis::part::writeColumn
		    (fdes, 0, nEvents, *values,
		     (uint32_t)0xFFFFFFFF, msk1, msk0);
	    else
		ierr = -4;
	    break;}
	case ibis::LONG: {
	    array_t<int64_t> *values = col.selectLongs(msk0);
	    if (values != 0)
		ierr = ibis::part::writeColumn<int64_t>
		    (fdes, 0, nEvents, *values,
		     0x7FFFFFFFFFFFFFFFLL, msk1, msk0);
	    else
		ierr = -4;
	    break;}
	case ibis::ULONG: {
	    array_t<uint64_t> *values = col.selectULongs(msk0);
	    if (values != 0)
		ierr = ibis::part::writeColumn<uint64_t>
		    (fdes, 0, nEvents, *values, 0xFFFFFFFFFFFFFFFFULL,
		     msk1, msk0);
	    else
		ierr = -4;
	    break;}
	case ibis::FLOAT: {
	    array_t<float> *values = col.selectFloats(msk0);
	    if (values != 0)
		ierr = ibis::part::writeColumn
		    (fdes, 0, nEvents, *values,
		     std::numeric_limits<float>::quiet_NaN(), msk1, msk0);
	    else
		ierr = -4;
	    break;}
	case ibis::DOUBLE: {
	    array_t<double> *values = col.selectDoubles(msk0);
	    if (values != 0)
		ierr = ibis::part::writeColumn
		    (fdes, 0, nEvents, *values, 
		     std::numeric_limits<double>::quiet_NaN(), msk1, msk0);
	    else
		ierr = -4;
	    break;}
	case ibis::TEXT:
	case ibis::CATEGORY: {
	    std::vector<std::string> *values = col.selectStrings(msk0);
	    if (values != 0)
		ierr = ibis::part::writeString
		    (fdes, 0, nEvents, *values, msk1, msk0);
	    else
		ierr =-4;
	    break;}
// 	case ibis::BLOB: {
// 	    std::string spname = cnm;
// 	    spname += ".sp";
// 	    int sdes = UnixOpen(spname.c_str(), OPEN_READWRITE, OPEN_FILEMODE);
// 	    if (sdes < 0) {
// 		LOGGER(ibis::gVerbose >= 0)
// 		    << "bord::backup(" << dir << ") failed to open file "
// 		    << spname << " for writing the starting positions";
// 		return -4;
// 	    }
// 	    ibis::util::guard gsdes = ibis::util::makeGuard(UnixClose, sdes);
// #if defined(_WIN32) && defined(_MSC_VER)
// 	    (void)_setmode(sdes, _O_BINARY);
// #endif

// 	    ierr = ibis::part::writeRaw
// 		(fdes, sdes, 0, nEvents,
// 		 * static_cast<const array_t<unsigned char>*>(values),
// 		 col.starts, msk1, msk0);
// 	    break;}
	default:
	    break;
	}
#if defined(FASTBIT_SYNC_WRITE)
#if _POSIX_FSYNC+0 > 0
	(void) UnixFlush(fdes); // write to disk
#elif defined(_WIN32) && defined(_MSC_VER)
	(void) _commit(fdes);
#endif
#endif
	if (ierr < 0) {
	    LOGGER(ibis::gVerbose > 0)
		<< "bord::backup(" << dir << ") failed to write column "
		<< (*it).first << " (type " << ibis::TYPESTRING[(int)col.type()]
		<< ") to " << cnm;
	    return ierr;
	}

	LOGGER(msk1.cnt() != msk1.size() && ibis::gVerbose > 1)
	    << "Warning -- bord::backup(" << dir
	    << ") expected msk1 to contain only 1s for column " << col.name()
	    << ", but it has only " << msk1.cnt() << " out of " << msk1.size();
	remove(mskfile.c_str());

	md << "\nBegin Column\nname = " << (*it).first << "\ndata_type = "
	   << ibis::TYPESTRING[(int) col.type()];
	if (col.indexSpec() != 0 && *(col.indexSpec()) != 0) {
	    md << "\nindex = " << col.indexSpec();
	}
	else if (col.type() == ibis::TEXT) {
	    md << "\nindex=none";
	}
	else {
	    std::string idxkey = "ibis.";
	    idxkey += tname;
	    idxkey += ".";
	    idxkey += (*it).first;
	    idxkey += ".index";
	    const char* str = ibis::gParameters()[idxkey.c_str()];
	    if (str != 0)
		md << "\nindex = " << str;
	}
	md << "\nEnd Column\n";
    }
    md.close(); // close the file
    ibis::fileManager::instance().flushDir(dir);
    if (ibis::gVerbose > 0) {
	timer.stop();
	ibis::util::logger()()
	    << "bord::backup completed writing partition " 
	    << tname << " (" << tdesc << ") with " << columns.size()
	    << " column" << (columns.size()>1 ? "s" : "") << " and "
	    << nEvents << " row" << (nEvents>1 ? "s" : "") << ") to " << dir
	    << " using " << timer.CPUTime() << " sec(CPU), "
	    << timer.realTime() << " sec(elapsed)";
    }

    return 0;
} // ibis::bord::part::backup

/// The actual function to perform the group by operation.
/// @note The input argument can only contain column names and support
/// aggregation functions with column names arguments.  Not futher
/// arithmetic operations are allowed!
ibis::table*
ibis::bord::part::groupby(const ibis::selectClause& sel) const {
    if (sel.empty())
	return 0;

    std::string td = "GROUP BY ";
    td += *sel;
    td += " on table ";
    td += m_name;
    td += " (";
    td += m_desc;
    td += ')';
    std::string tn = ibis::util::shortName(td);
    if (nEvents == 0)
	return new ibis::tabula(tn.c_str(), td.c_str(), nEvents);

    std::vector<uint32_t> bad;
    // buf simply collects the pointers, not responsible for freeing them
    ibis::table::bufferList buf;
    const uint32_t nc = sel.size();
    buf.reserve(sel.size());
    for (uint32_t i = 0; i < sel.size(); ++ i) {
	columnList::const_iterator it = columns.find(sel.argName(i));
	if (it == columns.end())
	    it = columns.find(sel.termName(i));
	if (it != columns.end()) {
	    const ibis::bord::column* col =
		dynamic_cast<const ibis::bord::column*>((*it).second);
	    if (col != 0) {
		buf.push_back(col->getArray());
	    }
	    else {
		bad.push_back(i);
	    }
	}
	else if (*(sel.argName(i)) != '*') {
	    bad.push_back(i);
	}
    }
    if (! bad.empty()) {
	if (ibis::gVerbose >= 0) {
	    ibis::util::logger lg;
	    lg() << "Warning -- bord::part::groupby found " << bad.size()
		 << " unknown name" << (bad.size()>1?"s":"")
		 << " in \"";
	    sel.print(lg());
	    lg() << "\", cannot perform groupby operation";
	}
	return 0;
    }

    // create bundle
    std::auto_ptr<ibis::bundle> bdl(ibis::bundle::create(*this, sel, buf));
    buf.clear();
    if (bdl.get() == 0) {
	LOGGER(ibis::gVerbose >= 0)
	    << "Warning -- bord::part::groupby failed to create "
	    "bundle for \"" << *sel << "\" from in-memory data";

	return 0;
    }
    const uint32_t nr = bdl->size();
    if (nr == 0) {
	if (ibis::gVerbose > 0) {
	    ibis::util::logger lg;
	    lg() << "Warning -- bord::part::groupby(";
	    sel.print(lg());
	    lg() << ") produced no answer on a table with nRows = "
		 << nEvents;
	}
	return 0;
    }

    // convert bundle back into a partition, first generate the name and
    // description for the new table
    if (nc == 0)
	return new ibis::tabula(tn.c_str(), td.c_str(), nr);

    // prepare the types and values for the new table
    std::vector<std::string> nms(nc), des(nc);
    ibis::table::stringList  nmc(nc), dec(nc);
    ibis::table::typeList    tps(nc);
    buf.resize(nc);
    ibis::util::guard gbuf
	= ibis::util::makeGuard(ibis::table::freeBuffers, buf, tps);
    uint32_t jbdl = 0;
    bool countstar = false;
    for (uint32_t i = 0; i < nc; ++ i) {
	void *bptr = 0;
	nms[i] = sel.termName(i);
	nmc[i] = nms[i].c_str();
	sel.describe(i, des[i]);
	dec[i] = des[i].c_str();
	if (*(sel.argName(i)) == '*' &&
	    sel.getAggregator(i) == ibis::selectClause::CNT) {
	    countstar = true;
	    array_t<uint32_t>* cnts = new array_t<uint32_t>;
	    bdl->rowCounts(*cnts);
	    tps[i] = ibis::UINT;
	    buf[i] = cnts;
	    continue;
	}
	else {
	    tps[i] = bdl->columnType(jbdl);
	    bptr = bdl->columnArray(jbdl);
	    ++ jbdl;
	}

	if (bptr == 0) {
	    buf[i] = 0;
	    continue;
	}

	switch (tps[i]) {
	case ibis::BYTE:
	    buf[i] = new array_t<signed char>
		(* static_cast<const array_t<signed char>*>(bptr));
	    break;
	case ibis::UBYTE:
	    buf[i] = new array_t<unsigned char>
		(* static_cast<const array_t<unsigned char>*>(bptr));
	    break;
	case ibis::SHORT:
	    buf[i] = new array_t<int16_t>
		(* static_cast<const array_t<int16_t>*>(bptr));
	    break;
	case ibis::USHORT:
	    buf[i] = new array_t<uint16_t>
		(* static_cast<const array_t<uint16_t>*>(bptr));
	    break;
	case ibis::INT:
	    buf[i] = new array_t<int32_t>
		(* static_cast<const array_t<int32_t>*>(bptr));
	    break;
	case ibis::UINT:
	    buf[i] = new array_t<uint32_t>
		(* static_cast<const array_t<uint32_t>*>(bptr));
	    break;
	case ibis::LONG:
	    buf[i] = new array_t<int64_t>
		(* static_cast<const array_t<int64_t>*>(bptr));
	    break;
	case ibis::ULONG:
	    buf[i] = new array_t<uint64_t>
		(* static_cast<const array_t<uint64_t>*>(bptr));
	    break;
	case ibis::FLOAT:
	    buf[i] = new array_t<float>
		(* static_cast<const array_t<float>*>(bptr));
	    break;
	case ibis::DOUBLE:
	    buf[i] = new array_t<double>
		(* static_cast<const array_t<double>*>(bptr));
	    break;
	case ibis::TEXT:
	case ibis::CATEGORY: {
	    std::vector<std::string> &bstr =
		* static_cast<std::vector<std::string>*>(bptr);
	    std::vector<std::string> *tmp =
		new std::vector<std::string>(bstr.size());
	    for (uint32_t j = 0; j < bstr.size(); ++ j)
		bstr[j].swap((*tmp)[j]);
	    buf[i] = tmp;
	    break;}
	default:
	    LOGGER(ibis::gVerbose >= 0)
		<< "Warning -- " << td << " can not process column "
		<< nms[i] << " (" << des[i] << ") of type "
		<< ibis::TYPESTRING[static_cast<int>(tps[i])];
	    buf[i] = 0;
	    break;
	}
    }
    if (! countstar) {// if count(*) is not already there, add it
	array_t<uint32_t>* cnts = new array_t<uint32_t>;
	bdl->rowCounts(*cnts);
	nmc.push_back("count0");
	dec.push_back("COUNT(*)");
	tps.push_back(ibis::UINT);
	buf.push_back(cnts);
    }
    std::auto_ptr<ibis::table>
	brd(new ibis::bord(tn.c_str(), td.c_str(), nr, buf, tps, nmc, &dec));
    if (brd.get() != 0)
	// buf has been successfully transferred to the new table object
	gbuf.dismiss();
    return brd.release();
} // ibis::bord::part::groupby

long ibis::bord::part::reorder(const ibis::table::stringList& cols) {
    long ierr = 0;
    if (nRows() == 0 || nColumns() == 0) return ierr;

    std::string evt = "part[";
    evt += m_name;
    evt += "]::reorder";
    writeLock lock(this, evt.c_str()); // can't process other operations
    for (columnList::const_iterator it = columns.begin();
	 it != columns.end();
	 ++ it) { // purge all index files
	(*it).second->unloadIndex();
	(*it).second->purgeIndexFile();
    }

    // look through all columns to match the incoming column names
    typedef std::vector<ibis::column*> colVector;
    std::set<const char*, ibis::lessi> used;
    colVector keys, load; // sort according to the keys
    for (ibis::table::stringList::const_iterator nit = cols.begin();
	 nit != cols.end(); ++ nit) {
	ibis::part::columnList::iterator it = columns.find(*nit);
	if (it != columns.end()) {
	    used.insert((*it).first);
	    if ((*it).second->upperBound() > (*it).second->lowerBound()) {
		keys.push_back((*it).second);
	    }
	    else {
		(*it).second->computeMinMax();
		if ((*it).second->upperBound() > (*it).second->lowerBound())
		    keys.push_back((*it).second);
		else
		    load.push_back((*it).second);
	    }
	}
    }

    if (keys.empty()) { // use all integral values
	if (ibis::gVerbose > 2) {
	    if (cols.empty()) {
		LOGGER(true)
		    << evt << " user did not specify ordering keys, will "
		    "attempt to use all integer columns as ordering keys";
	    }
	    else {
		std::ostringstream oss;
		oss << cols[0];
		for (unsigned i = 1; i < cols.size(); ++ i)
		    oss << ", " << cols[i];
		LOGGER(true)
		    << evt << " user specified ordering keys \"" << oss.str()
		    << "\" does not match any numerical columns with more "
		    "than one distinct value, will attempt to use "
		    "all integer columns as ordering keys";
	    }
	}

	load.clear();
	keys.clear();
	array_t<double> width;
	for (ibis::part::columnList::iterator it = columns.begin();
	     it != columns.end(); ++ it) {
	    if (! (*it).second->isInteger()) {
		load.push_back((*it).second);
	    }
	    else if ((*it).second->upperBound() > (*it).second->lowerBound()) {
		keys.push_back((*it).second);
		width.push_back((*it).second->upperBound() -
				(*it).second->lowerBound());
	    }
	    else {
		double cmin, cmax;
		(*it).second->computeMinMax(0, cmin, cmax);
		if (cmax > cmin) {
		    keys.push_back((*it).second);
		    width.push_back(cmax - cmin);
		}
		else {
		    load.push_back((*it).second);
		}
	    }
	}
	if (keys.empty()) return 0; //no integral values to use as key
	if (keys.size() > 1) {
	    colVector tmp(keys.size());
	    array_t<uint32_t> idx;
	    width.sort(idx);
	    for (uint32_t i = 0; i < keys.size(); ++ i)
		tmp[i] = keys[idx[i]];
	    tmp.swap(keys);
	}
    }
    else {
	for (ibis::part::columnList::const_iterator it = columns.begin();
	     it != columns.end(); ++ it) {
	    std::set<const char*, ibis::lessi>::const_iterator uit =
		used.find((*it).first);
	    if (uit == used.end())
		load.push_back((*it).second);
	}
    }
    if (keys.empty()) {
	LOGGER(ibis::gVerbose > 1)
	    << evt << " no keys found for sorting, do nothing";
	return 0;
    }
    if (ibis::gVerbose > 1) {
	std::ostringstream oss;
	oss << evt << '(' << keys[0]->name();
	for (unsigned i = 1; i < keys.size(); ++ i)
	    oss << ", " << keys[i]->name();
	oss << ')';
	evt = oss.str();
    }
    ibis::util::timer mytimer(evt.c_str(), 1);

    ierr = nEvents;
    array_t<uint32_t> ind1;
    { // the sorting loop -- use a block to limit the scope of starts and ind0
	array_t<uint32_t>  starts, ind0;
	for (uint32_t i = 0; i < keys.size(); ++ i) {
	    ibis::bord::column* col =
		dynamic_cast<ibis::bord::column*>(keys[i]);
	    if (col == 0) {
		logError("reorder", "all columns must be in-memory");
		return -2;
	    }

	    switch (keys[i]->type()) {
	    case ibis::TEXT:
	    case ibis::CATEGORY:
		ierr = sortStrings(* static_cast<std::vector<std::string>*>
				   (col->getArray()), ind1, ind0, starts);
		break;
	    case ibis::DOUBLE:
		ierr = sortValues(* static_cast<array_t<double>*>
				  (col->getArray()), ind1, ind0, starts);
		break;
	    case ibis::FLOAT:
		ierr = sortValues(* static_cast<array_t<float>*>
				  (col->getArray()), ind1, ind0, starts);
		break;
	    case ibis::ULONG:
		ierr = sortValues(* static_cast<array_t<uint64_t>*>
				  (col->getArray()), ind1, ind0, starts);
		break;
	    case ibis::LONG:
		ierr = sortValues(* static_cast<array_t<int64_t>*>
				  (col->getArray()), ind1, ind0, starts);
		break;
	    case ibis::UINT:
		ierr = sortValues(* static_cast<array_t<uint32_t>*>
				  (col->getArray()), ind1, ind0, starts);
		break;
	    case ibis::INT:
		ierr = sortValues(* static_cast<array_t<int32_t>*>
				  (col->getArray()), ind1, ind0, starts);
		break;
	    case ibis::USHORT:
		ierr = sortValues(* static_cast<array_t<uint16_t>*>
				  (col->getArray()), ind1, ind0, starts);
		break;
	    case ibis::SHORT:
		ierr = sortValues(* static_cast<array_t<int16_t>*>
				  (col->getArray()), ind1, ind0, starts);
		break;
	    case ibis::UBYTE:
		ierr = sortValues(* static_cast<array_t<unsigned char>*>
				  (col->getArray()), ind1, ind0, starts);
		break;
	    case ibis::BYTE:
		ierr = sortValues(* static_cast<array_t<signed char>*>
				  (col->getArray()), ind1, ind0, starts);
		break;
	    default:
		logWarning("reorder", "column %s type %d is not supported",
			   keys[i]->name(), static_cast<int>(keys[i]->type()));
		break;
	    }

	    if (ierr == static_cast<long>(nRows())) {
		ind1.swap(ind0);
	    }
	    else {
		logError("reorder", "failed to reorder column %s, ierr=%ld.  "
			 "data files are no longer consistent!",
			 keys[i]->name(), ierr);
	    }
	}
    }
#if DEBUG+0 > 0 || _DEBUG+0 > 0
    {
	ibis::util::logger lg(4);
	lg() << "DEBUG -- bord[" << name() << "]::reorder --\n";
	std::vector<bool> marks(ind1.size(), false);
	for (uint32_t i = 0; i < ind1.size(); ++ i) {
	    if (ibis::gVerbose > 6)
		lg() << "ind[" << i << "]=" << ind1[i] << "\n";
	    if (ind1[i] < marks.size())
		marks[ind1[i]] = true;
	}
	bool isperm = true;
	for (uint32_t i = 0; isperm && i < marks.size(); ++ i)
	    isperm = marks[i];
	if (isperm)
	    lg() << "array ind IS a permutation\n";
	else
	    lg() << "array ind is NOT a permutation\n";
    }
#endif
    for (ibis::part::columnList::const_iterator it = columns.begin();
	 it != columns.end();
	 ++ it) { // update the m_sorted flag of each column
	(*it).second->isSorted((*it).second == keys[0]);
    }

    for (uint32_t i = 0; i < load.size(); ++ i) {
	ibis::bord::column* col = dynamic_cast<ibis::bord::column*>(load[i]);
	if (col == 0) {
	    logError("reorder", "all columns must be in-memory");
	    return -2;
	}

	switch (load[i]->type()) {
	case ibis::CATEGORY:
	case ibis::TEXT:
	    ierr = reorderStrings(* static_cast<std::vector<std::string>*>
				  (col->getArray()), ind1);
	    break;
	case ibis::DOUBLE:
	    ierr = reorderValues(* static_cast<array_t<double>*>
				 (col->getArray()), ind1);
	    break;
	case ibis::FLOAT:
	    ierr = reorderValues(* static_cast<array_t<float>*>
				 (col->getArray()), ind1);
	    break;
	case ibis::ULONG:
	    ierr = reorderValues(* static_cast<array_t<uint64_t>*>
				 (col->getArray()), ind1);
	    break;
	case ibis::LONG:
	    ierr = reorderValues(* static_cast<array_t<int64_t>*>
				 (col->getArray()), ind1);
	    break;
	case ibis::UINT:
	    ierr = reorderValues(* static_cast<array_t<uint32_t>*>
				 (col->getArray()), ind1);
	    break;
	case ibis::INT:
	    ierr = reorderValues(* static_cast<array_t<int32_t>*>
				 (col->getArray()), ind1);
	    break;
	case ibis::USHORT:
	    ierr = reorderValues(* static_cast<array_t<uint16_t>*>
				 (col->getArray()), ind1);
	    break;
	case ibis::SHORT:
	    ierr = reorderValues(* static_cast<array_t<int16_t>*>
				 (col->getArray()), ind1);
	    break;
	case ibis::UBYTE:
	    ierr = reorderValues(* static_cast<array_t<unsigned char>*>
				 (col->getArray()), ind1);
	    break;
	case ibis::BYTE:
	    ierr = reorderValues(* static_cast<array_t<signed char>*>
				 (col->getArray()), ind1);
	    break;
	default:
	    logError("reorder", "column %s type %s is not supported",
		     keys[i]->name(),
		     ibis::TYPESTRING[static_cast<int>(keys[i]->type())]);
	    break;
	}
    }
    return ierr;
} // ibis::bord::part::reorder

template <typename T>
long ibis::bord::part::sortValues(array_t<T>& vals,
				  const array_t<uint32_t>& idxin,
				  array_t<uint32_t>& idxout,
				  array_t<uint32_t>& starts) const {
    ibis::horometer timer;
    if (ibis::gVerbose > 4)
	timer.start();

    if (vals.size() != nEvents ||
	(idxin.size() != vals.size() && ! idxin.empty())) {
	LOGGER(ibis::gVerbose > 1)
	    << "Warning -- bord[" << name() << "]::part::sortValues<"
	    << typeid(T).name() << "> can not proceed because array sizes do "
	    "not match, both vals.size(" << vals.size() << ") and idxin.size("
	    << idxin.size() << ") are expected to be " << nEvents;
	return -3;
    }
    if (idxin.empty() || starts.size() < 2 || starts[0] != 0
	|| starts.back() != vals.size()) {
	vals.nosharing(); // make a copy if necessary
	starts.resize(2);
	starts[0] = 0;
	starts[1] = vals.size();
	LOGGER(ibis::gVerbose > 1)
	    << "bord[" << name() << "]::part::sortValues<" << typeid(T).name()
	    << "> (re)set array starts to contain [0, " << nEvents << "]";
    }

    uint32_t nseg = starts.size() - 1;
    bool needreorder = false;
    if (nseg > nEvents) { // no sorting necessary
	idxout.copy(idxin);
    }
    else if (nseg > 1) { // sort multiple blocks
	idxout.resize(nEvents);
	array_t<uint32_t> starts2;

	for (uint32_t iseg = 0; iseg < nseg; ++ iseg) {
	    const uint32_t segstart = starts[iseg];
	    const uint32_t segsize = starts[iseg+1]-starts[iseg];
	    if (segsize > 2) {
		// copy the segment into a temporary array, then sort it
		array_t<T> tmp(segsize);
		array_t<uint32_t> ind0;
		for (unsigned i = 0; i < segsize; ++ i)
		    tmp[i] = vals[idxin[i+segstart]];
		tmp.sort(ind0);

		starts2.push_back(segstart);
		T last = tmp[ind0[0]];
		idxout[segstart] = idxin[ind0[0] + segstart];
		for (unsigned i = 1; i < segsize; ++ i) {
		    idxout[i+segstart] = idxin[ind0[i] + segstart];
		    if (tmp[ind0[i]] > last) {
			starts2.push_back(i + segstart);
			last = tmp[ind0[i]];
		    }
		}
	    }
	    else if (segsize == 2) { // two-element segment
		if (vals[idxin[segstart]] < vals[idxin[segstart+1]]) {
		    // in the right order
		    idxout[segstart] = idxin[segstart];
		    idxout[segstart+1] = idxin[segstart+1];
		    starts2.push_back(segstart);
		    starts2.push_back(segstart+1);
		}
		else if (vals[idxin[segstart]] == vals[idxin[segstart+1]]) {
		    idxout[segstart] = idxin[segstart];
		    idxout[segstart+1] = idxin[segstart+1];
		    starts2.push_back(segstart);
		}
		else { // assum the 1st value is larger (could be
		       // incomparable though)
		    idxout[segstart] = idxin[segstart+1];
		    idxout[segstart+1] = idxin[segstart];
		    starts2.push_back(segstart);
		    starts2.push_back(segstart+1);
		}
	    }
	    else { // segment contains only one element
		starts2.push_back(segstart);
		idxout[segstart] = idxin[segstart];
	    }
	}
	starts2.push_back(nEvents);
	starts.swap(starts2);
	needreorder = true;
    }
    else { // all in one block
	idxout.resize(nEvents);
	for (uint32_t j = 0; j < nEvents; ++ j)
	    idxout[j] = j;
	ibis::util::sortKeys(vals, idxout);

	starts.clear();
	starts.push_back(0U);
	T last = vals[0];
	for (uint32_t i = 1; i < nEvents; ++ i) {
	    if (vals[i] > last) {
		starts.push_back(i);
		last = vals[i];
	    }
	}
	starts.push_back(nEvents);
    }

    if (needreorder) { // place values in the new order
	array_t<T> tmp(nEvents);
	for (uint32_t i = 0; i < nEvents; ++ i)
	    tmp[i] = vals[idxout[i]];
	vals.swap(tmp);
    }
    if (ibis::gVerbose > 4) {
	timer.stop();
	nseg = starts.size() - 1;
	logMessage("sortValues", "reordered %lu value%s (into %lu "
		   "segment%s) in %g sec(CPU), %g sec(elapsed)",
		   static_cast<long unsigned>(nEvents), (nEvents>1 ? "s" : ""),
		   static_cast<long unsigned>(nseg), (nseg>1 ? "s" : ""),
		   timer.CPUTime(), timer.realTime());
    }
    return nEvents;
} // ibis::bord::part::sortValues

/// Sort the string values.  It preserves the previous order determined
/// represented by idxin and starts.
long ibis::bord::part::sortStrings(std::vector<std::string>& vals,
				  const array_t<uint32_t>& idxin,
				  array_t<uint32_t>& idxout,
				  array_t<uint32_t>& starts) const {
    ibis::horometer timer;
    if (ibis::gVerbose > 4)
	timer.start();

    if (vals.size() != nEvents ||
	(idxin.size() != vals.size() && ! idxin.empty())) {
	LOGGER(ibis::gVerbose > 1)
	    << "Warning -- bord[" << name() << "]::part::sortStrings "
	    " can not proceed because array sizes do not match, both "
	    "vals.size(" << vals.size() << ") and idxin.size(" << idxin.size()
	    << ") are expected to be " << nEvents;
	return -3;
    }
    if (idxin.empty() || starts.size() < 2 || starts[0] != 0
	|| starts.back() != vals.size()) {
	starts.resize(2);
	starts[0] = 0;
	starts[1] = vals.size();
	LOGGER(ibis::gVerbose > 1)
	    << "bord[" << name() << "]::part::sortStrings -- (re)set array "
	    "starts to contain [0, " << nEvents << "]";
    }

    bool needreorder = false;
    uint32_t nseg = starts.size() - 1;
    if (nseg > nEvents) { // no sorting necessary
	idxout.copy(idxin);
    }
    else if (nseg > 1) { // sort multiple blocks
	idxout.resize(nEvents);
	array_t<uint32_t> starts2;

	for (uint32_t iseg = 0; iseg < nseg; ++ iseg) {
	    const uint32_t segstart = starts[iseg];
	    const uint32_t segsize = starts[iseg+1]-starts[iseg];
	    if (segsize > 2) {
		// copy the segment into a temporary array, then sort it
		std::vector<std::string> tmp(segsize);
		array_t<uint32_t> ind0(segsize);
		for (unsigned i = segstart; i < starts[iseg+1]; ++ i) {
		    tmp[i-segstart] = vals[idxin[i]];
		    ind0[i-segstart] = idxin[i];
		}
		// sort tmp and move ind0
		ibis::util::sortStrings(tmp, ind0);

		starts2.push_back(segstart);
		uint32_t last = 0;
		idxout[segstart] = ind0[0];
		for (unsigned i = 1; i < segsize; ++ i) {
		    idxout[i+segstart] = ind0[i];
		    if (tmp[i].compare(tmp[last]) != 0) {
			starts2.push_back(i + segstart);
			last = i;
		    }
		}
	    }
	    else if (segsize == 2) {
		int cmp = vals[idxin[segstart]].compare
		    (vals[idxin[segstart+1]]);
		if (cmp < 0) { // in the right order, different strings
		    idxout[segstart] = idxin[segstart];
		    idxout[segstart+1] = idxin[segstart+1];
		    starts2.push_back(segstart);
		    starts2.push_back(segstart+1);
		}
		else if (cmp == 0) { // two strings are the same
		    idxout[segstart] = idxin[segstart];
		    idxout[segstart+1] = idxin[segstart+1];
		    starts2.push_back(segstart);
		}
		else { // in the wrong order, different strings
		    idxout[segstart] = idxin[segstart+1];
		    idxout[segstart+1] = idxin[segstart];
		    starts2.push_back(segstart);
		    starts2.push_back(segstart+1);
		}
	    }
	    else { // segment contains only one element
		starts2.push_back(segstart);
		idxout[segstart] = idxin[segstart];
	    }
	}
	starts2.push_back(nEvents);
	starts.swap(starts2);
	needreorder = true;
    }
    else { // all in one block
	idxout.resize(nEvents);
	for (uint32_t j = 0; j < nEvents; ++ j)
	    idxout[j] = j;
	ibis::util::sortStrings(vals, idxout);

	starts.clear();
	starts.push_back(0U);
	uint32_t last = 0;
	for (uint32_t i = 1; i < nEvents; ++ i) {
	    if (vals[i].compare(vals[last]) != 0) {
		starts.push_back(i);
		last = i;
	    }
	}
	starts.push_back(nEvents);
    }

    if (needreorder) { // place values in the new order
	std::vector<std::string> tmp(nEvents);
	for (uint32_t i = 0; i < nEvents; ++ i)
	    tmp[i].swap(vals[idxout[i]]);
	vals.swap(tmp);
    }
    if (ibis::gVerbose > 4) {
	timer.stop();
	nseg = starts.size() - 1;
	logMessage("sortStrings", "reordered %lu string%s (into %lu "
		   "segment%s) in %g sec(CPU), %g sec(elapsed)",
		   static_cast<long unsigned>(nEvents), (nEvents>1 ? "s" : ""),
		   static_cast<long unsigned>(nseg), (nseg>1 ? "s" : ""),
		   timer.CPUTime(), timer.realTime());
    }
    return nEvents;
} // ibis::bord::part::sortStrings

template <typename T>
long ibis::bord::part::reorderValues(array_t<T>& vals,
				     const array_t<uint32_t>& ind) const {
    if (vals.size() != nEvents || ind.size() != vals.size()) {
	if (ibis::gVerbose > 1)
	    logMessage("reorderValues", "array sizes do not match, both "
		       "vals.size(%ld) and ind.size(%ld) are expected to "
		       "be %ld", static_cast<long>(vals.size()),
		       static_cast<long>(ind.size()),
		       static_cast<long>(nEvents));
	return -3;
    }
    array_t<T> tmp(vals.size());
    for (uint32_t i = 0; i < vals.size(); ++ i)
	tmp[i] = vals[ind[i]];
    tmp.swap(vals);
    return nEvents;
} // ibis::bord::part::reorderValues

/// Reorder the vector of strings.  To avoid recreating the content of the
/// string values, this function uses swap operations to move the existing
/// strings into their new locations.  It only works if ind is a proper
/// permutation of integers between 0 and vals.size() (include 0 but
/// exclude vals.size()), however, it does not check whether the input
/// array is a proper permutation.
long ibis::bord::part::reorderStrings(std::vector<std::string>& vals,
				      const array_t<uint32_t>& ind) const {
    if (vals.size() != nEvents || ind.size() != vals.size()) {
	if (ibis::gVerbose > 1)
	    logMessage("reorderValues", "array sizes do not match, both "
		       "vals.size(%ld) and ind.size(%ld) are expected to "
		       "be %ld", static_cast<long>(vals.size()),
		       static_cast<long>(ind.size()),
		       static_cast<long>(nEvents));
	return -3;
    }
    std::vector<std::string> tmp(vals.size());
    for (uint32_t i = 0; i < vals.size(); ++ i)
	tmp[i].swap(vals[ind[i]]);
    tmp.swap(vals);
    return nEvents;
} // ibis::bord::part::reorderValues

void ibis::bord::part::reverseRows() {
    for (ibis::part::columnList::iterator it = columns.begin();
	 it != columns.end(); ++ it) {
	reinterpret_cast<ibis::bord::column*>((*it).second)->reverseRows();
    }
} // ibis::bord::reverseRows

int ibis::bord::part::limit(uint32_t nr) {
    int ierr = 0;
    if (nEvents <= nr) return ierr;

    for (ibis::part::columnList::iterator it = columns.begin();
	 it != columns.end(); ++ it) {
	ierr = reinterpret_cast<ibis::bord::column*>((*it).second)->limit(nr);
	if (ierr < 0) return ierr;
    }
    nEvents = nr;
    return ierr;
} // ibis::bord::part::limit

ibis::table*
ibis::bord::part::evaluateTerms(const ibis::selectClause& sel,
				const char* desc) const {
    std::string mydesc;
    if (desc == 0 || *desc == 0) {
	mydesc = sel.getString();
	mydesc += " on ";
	mydesc += m_desc;
	desc = mydesc.c_str();
    }
    std::string tn = ibis::util::shortName(desc);
    if (nEvents == 0 || columns.empty() || sel.empty()) {
	return new ibis::tabula(tn.c_str(), desc, nEvents);
    }

    long ierr;
    ibis::bitvector msk;
    msk.set(1, nEvents);
    ibis::table::bufferList  buf(sel.size(), 0);
    ibis::table::typeList    ct(sel.size(), ibis::UNKNOWN_TYPE);
    ibis::table::stringList  cn(sel.size());
    ibis::table::stringList  cd(sel.size());
    std::vector<std::string> cdesc(sel.size());
    ibis::util::guard gbuf =
	ibis::util::makeGuard(ibis::table::freeBuffers, buf, ct);
    for (uint32_t j = 0; j < sel.size(); ++ j) {
	const ibis::math::term* t = sel[j];

	switch (t->termType()) {
	default: {
	    sel.describe(j, cdesc[j]);
	    cn[j] = sel.termName(j);
	    ct[j] = ibis::DOUBLE;
	    buf[j] = new ibis::array_t<double>(nEvents);
	    ierr = calculate(*t, msk,
			     *static_cast< ibis::array_t<double>* >(buf[j]));
	    if (ierr != (long)nEvents) {
		LOGGER(ibis::gVerbose > 0)
		    << "Warning -- bord::part::evaluateTerms(" << desc
		    << ") failed to evaluate term " << j << " (" << cdesc[j]
		    << "), ierr = " << ierr;
		return 0;
	    }
	    break;}
	case ibis::math::NUMBER: { // a fixed number
	    sel.describe(j, cdesc[j]);
	    cn[j] = sel.termName(j);
	    ct[j] = ibis::DOUBLE;
	    buf[j] = new ibis::array_t<double>(nEvents, t->eval());
	    break;}
	case ibis::math::STRING: { // a string literal
	    sel.describe(j, cdesc[j]);
	    cn[j] = sel.termName(j);
	    ct[j] = ibis::CATEGORY;
	    buf[j] = new std::vector<std::string>
		(nEvents, (char*)*static_cast<const ibis::math::literal*>(t));
	    break;}
	case ibis::math::VARIABLE: {
	    const char* cn1 = static_cast<const ibis::math::variable*>
		(t)->variableName();
	    cn[j] = cn1;
	    cdesc[j] = cn1;
	    ibis::part::columnList::const_iterator it = columns.find(cn1);
	    if (it != columns.end()) {
		ct[j] = it->second->type();
		switch (ct[j]) {
		default: {
		    LOGGER(ibis::gVerbose > 0)
			<< "Warning -- bord::part::evaluateTerms(" << desc
			<< ") can not handle column " << j << " type "
			<< ibis::TYPESTRING[(int)ct[j]];
		    return 0;}
		case ibis::BYTE:
		    buf[j] = new ibis::array_t<signed char>;
		    ierr = it->second->selectValues(msk, buf[j]);
		    if (ierr < 0) {
			LOGGER(ibis::gVerbose > 0)
			    << "Warning -- bord::part::evaluateTerms(" << desc
			    << ") expected to retrieve " << nEvents
			    << " values for column " << j << " (" << cn1
			    << ", " << ibis::TYPESTRING[(int)ct[j]]
			    << "), but got " << ierr;
			return 0;
		    }
		    break;
		case ibis::UBYTE:
		    buf[j] = new ibis::array_t<unsigned char>;
		    ierr = it->second->selectValues(msk, buf[j]);
		    if (ierr < 0) {
			LOGGER(ibis::gVerbose > 0)
			    << "Warning -- bord::part::evaluateTerms(" << desc
			    << ") expected to retrieve " << nEvents
			    << " values " << j << " (" << cn1
			    << ", " << ibis::TYPESTRING[(int)ct[j]]
			    << "), but got " << ierr;
			return 0;
		    }
		    break;
		case ibis::SHORT:
		    buf[j] = new ibis::array_t<int16_t>;
		    ierr = it->second->selectValues(msk, buf[j]);
		    if (ierr < 0) {
			LOGGER(ibis::gVerbose > 0)
			    << "Warning -- bord::part::evaluateTerms(" << desc
			    << ") expected to retrieve " << nEvents
			    << " values for column " << j << " (" << cn1
			    << ", " << ibis::TYPESTRING[(int)ct[j]]
			    << "), but got " << ierr;
			return 0;
		    }
		    break;
		case ibis::USHORT:
		    buf[j] = new ibis::array_t<uint16_t>;
		    ierr = it->second->selectValues(msk, buf[j]);
		    if (ierr < 0) {
			LOGGER(ibis::gVerbose > 0)
			    << "Warning -- bord::part::evaluateTerms(" << desc
			    << ") expected to retrieve " << nEvents
			    << " values " << j << " (" << cn1
			    << ", " << ibis::TYPESTRING[(int)ct[j]]
			    << "), but got " << ierr;
			return 0;
		    }
		    break;
		case ibis::INT:
		    buf[j] = new ibis::array_t<int32_t>;
		    ierr = it->second->selectValues(msk, buf[j]);
		    if (ierr < 0) {
			LOGGER(ibis::gVerbose > 0)
			    << "Warning -- bord::part::evaluateTerms(" << desc
			    << ") expected to retrieve " << nEvents
			    << " values for column " << j << " (" << cn1
			    << ", " << ibis::TYPESTRING[(int)ct[j]]
			    << "), but got " << ierr;
			return 0;
		    }
		    break;
		case ibis::UINT:
		    buf[j] = new ibis::array_t<uint32_t>;
		    ierr = it->second->selectValues(msk, buf[j]);
		    if (ierr < 0) {
			LOGGER(ibis::gVerbose > 0)
			    << "Warning -- bord::part::evaluateTerms(" << desc
			    << ") expected to retrieve " << nEvents
			    << " values " << j << " (" << cn1
			    << ", " << ibis::TYPESTRING[(int)ct[j]]
			    << "), but got " << ierr;
			return 0;
		    }
		    break;
		case ibis::LONG:
		    buf[j] = new ibis::array_t<int64_t>;
		    ierr = it->second->selectValues(msk, buf[j]);
		    if (ierr < 0) {
			LOGGER(ibis::gVerbose > 0)
			    << "Warning -- bord::part::evaluateTerms(" << desc
			    << ") expected to retrieve " << nEvents
			    << " values for column " << j << " (" << cn1
			    << ", " << ibis::TYPESTRING[(int)ct[j]]
			    << "), but got " << ierr;
			return 0;
		    }
		    break;
		case ibis::ULONG:
		    buf[j] = new ibis::array_t<uint64_t>;
		    ierr = it->second->selectValues(msk, buf[j]);
		    if (ierr < 0) {
			LOGGER(ibis::gVerbose > 0)
			    << "Warning -- bord::part::evaluateTerms(" << desc
			    << ") expected to retrieve " << nEvents
			    << " values " << j << " (" << cn1
			    << ", " << ibis::TYPESTRING[(int)ct[j]]
			    << "), but got " << ierr;
			return 0;
		    }
		    break;
		case ibis::FLOAT:
		    buf[j] = new ibis::array_t<float>;
		    ierr = it->second->selectValues(msk, buf[j]);
		    if (ierr < 0) {
			LOGGER(ibis::gVerbose > 0)
			    << "Warning -- bord::part::evaluateTerms(" << desc
			    << ") expected to retrieve " << nEvents
			    << " values for column " << j << " (" << cn1
			    << ", " << ibis::TYPESTRING[(int)ct[j]]
			    << "), but got " << ierr;
			return 0;
		    }
		    break;
		case ibis::DOUBLE:
		    buf[j] = new ibis::array_t<double>;
		    ierr = it->second->selectValues(msk, buf[j]);
		    if (ierr < 0) {
			LOGGER(ibis::gVerbose > 0)
			    << "Warning -- bord::part::evaluateTerms(" << desc
			    << ") expected to retrieve " << nEvents
			    << " values " << j << " (" << cn1
			    << ", " << ibis::TYPESTRING[(int)ct[j]]
			    << "), but got " << ierr;
			return 0;
		    }
		    break;
		case ibis::TEXT:
		case ibis::CATEGORY:
		    buf[j] = it->second->selectStrings(msk);
		    if (buf[j] == 0 || static_cast<std::vector<std::string>*>
			(buf[j])->size() != nEvents) {
			LOGGER(ibis::gVerbose > 0)
			    << "Warning -- bord::part::evaluateTerms(" << desc
			    << ") expected to retrieve " << nEvents
			    << " values " << j << " (" << cn1
			    << ", " << ibis::TYPESTRING[(int)ct[j]]
			    << "), but got "
			    << (buf[j] != 0 ?
				static_cast<std::vector<std::string>*>
				(buf[j])->size() : 0U);
			return 0;
		    }
		    break;
		}
	    }
	    else {
		LOGGER(ibis::gVerbose > 0)
		    << "Warning -- bord::part::evaluateTerms(" << desc
		    << ") failed to find a column " << j << " named " << cn1;
		return 0;
	    }
	    break;}
	} // switch (t->termType())

	cd[j] = cdesc[j].c_str();
    } // for (uint32_t j...

    return new
	ibis::bord(tn.c_str(), desc, (uint64_t)nEvents, buf, ct, cn, &cd);
} // ibis::bord::part::evaluateTerms

ibis::bord::column::column(const ibis::bord::part *tbl, ibis::TYPE_T t,
			   const char *cn, void *st,
			   const char *de, double lo, double hi)
    : ibis::column(tbl, t, cn, de, lo, hi), buffer(st) {
} // ibis::bord::column::column

///@note Transfer the ownership of @c st to the new @c column object.
ibis::bord::column::column(const ibis::bord::part *tbl,
			   const ibis::column& old, void *st)
    : ibis::column(tbl, old.type(), old.name(), old.description(),
		   old.lowerBound(), old.upperBound()),
      buffer(st) {
} // ibis::bord::column::column

ibis::bord::column::column(const ibis::bord::column &c)
    : ibis::column(c.thePart, c.m_type, c.m_name.c_str(), c.m_desc.c_str(),
		   c.lower, c.upper) {
    switch (c.m_type) {
    case ibis::BYTE: {
	buffer = new array_t<signed char>
	    (* static_cast<array_t<signed char>*>(c.buffer));
	break;}
    case ibis::UBYTE: {
	buffer = new array_t<unsigned char>
	    (* static_cast<array_t<unsigned char>*>(c.buffer));
	break;}
    case ibis::SHORT: {
	buffer = new array_t<int16_t>
	    (* static_cast<array_t<int16_t>*>(c.buffer));
	break;}
    case ibis::USHORT: {
	buffer = new array_t<uint16_t>
	    (* static_cast<array_t<uint16_t>*>(c.buffer));
	break;}
    case ibis::INT: {
	buffer = new array_t<int32_t>
	    (* static_cast<array_t<int32_t>*>(c.buffer));
	break;}
    case ibis::UINT: {
	buffer = new array_t<uint32_t>
	    (* static_cast<array_t<uint32_t>*>(c.buffer));
	break;}
    case ibis::LONG: {
	buffer = new array_t<int64_t>
	    (* static_cast<array_t<int64_t>*>(c.buffer));
	break;}
    case ibis::ULONG: {
	buffer = new array_t<uint64_t>
	    (* static_cast<array_t<uint64_t>*>(c.buffer));
	break;}
    case ibis::FLOAT: {
	buffer = new array_t<float>
	    (* static_cast<array_t<float>*>(c.buffer));
	break;}
    case ibis::DOUBLE: {
	buffer = new array_t<double>
	    (* static_cast<array_t<double>*>(c.buffer));
	break;}
    default: {
	buffer = 0;
	ibis::util::logMessage
	    ("bord::column::column",
	     "copy constructor can not handle column (%s) with type %s",
	     c.name(), ibis::TYPESTRING[(int)c.type()]);
	break;}
    }
    m_sorted = c.m_sorted;
} // ibis::bord::column::column

ibis::bord::column::~column() {
    switch (m_type) {
    case ibis::BYTE: {
	delete static_cast<array_t<signed char>*>(buffer);
	break;}
    case ibis::UBYTE: {
	delete static_cast<array_t<unsigned char>*>(buffer);
	break;}
    case ibis::SHORT: {
	delete static_cast<array_t<int16_t>*>(buffer);
	break;}
    case ibis::USHORT: {
	delete static_cast<array_t<uint16_t>*>(buffer);
	break;}
    case ibis::INT: {
	delete static_cast<array_t<int32_t>*>(buffer);
	break;}
    case ibis::UINT: {
	delete static_cast<array_t<uint32_t>*>(buffer);
	break;}
    case ibis::LONG: {
	delete static_cast<array_t<int64_t>*>(buffer);
	break;}
    case ibis::ULONG: {
	delete static_cast<array_t<uint64_t>*>(buffer);
	break;}
    case ibis::FLOAT: {
	delete static_cast<array_t<float>*>(buffer);
	break;}
    case ibis::DOUBLE: {
	delete static_cast<array_t<double>*>(buffer);
	break;}
    case ibis::TEXT:
    case ibis::CATEGORY: {
	delete static_cast<std::vector<std::string>*>(buffer);
	break;}
    default: {
	break;}
    }
} // ibis::bord::column::~column

/// Retrieve the raw data buffer as an ibis::fileManager::storage.  Since
/// this function exposes the internal storage representation, it should
/// not be relied upon for general uses.  This is mostly a convenience
/// thing for FastBit internal development!
///
/// @note Only fix-sized columns are stored using
/// ibis::fileManager::storage objects.  It will return a nil pointer for
/// string-valued columns.
inline ibis::fileManager::storage*
ibis::bord::column::getRawData() const {
    ibis::fileManager::storage *str = 0;
    switch (m_type) {
    case ibis::BYTE: {
	str = static_cast<array_t<signed char>*>(buffer)->getStorage();
	break;}
    case ibis::UBYTE: {
	str = static_cast<array_t<unsigned char>*>(buffer)->getStorage();
	break;}
    case ibis::SHORT: {
	str = static_cast<array_t<int16_t>*>(buffer)->getStorage();
	break;}
    case ibis::USHORT: {
	str = static_cast<array_t<uint16_t>*>(buffer)->getStorage();
	break;}
    case ibis::INT: {
	str = static_cast<array_t<int32_t>*>(buffer)->getStorage();
	break;}
    case ibis::UINT: {
	str = static_cast<array_t<uint32_t>*>(buffer)->getStorage();
	break;}
    case ibis::LONG: {
	str = static_cast<array_t<int64_t>*>(buffer)->getStorage();
	break;}
    case ibis::ULONG: {
	str = static_cast<array_t<uint64_t>*>(buffer)->getStorage();
	break;}
    case ibis::FLOAT: {
	str = static_cast<array_t<float>*>(buffer)->getStorage();
	break;}
    case ibis::DOUBLE: {
	str = static_cast<array_t<double>*>(buffer)->getStorage();
	break;}
    default: {
	break;}
    }

    return str;
} // ibis::bord::column::getRawData

void ibis::bord::column::computeMinMax(const char *,
				       double &min, double &max) const {
    if (buffer == 0) return;

    switch (m_type) {
    case ibis::UBYTE: {
	const array_t<unsigned char> &val =
	    * static_cast<const array_t<unsigned char>*>(buffer);

	unsigned char imin = val[0];
	unsigned char imax = val[0];
	long unsigned nelm = val.size();
	for (uint32_t i = 1; i < nelm; ++i) {
	    if (imin > val[i])
		imin = val[i];
	    else if (imax < val[i])
		imax = val[i];
	}
	if (ibis::gVerbose > 5)
	    logMessage("computeMinMax", "nelm = %lu, min = %u, max = %u",
		       nelm, static_cast<unsigned>(imin),
		       static_cast<unsigned>(imax));
	min = imin;
	max = imax;
	break;}
    case ibis::BYTE: {
	const array_t<signed char> &val =
	    * static_cast<const array_t<signed char>*>(buffer);

	signed char imin = val[0];
	signed char imax = val[0];
	uint32_t nelm = val.size();
	for (uint32_t i = 1; i < nelm; ++i) {
	    if (imin > val[i])
		imin = val[i];
	    else if (imax < val[i])
		imax = val[i];
	}
	if (ibis::gVerbose > 5)
	    logMessage("computeMinMax", "nelm = %lu, min = %d, max = %d",
		       nelm, static_cast<int>(imin), static_cast<int>(imax));
	min = imin;
	max = imax;
	break;}
    case ibis::USHORT: {
	const array_t<uint16_t> &val = 
	    * static_cast<const array_t<uint16_t>*>(buffer);

	uint16_t imin = val[0];
	uint16_t imax = val[0];
	uint32_t nelm = val.size();
	for (uint32_t i = 1; i < nelm; ++i) {
	    if (imin > val[i])
		imin = val[i];
	    else if (imax < val[i])
		imax = val[i];
	}
	if (ibis::gVerbose > 5)
	    logMessage("computeMinMax", "nelm = %lu, min = %u, max = %u",
		       nelm, static_cast<unsigned>(imin),
		       static_cast<unsigned>(imax));
	min = imin;
	max = imax;
	break;}
    case ibis::SHORT: {
	const array_t<int16_t> &val =
	    * static_cast<const array_t<int16_t>*>(buffer);

	int16_t imin = val[0];
	int16_t imax = val[0];
	uint32_t nelm = val.size();
	for (uint32_t i = 1; i < nelm; ++i) {
	    if (imin > val[i])
		imin = val[i];
	    else if (imax < val[i])
		imax = val[i];
	}
	if (ibis::gVerbose > 5)
	    logMessage("computeMinMax", "nelm = %lu, min = %d, max = %d",
		       nelm, static_cast<int>(imin), static_cast<int>(imax));
	min = imin;
	max = imax;
	break;}
    case ibis::UINT: {
	const array_t<uint32_t> &val =
	    * static_cast<const array_t<uint32_t>*>(buffer);

	uint32_t imin = val[0];
	uint32_t imax = val[0];
	uint32_t nelm = val.size();
	for (uint32_t i = 1; i < nelm; ++i) {
	    if (imin > val[i])
		imin = val[i];
	    else if (imax < val[i])
		imax = val[i];
	}
	if (ibis::gVerbose > 5)
	    logMessage("computeMinMax", "nelm = %lu, min = %lu, max = %lu",
		       nelm, static_cast<long unsigned>(imin),
		       static_cast<long unsigned>(imax));
	min = imin;
	max = imax;
	break;}
    case ibis::INT: {
	const array_t<int32_t> &val =
	    *static_cast<const array_t<int32_t>*>(buffer);

	int32_t imin = val[0];
	int32_t imax = val[0];
	uint32_t nelm = val.size();
	for (uint32_t i = 1; i < nelm; ++i) {
	    if (imin > val[i])
		imin = val[i];
	    else if (imax < val[i])
		imax = val[i];
	}
	if (ibis::gVerbose > 5)
	    logMessage("computeMinMax", "nelm = %lu, min = %ld, max = %ld",
		       nelm, static_cast<long>(imin), static_cast<long>(imax));
	min = imin;
	max = imax;
	break;}
    case ibis::ULONG: {
	const array_t<uint64_t> &val =
	    * static_cast<const array_t<uint64_t>*>(buffer);

	uint64_t imin = val[0];
	uint64_t imax = val[0];
	uint32_t nelm = val.size();
	for (uint32_t i = 1; i < nelm; ++i) {
	    if (imin > val[i])
		imin = val[i];
	    else if (imax < val[i])
		imax = val[i];
	}
	if (ibis::gVerbose > 5)
	    logMessage("computeMinMax", "nelm = %lu, min = %llu, max = %llu",
		       nelm, static_cast<long long unsigned>(imin),
		       static_cast<long long unsigned>(imax));
	min = static_cast<double>(imin);
	max = static_cast<double>(imax);
	break;}
    case ibis::LONG: {
	const array_t<int64_t> &val =
	    *static_cast<const array_t<int64_t>*>(buffer);

	int64_t imin = val[0];
	int64_t imax = val[0];
	uint32_t nelm = val.size();
	for (uint32_t i = 1; i < nelm; ++i) {
	    if (imin > val[i])
		imin = val[i];
	    else if (imax < val[i])
		imax = val[i];
	}
	if (ibis::gVerbose > 5)
	    logMessage("computeMinMax", "nelm = %lu, min = %lld, max = %lld",
		       nelm, static_cast<long long>(imin),
		       static_cast<long long>(imax));
	min = static_cast<double>(imin);
	max = static_cast<double>(imax);
	break;}
    case ibis::FLOAT: {
	const array_t<float> &val =
	    * static_cast<const array_t<float>*>(buffer);

	float imin = val[0];
	float imax = val[0];
	uint32_t nelm = val.size();
	for (uint32_t i = 1; i < nelm; ++i) {
	    if (imin > val[i])
		imin = val[i];
	    else if (imax < val[i])
		imax = val[i];
	}
	if (ibis::gVerbose > 5)
	    logMessage("computeMinMax", "nelm = %lu, min = %g, max = %g",
		       nelm, imin, imax);
	min = imin;
	max = imax;
	break;}
    case ibis::DOUBLE: {
	const array_t<double> &val =
	    * static_cast<const array_t<double>*>(buffer);

	min = val[0];
	max = val[0];
	uint32_t nelm = val.size();
	for (uint32_t i = 1; i < nelm; ++i) {
	    if (min > val[i])
		min = val[i];
	    else if (max < val[i])
		max = val[i];
	}
	if (ibis::gVerbose > 5)
	    logMessage("computeMinMax", "nelm = %lu, min = %lg, max = %lg",
		       nelm, min, max);
	break;}
    default:
	if (ibis::gVerbose > 2)
	    logMessage("computeMinMax", "column type %s is not one of the "
		       "supported types (int, uint, float, double)",
		       TYPESTRING[static_cast<int>(m_type)]);
	min = 0;
	max = (thePart != 0) ? thePart->nRows() : -DBL_MAX;
    } // switch(m_type)
} // ibis::bord::column::computeMinMax

long ibis::bord::column::evaluateRange(const ibis::qContinuousRange& cmp,
				       const ibis::bitvector& mask,
				       ibis::bitvector& res) const {
    long ierr = -1;

    switch (m_type) {
    case ibis::UBYTE: {
	const array_t<unsigned char> &val =
	    * static_cast<const array_t<unsigned char>*>(buffer);
	ierr = ibis::part::doScan(val, cmp, mask, res);
	break;}
    case ibis::BYTE: {
	const array_t<signed char> &val =
	    * static_cast<const array_t<signed char>*>(buffer);
	ierr = ibis::part::doScan(val, cmp, mask, res);
	break;}
    case ibis::USHORT: {
	const array_t<uint16_t> &val =
	    * static_cast<const array_t<uint16_t>*>(buffer);
	ierr = ibis::part::doScan(val, cmp, mask, res);
	break;}
    case ibis::SHORT: {
	const array_t<int16_t> &val =
	    * static_cast<const array_t<int16_t>*>(buffer);
	ierr = ibis::part::doScan(val, cmp, mask, res);
	break;}
    case ibis::UINT: {
	const array_t<uint32_t> &val =
	    * static_cast<const array_t<uint32_t>*>(buffer);
	ierr = ibis::part::doScan(val, cmp, mask, res);
	break;}
    case ibis::INT: {
	const array_t<int32_t> &val =
	    * static_cast<const array_t<int32_t>*>(buffer);
	ierr = ibis::part::doScan(val, cmp, mask, res);
	break;}
    case ibis::ULONG: {
	const array_t<uint64_t> &val =
	    * static_cast<const array_t<uint64_t>*>(buffer);
	ierr = ibis::part::doScan(val, cmp, mask, res);
	break;}
    case ibis::LONG: {
	const array_t<int64_t> &val =
	    * static_cast<const array_t<int64_t>*>(buffer);
	ierr = ibis::part::doScan(val, cmp, mask, res);
	break;}
    case ibis::FLOAT: {
	const array_t<float> &val =
	    * static_cast<const array_t<float>*>(buffer);
	ierr = ibis::part::doScan(val, cmp, mask, res);
	break;}
    case ibis::DOUBLE: {
	const array_t<double> &val =
	    * static_cast<const array_t<double>*>(buffer);
	ierr = ibis::part::doScan(val, cmp, mask, res);
	break;}
    default:
	LOGGER(ibis::gVerbose > 2)
	    << "Warning -- column[" << (thePart ? thePart->name() : "?")
	    << '.' << m_name << "]::evaluateRange deos not support column type "
	    << TYPESTRING[static_cast<int>(m_type)]
	    << ", only supports integers and floats";
	ierr = -2;
    } // switch(m_type)
    return ierr;
} // ibis::bord::column::evaluateRange

long ibis::bord::column::evaluateRange(const ibis::qDiscreteRange& cmp,
				       const ibis::bitvector& mask,
				       ibis::bitvector& res) const {
    long ierr = -1;

    switch (m_type) {
    case ibis::UBYTE: {
	const array_t<unsigned char> &val =
	    * static_cast<const array_t<unsigned char>*>(buffer);
	ierr = ibis::part::doScan(val, cmp, mask, res);
	break;}
    case ibis::BYTE: {
	const array_t<signed char> &val =
	    * static_cast<const array_t<signed char>*>(buffer);
	ierr = ibis::part::doScan(val, cmp, mask, res);
	break;}
    case ibis::USHORT: {
	const array_t<uint16_t> &val =
	    * static_cast<const array_t<uint16_t>*>(buffer);
	ierr = ibis::part::doScan(val, cmp, mask, res);
	break;}
    case ibis::SHORT: {
	const array_t<int16_t> &val =
	    * static_cast<const array_t<int16_t>*>(buffer);
	ierr = ibis::part::doScan(val, cmp, mask, res);
	break;}
    case ibis::UINT: {
	const array_t<uint32_t> &val =
	    * static_cast<const array_t<uint32_t>*>(buffer);
	ierr = ibis::part::doScan(val, cmp, mask, res);
	break;}
    case ibis::INT: {
	const array_t<int32_t> &val =
	    * static_cast<const array_t<int32_t>*>(buffer);
	ierr = ibis::part::doScan(val, cmp, mask, res);
	break;}
    case ibis::ULONG: {
	const array_t<uint64_t> &val =
	    * static_cast<const array_t<uint64_t>*>(buffer);
	ierr = ibis::part::doScan(val, cmp, mask, res);
	break;}
    case ibis::LONG: {
	const array_t<int64_t> &val =
	    * static_cast<const array_t<int64_t>*>(buffer);
	ierr = ibis::part::doScan(val, cmp, mask, res);
	break;}
    case ibis::FLOAT: {
	const array_t<float> &val =
	    * static_cast<const array_t<float>*>(buffer);
	ierr = ibis::part::doScan(val, cmp, mask, res);
	break;}
    case ibis::DOUBLE: {
	const array_t<double> &val =
	    * static_cast<const array_t<double>*>(buffer);
	ierr = ibis::part::doScan(val, cmp, mask, res);
	break;}
    default:
	LOGGER(ibis::gVerbose > 2)
	    << "Warning -- column[" << (thePart ? thePart->name() : "?")
	    << '.' << m_name << "]::evaluateRange deos not support column type "
	    << TYPESTRING[static_cast<int>(m_type)]
	    << ", only supports integers and floats";
	ierr = -2;
    } // switch(m_type)
    return ierr;
} // ibis::bord::column::evaluateRange

/// Locate the strings that match the given string.  The comaprison is case
/// sensitive.  If the incoming strign is a nil pointer, it matches nothing.
long ibis::bord::column::stringSearch(const char* str,
				      ibis::bitvector& hits) const {
    std::string evt = "column[";
    evt += (thePart ? thePart->name() : "");
    evt += '.';
    evt += m_name;
    evt += "]::stringSearch(";
    evt += (str ? str : "");
    evt += ')';
    if (m_type != ibis::TEXT && m_type != ibis::CATEGORY) {
	LOGGER(ibis::gVerbose > 0)
	    << "Warning -- " << evt << " is not supported on column type "
	    << ibis::TYPESTRING[(int)m_type];
	return -1;
    }
    if (buffer == 0) {
	LOGGER(ibis::gVerbose > 0)
	    << "Warning -- " << evt << " can not proceed with a nil buffer";
	return -2;
    }

    const std::vector<std::string>&
	vals(*static_cast<const std::vector<std::string>*>(buffer));
    if (str == 0) { // null string can not match any thing
	hits.set(0, thePart ? thePart->nRows() : vals.size());
	return 0;
    }

    ibis::util::timer mytimer(evt.c_str(), 3);
    hits.clear();
    for (size_t j = 0; j < vals.size(); ++ j) {
	if (vals[j].compare(str) == 0)
	    hits.setBit(j, 1);
    }
    hits.adjustSize(0, thePart ? thePart->nRows() : vals.size());
    return hits.cnt();
} // ibis::bord::column::stringSearch

long ibis::bord::column::stringSearch(const std::vector<std::string>& str,
				      ibis::bitvector& hits) const {
    std::string evt = "column[";
    evt += (thePart ? thePart->name() : "");
    evt += '.';
    evt += m_name;
    evt += "]::stringSearch(<...>)";
    if (m_type != ibis::TEXT && m_type != ibis::CATEGORY) {
	LOGGER(ibis::gVerbose > 0)
	    << "Warning -- " << evt << " is not supported on column type "
	    << ibis::TYPESTRING[(int)m_type];
	return -1;
    }
    if (buffer == 0) {
	LOGGER(ibis::gVerbose > 0)
	    << "Warning -- " << evt << " can not proceed with a nil buffer";
	return -2;
    }
    const std::vector<std::string>&
	vals(*static_cast<const std::vector<std::string>*>(buffer));
    if (str.empty()) { // null string can not match any thing
	hits.set(0, thePart ? thePart->nRows() : vals.size());
	return 0;
    }

    ibis::util::timer mytimer(evt.c_str(), 3);
    hits.clear();
    for (size_t j = 0; j < str.size(); ++ j) {
	bool hit = false;
	for (size_t i = 0; i < str.size() && hit == false; ++ i) {
	    hit = (0 == vals[j].compare(str[i]));
	}
	if (hit) {
	    hits.setBit(j, 1);
	}
    }
    hits.adjustSize(0, thePart ? thePart->nRows() : vals.size());
    return hits.cnt();
} // ibis::bord::column::stringSearch

/// Compute an estimate of the maximum number of possible matches.  This is
/// a trivial implementation that does not actually perform any meaningful
/// checks.  It simply returns the number of strings in memory as the
/// estimate.
long ibis::bord::column::stringSearch(const char* str) const {
    if (m_type != ibis::TEXT && m_type != ibis::CATEGORY) {
	LOGGER(ibis::gVerbose > 0)
	    << "Warning -- column[" << (thePart ? thePart->name() : "") << '.'
	    << m_name << "]::stringSearch is not supported on column type "
	    << ibis::TYPESTRING[(int)m_type];
	return -1;
    }
    if (buffer == 0) {
	LOGGER(ibis::gVerbose > 0)
	    << "Warning -- column[" << (thePart ? thePart->name() : "") << '.'
	    << m_name << "]::stringSearch can not proceed with a nil buffer";
	return -2;
    }
    if (str == 0) return 0;

    const std::vector<std::string>&
	vals(*static_cast<const std::vector<std::string>*>(buffer));
    return vals.size();
} // ibis::bord::column::stringSearch

/// Compute an estimate of the maximum number of possible matches.  This is
/// a trivial implementation that does not actually perform any meaningful
/// checks.  It simply returns the number of strings in memory as the
/// estimate.
long
ibis::bord::column::stringSearch(const std::vector<std::string>& str) const {
    if (m_type != ibis::TEXT && m_type != ibis::CATEGORY) {
	LOGGER(ibis::gVerbose > 0)
	    << "Warning -- column[" << (thePart ? thePart->name() : "") << '.'
	    << m_name << "]::stringSearch is not supported on column type "
	    << ibis::TYPESTRING[(int)m_type];
	return -1;
    }
    if (buffer == 0) {
	LOGGER(ibis::gVerbose > 0)
	    << "Warning -- column[" << (thePart ? thePart->name() : "") << '.'
	    << m_name << "]::stringSearch can not proceed with a nil buffer";
	return -2;
    }
    if (str.empty()) return 0;

    const std::vector<std::string>&
	vals(*static_cast<const std::vector<std::string>*>(buffer));
    return vals.size();
} // ibis::bord::column::stringSearch

long ibis::bord::column::keywordSearch(const char*, ibis::bitvector&) const {
    LOGGER(ibis::gVerbose > 0)
	<< "Warning -- column[" << (thePart ? thePart->name() : "") << '.'
	<< m_name << "]::keywordSearch is not supported on column type "
	<< ibis::TYPESTRING[(int)m_type];
    return -1;
} // ibis::bord::column::keywordSearch

long ibis::bord::column::keywordSearch(const char*) const {
    LOGGER(ibis::gVerbose > 0)
	<< "Warning -- column[" << (thePart ? thePart->name() : "") << '.'
	<< m_name << "]::keywordSearch is not supported on column type "
	<< ibis::TYPESTRING[(int)m_type];
    return -1;
} // ibis::bord::column::keywordSearch

/// Compute an estimate of the maximum number of possible matches.  This is
/// a trivial implementation that does not actually perform any meaningful
/// checks.  It simply returns the number of strings in memory as the
/// estimate.
long ibis::bord::column::patternSearch(const char* pat) const {
    if (m_type != ibis::TEXT && m_type != ibis::CATEGORY) {
	LOGGER(ibis::gVerbose > 0)
	    << "Warning -- column[" << (thePart ? thePart->name() : "") << '.'
	    << m_name << "]::patternSearch is not supported on column type "
	    << ibis::TYPESTRING[(int)m_type];
	return -1;
    }
    if (buffer == 0) {
	LOGGER(ibis::gVerbose > 0)
	    << "Warning -- column[" << (thePart ? thePart->name() : "") << '.'
	    << m_name << "]::stringSearch can not proceed with a nil buffer";
	return -2;
    }
    if (pat == 0) return 0;

    const std::vector<std::string>&
	vals(*static_cast<const std::vector<std::string>*>(buffer));
    return vals.size();
} // ibis::bord::column::patternSearch

long ibis::bord::column::patternSearch(const char* pat,
				       ibis::bitvector &hits) const {
    std::string evt = "column[";
    evt += (thePart ? thePart->name() : "");
    evt += '.';
    evt += m_name;
    evt += "]::patternSearch(";
    evt += (pat ? pat : "");
    evt += ')';
    if (m_type != ibis::TEXT && m_type != ibis::CATEGORY) {
	LOGGER(ibis::gVerbose > 0)
	    << "Warning -- " << evt << " is not supported on column type "
	    << ibis::TYPESTRING[(int)m_type];
	return -1;
    }
    if (buffer == 0) {
	LOGGER(ibis::gVerbose > 0)
	    << "Warning -- " << evt << " can not proceed with a nil buffer";
	return -2;
    }

    const std::vector<std::string>&
	vals(*static_cast<const std::vector<std::string>*>(buffer));
    if (pat == 0) { // null string can not match any thing
	hits.set(0, thePart ? thePart->nRows() : vals.size());
	return 0;
    }

    ibis::util::timer mytimer(evt.c_str(), 3);
    hits.clear();
    for (size_t j = 0; j < vals.size(); ++ j) {
	if (ibis::util::strMatch(vals[j].c_str(), pat))
	    hits.setBit(j, 1);
    }

    hits.adjustSize(0, thePart ? thePart->nRows() : vals.size());
    return hits.cnt();
} // ibis::bord::column::patternSearch

ibis::array_t<signed char>*
ibis::bord::column::selectBytes(const ibis::bitvector &mask) const {
    ibis::array_t<signed char>* array = new array_t<signed char>;
    const uint32_t tot = mask.cnt();
    if (tot == 0)
	return array;

    ibis::horometer timer;
    if (ibis::gVerbose > 5)
	timer.start();
    if (m_type == ibis::BYTE) {
	const array_t<signed char> &prop =
	    * static_cast<const array_t<signed char>*>(buffer);
	uint32_t i = 0;
	array->resize(tot);
	const uint32_t nprop = prop.size();
	ibis::bitvector::indexSet index = mask.firstIndexSet();
	if (nprop >= mask.size()) { // no need to check loop bounds
	    while (index.nIndices() > 0) {
		const ibis::bitvector::word_t *idx0 = index.indices();
		if (index.isRange()) {
		    for (uint32_t j = *idx0; j < idx0[1]; ++j, ++i) {
			(*array)[i] = (prop[j]);
		    }
		}
		else {
		    for (uint32_t j = 0; j<index.nIndices(); ++j, ++i) {
			(*array)[i] = (prop[idx0[j]]);
		    }
		}
		++ index;
	    }
	}
	else { // need to check loop bounds against nprop
	    while (index.nIndices() > 0) {
		const ibis::bitvector::word_t *idx0 = index.indices();
		if (*idx0 >= nprop) break;
		if (index.isRange()) {
		    for (uint32_t j = *idx0;
			 j < (idx0[1]<=nprop ? idx0[1] : nprop);
			 ++j, ++i) {
			(*array)[i] = (prop[j]);
		    }
		}
		else {
		    for (uint32_t j = 0; j<index.nIndices(); ++j, ++i) {
			if (idx0[j] < nprop)
			    (*array)[i] = (prop[idx0[j]]);
			else
			    break;
		    }
		}
		++ index;
	    }
	}
	if (i != tot) {
	    array->resize(i);
	    logWarning("selectBytes", "expects to retrieve %lu elements "
		       "but only got %lu", static_cast<long unsigned>(tot),
		       static_cast<long unsigned>(i));
	}
    }
    else {
	logWarning("selectBytes", "incompatible data type");
    }
    if (ibis::gVerbose > 5) {
	timer.stop();
	long unsigned cnt = mask.cnt();
	logMessage("selectBytes", "retrieving %lu integer%s "
		   "took %g sec(CPU), %g sec(elapsed)",
		   static_cast<long unsigned>(cnt), (cnt > 1 ? "s" : ""),
		   timer.CPUTime(), timer.realTime());
    }
    return array;
} // ibis::bord::column::selectBytes

ibis::array_t<unsigned char>*
ibis::bord::column::selectUBytes(const ibis::bitvector& mask) const {
    array_t<unsigned char>* array = new array_t<unsigned char>;
    const uint32_t tot = mask.cnt();
    if (tot == 0)
	return array;
    ibis::horometer timer;
    if (ibis::gVerbose > 5)
	timer.start();

    if (m_type == UBYTE) {
	const array_t<unsigned char> &prop =
	    * static_cast<const array_t<unsigned char>*>(buffer);

	uint32_t i = 0;
	array->resize(tot);
	const uint32_t nprop = prop.size();
	ibis::bitvector::indexSet index = mask.firstIndexSet();
	if (nprop >= mask.size()) {
	    while (index.nIndices() > 0) {
		const ibis::bitvector::word_t *idx0 = index.indices();
		if (index.isRange()) {
		    for (uint32_t j = *idx0; j<idx0[1]; ++j, ++i) {
			(*array)[i] = (prop[j]);
		    }
		}
		else {
		    for (uint32_t j = 0; j<index.nIndices(); ++j, ++i) {
			(*array)[i] = (prop[idx0[j]]);
		    }
		}
		++ index;
	    }
	}
	else { // need to check loop bounds against nprop
	    while (index.nIndices() > 0) {
		const ibis::bitvector::word_t *idx0 = index.indices();
		if (*idx0 >= nprop) break;
		if (index.isRange()) {
		    for (uint32_t j = *idx0;
			 j < (idx0[1]<=nprop ? idx0[1] : nprop);
			 ++j, ++i) {
			(*array)[i] = (prop[j]);
		    }
		}
		else {
		    for (uint32_t j = 0; j<index.nIndices(); ++j, ++i) {
			if (idx0[j] < nprop)
			    (*array)[i] = (prop[idx0[j]]);
			else
			    break;
		    }
		}
		++ index;
	    }
	}

	if (i != tot) {
	    array->resize(i);
	    logWarning("selectUBytes", "expects to retrieve %lu elements "
		       "but only got %lu", static_cast<long unsigned>(tot),
		       static_cast<long unsigned>(i));
	}
    }
    else {
	logWarning("selectUBytes", "incompatible data type");
    }
    if (ibis::gVerbose > 5) {
	timer.stop();
	long unsigned cnt = mask.cnt();
	logMessage("selectUBytes", "retrieving %lu unsigned integer%s "
		   "took %g sec(CPU), %g sec(elapsed)",
		   static_cast<long unsigned>(cnt), (cnt > 1 ? "s" : ""),
		   timer.CPUTime(), timer.realTime());
    }
    return array;
} // ibis::bord::column::selectUBytes

ibis::array_t<int16_t>*
ibis::bord::column::selectShorts(const ibis::bitvector &mask) const {
    ibis::array_t<int16_t>* array = new array_t<int16_t>;
    const uint32_t tot = mask.cnt();
    if (tot == 0)
	return array;

    ibis::horometer timer;
    if (ibis::gVerbose > 5)
	timer.start();
    if (m_type == ibis::SHORT) {
	const array_t<int16_t> &prop =
	    * static_cast<const array_t<int16_t>*>(buffer);
	uint32_t i = 0;
	array->resize(tot);
	const uint32_t nprop = prop.size();
	ibis::bitvector::indexSet index = mask.firstIndexSet();
	if (nprop >= mask.size()) { // no need to check loop bounds
	    while (index.nIndices() > 0) {
		const ibis::bitvector::word_t *idx0 = index.indices();
		if (index.isRange()) {
		    for (uint32_t j = *idx0; j < idx0[1]; ++j, ++i) {
			(*array)[i] = (prop[j]);
		    }
		}
		else {
		    for (uint32_t j = 0; j<index.nIndices(); ++j, ++i) {
			(*array)[i] = (prop[idx0[j]]);
		    }
		}
		++ index;
	    }
	}
	else { // need to check loop bounds against nprop
	    while (index.nIndices() > 0) {
		const ibis::bitvector::word_t *idx0 = index.indices();
		if (*idx0 >= nprop) break;
		if (index.isRange()) {
		    for (uint32_t j = *idx0;
			 j < (idx0[1]<=nprop ? idx0[1] : nprop);
			 ++j, ++i) {
			(*array)[i] = (prop[j]);
		    }
		}
		else {
		    for (uint32_t j = 0; j<index.nIndices(); ++j, ++i) {
			if (idx0[j] < nprop)
			    (*array)[i] = (prop[idx0[j]]);
			else
			    break;
		    }
		}
		++ index;
	    }
	}
	if (i != tot) {
	    array->resize(i);
	    logWarning("selectShorts", "expects to retrieve %lu elements "
		       "but only got %lu", static_cast<long unsigned>(tot),
		       static_cast<long unsigned>(i));
	}
    }
    else if (m_type == ibis::BYTE) {
	const array_t<signed char> &prop =
	    * static_cast<const array_t<signed char>*>(buffer);
	uint32_t i = 0;
	array->resize(tot);
	const uint32_t nprop = prop.size();
	ibis::bitvector::indexSet index = mask.firstIndexSet();
	if (nprop >= mask.size()) { // no need to check loop bounds
	    while (index.nIndices() > 0) {
		const ibis::bitvector::word_t *idx0 = index.indices();
		if (index.isRange()) {
		    for (uint32_t j = *idx0; j < idx0[1]; ++j, ++i) {
			(*array)[i] = (prop[j]);
		    }
		}
		else {
		    for (uint32_t j = 0; j<index.nIndices(); ++j, ++i) {
			(*array)[i] = (prop[idx0[j]]);
		    }
		}
		++ index;
	    }
	}
	else { // need to check loop bounds against nprop
	    while (index.nIndices() > 0) {
		const ibis::bitvector::word_t *idx0 = index.indices();
		if (*idx0 >= nprop) break;
		if (index.isRange()) {
		    for (uint32_t j = *idx0;
			 j < (idx0[1]<=nprop ? idx0[1] : nprop);
			 ++j, ++i) {
			(*array)[i] = (prop[j]);
		    }
		}
		else {
		    for (uint32_t j = 0; j<index.nIndices(); ++j, ++i) {
			if (idx0[j] < nprop)
			    (*array)[i] = (prop[idx0[j]]);
			else
			    break;
		    }
		}
		++ index;
	    }
	}
	if (i != tot) {
	    array->resize(i);
	    logWarning("selectShorts", "expects to retrieve %lu elements "
		       "but only got %lu", static_cast<long unsigned>(tot),
		       static_cast<long unsigned>(i));
	}
    }
    else if (m_type == ibis::UBYTE) {
	const array_t<unsigned char> &prop =
	    * static_cast<const array_t<unsigned char>*>(buffer);
	uint32_t i = 0;
	array->resize(tot);
	const uint32_t nprop = prop.size();
	ibis::bitvector::indexSet index = mask.firstIndexSet();
	if (nprop >= mask.size()) { // no need to check loop bounds
	    while (index.nIndices() > 0) {
		const ibis::bitvector::word_t *idx0 = index.indices();
		if (index.isRange()) {
		    for (uint32_t j = *idx0; j < idx0[1]; ++j, ++i) {
			(*array)[i] = (prop[j]);
		    }
		}
		else {
		    for (uint32_t j = 0; j<index.nIndices(); ++j, ++i) {
			(*array)[i] = (prop[idx0[j]]);
		    }
		}
		++ index;
	    }
	}
	else { // need to check loop bounds against nprop
	    while (index.nIndices() > 0) {
		const ibis::bitvector::word_t *idx0 = index.indices();
		if (*idx0 >= nprop) break;
		if (index.isRange()) {
		    for (uint32_t j = *idx0;
			 j < (idx0[1]<=nprop ? idx0[1] : nprop);
			 ++j, ++i) {
			(*array)[i] = (prop[j]);
		    }
		}
		else {
		    for (uint32_t j = 0; j<index.nIndices(); ++j, ++i) {
			if (idx0[j] < nprop)
			    (*array)[i] = (prop[idx0[j]]);
			else
			    break;
		    }
		}
		++ index;
	    }
	}
	if (i != tot) {
	    array->resize(i);
	    logWarning("selectShorts", "expects to retrieve %lu elements "
		       "but only got %lu", static_cast<long unsigned>(tot),
		       static_cast<long unsigned>(i));
	}
    }
    else {
	logWarning("selectShorts", "incompatible data type");
    }
    if (ibis::gVerbose > 5) {
	timer.stop();
	long unsigned cnt = mask.cnt();
	logMessage("selectShorts", "retrieving %lu integer%s "
		   "took %g sec(CPU), %g sec(elapsed)",
		   static_cast<long unsigned>(cnt), (cnt > 1 ? "s" : ""),
		   timer.CPUTime(), timer.realTime());
    }
    return array;
} // ibis::bord::column::selectShorts

ibis::array_t<uint16_t>*
ibis::bord::column::selectUShorts(const ibis::bitvector& mask) const {
    array_t<uint16_t>* array = new array_t<uint16_t>;
    const uint32_t tot = mask.cnt();
    if (tot == 0)
	return array;
    ibis::horometer timer;
    if (ibis::gVerbose > 5)
	timer.start();

    if (m_type == USHORT) {
	const array_t<uint16_t> &prop =
	    * static_cast<const array_t<uint16_t>*>(buffer);

	uint32_t i = 0;
	array->resize(tot);
	const uint32_t nprop = prop.size();
	ibis::bitvector::indexSet index = mask.firstIndexSet();
	if (nprop >= mask.size()) {
	    while (index.nIndices() > 0) {
		const ibis::bitvector::word_t *idx0 = index.indices();
		if (index.isRange()) {
		    for (uint32_t j = *idx0; j<idx0[1]; ++j, ++i) {
			(*array)[i] = (prop[j]);
		    }
		}
		else {
		    for (uint32_t j = 0; j<index.nIndices(); ++j, ++i) {
			(*array)[i] = (prop[idx0[j]]);
		    }
		}
		++ index;
	    }
	}
	else { // need to check loop bounds against nprop
	    while (index.nIndices() > 0) {
		const ibis::bitvector::word_t *idx0 = index.indices();
		if (*idx0 >= nprop) break;
		if (index.isRange()) {
		    for (uint32_t j = *idx0;
			 j < (idx0[1]<=nprop ? idx0[1] : nprop);
			 ++j, ++i) {
			(*array)[i] = (prop[j]);
		    }
		}
		else {
		    for (uint32_t j = 0; j<index.nIndices(); ++j, ++i) {
			if (idx0[j] < nprop)
			    (*array)[i] = (prop[idx0[j]]);
			else
			    break;
		    }
		}
		++ index;
	    }
	}

	if (i != tot) {
	    array->resize(i);
	    logWarning("selectUShorts", "expects to retrieve %lu elements "
		       "but only got %lu", static_cast<long unsigned>(tot),
		       static_cast<long unsigned>(i));
	}
    }
    else if (m_type == UBYTE) {
	const array_t<unsigned char> &prop =
	    * static_cast<const array_t<unsigned char>*>(buffer);

	uint32_t i = 0;
	array->resize(tot);
	const uint32_t nprop = prop.size();
	ibis::bitvector::indexSet index = mask.firstIndexSet();
	if (nprop >= mask.size()) {
	    while (index.nIndices() > 0) {
		const ibis::bitvector::word_t *idx0 = index.indices();
		if (index.isRange()) {
		    for (uint32_t j = *idx0; j<idx0[1]; ++j, ++i) {
			(*array)[i] = (prop[j]);
		    }
		}
		else {
		    for (uint32_t j = 0; j<index.nIndices(); ++j, ++i) {
			(*array)[i] = (prop[idx0[j]]);
		    }
		}
		++ index;
	    }
	}
	else { // need to check loop bounds against nprop
	    while (index.nIndices() > 0) {
		const ibis::bitvector::word_t *idx0 = index.indices();
		if (*idx0 >= nprop) break;
		if (index.isRange()) {
		    for (uint32_t j = *idx0;
			 j < (idx0[1]<=nprop ? idx0[1] : nprop);
			 ++j, ++i) {
			(*array)[i] = (prop[j]);
		    }
		}
		else {
		    for (uint32_t j = 0; j<index.nIndices(); ++j, ++i) {
			if (idx0[j] < nprop)
			    (*array)[i] = (prop[idx0[j]]);
			else
			    break;
		    }
		}
		++ index;
	    }
	}

	if (i != tot) {
	    array->resize(i);
	    logWarning("selectUShorts", "expects to retrieve %lu elements "
		       "but only got %lu", static_cast<long unsigned>(tot),
		       static_cast<long unsigned>(i));
	}
    }
    else {
	logWarning("selectUShorts", "incompatible data type");
    }
    if (ibis::gVerbose > 5) {
	timer.stop();
	long unsigned cnt = mask.cnt();
	logMessage("selectUShorts", "retrieving %lu unsigned integer%s "
		   "took %g sec(CPU), %g sec(elapsed)",
		   static_cast<long unsigned>(cnt), (cnt > 1 ? "s" : ""),
		   timer.CPUTime(), timer.realTime());
    }
    return array;
} // ibis::bord::column::selectUShorts

ibis::array_t<int32_t>*
ibis::bord::column::selectInts(const ibis::bitvector &mask) const {
    ibis::array_t<int32_t>* array = new array_t<int32_t>;
    const uint32_t tot = mask.cnt();
    if (tot == 0)
	return array;

    ibis::horometer timer;
    if (ibis::gVerbose > 5)
	timer.start();
    if (m_type == ibis::INT) {
	const array_t<int32_t> &prop =
	    * static_cast<const array_t<int32_t>*>(buffer);
	uint32_t i = 0;
	array->resize(tot);
	const long unsigned nprop = prop.size();
#if DEBUG+0 > 0 || _DEBUG+0 > 0
	logMessage("DEBUG", "selectInts mask.size(%lu) and nprop=%lu",
		   static_cast<long unsigned>(mask.size()), nprop);
#endif
	ibis::bitvector::indexSet index = mask.firstIndexSet();
	if (nprop >= mask.size()) { // no need to check loop bounds
#if DEBUG+0 > 0 || _DEBUG+0 > 0
	    logMessage("DEBUG", "entering unchecked loops");
#endif
	    while (index.nIndices() > 0) {
		const ibis::bitvector::word_t *idx0 = index.indices();
		if (index.isRange()) {
#if DEBUG+0 > 0 || _DEBUG+0 > 0
		    logMessage("DEBUG", "copying range [%lu, %lu), i=%lu",
			       static_cast<long unsigned>(*idx0),
			       static_cast<long unsigned>(idx0[1]),
			       static_cast<long unsigned>(i));
#endif
		    for (uint32_t j = *idx0; j < idx0[1]; ++j, ++i) {
			(*array)[i] = (prop[j]);
		    }
		}
		else {
		    for (uint32_t j = 0; j<index.nIndices(); ++j, ++i) {
#if DEBUG+0 > 0 || _DEBUG+0 > 0
			logMessage("DEBUG", "copying value %lu to i=%lu",
				   static_cast<long unsigned>(idx0[j]),
				   static_cast<long unsigned>(i));
#endif
			(*array)[i] = (prop[idx0[j]]);
		    }
		}
		++ index;
	    }
	}
	else { // need to check loop bounds against nprop
#if DEBUG+0 > 0 || _DEBUG+0 > 0
	    logMessage("DEBUG", "entering checked loops");
#endif
	    while (index.nIndices() > 0) {
		const ibis::bitvector::word_t *idx0 = index.indices();
		if (*idx0 >= nprop) break;
		if (index.isRange()) {
#if DEBUG+0 > 0 || _DEBUG+0 > 0
		    logMessage("DEBUG", "copying range [%lu, %lu), i=%lu",
			       static_cast<long unsigned>(*idx0),
			       static_cast<long unsigned>
			       (idx0[1]<=nprop ? idx0[1] : nprop),
			       static_cast<long unsigned>(i));
#endif
		    for (uint32_t j = *idx0;
			 j < (idx0[1]<=nprop ? idx0[1] : nprop);
			 ++j, ++i) {
			(*array)[i] = (prop[j]);
		    }
		}
		else {
		    for (uint32_t j = 0; j<index.nIndices(); ++j, ++i) {
#if DEBUG+0 > 0 || _DEBUG+0 > 0
			logMessage("DEBUG", "copying value %lu to i=%lu",
				   static_cast<long unsigned>(idx0[j]),
				   static_cast<long unsigned>(i));
#endif
			if (idx0[j] < nprop)
			    (*array)[i] = (prop[idx0[j]]);
			else
			    break;
		    }
		}
		++ index;
	    }
	}
	if (i != tot) {
	    array->resize(i);
	    logWarning("selectInts", "expects to retrieve %lu elements "
		       "but only got %lu", static_cast<long unsigned>(tot),
		       static_cast<long unsigned>(i));
	}
    }
    else if (m_type == ibis::SHORT) {
	const array_t<int16_t> &prop =
	    * static_cast<const array_t<int16_t>*>(buffer);
	uint32_t i = 0;
	array->resize(tot);
	const uint32_t nprop = prop.size();
	ibis::bitvector::indexSet index = mask.firstIndexSet();
	if (nprop >= mask.size()) { // no need to check loop bounds
	    while (index.nIndices() > 0) {
		const ibis::bitvector::word_t *idx0 = index.indices();
		if (index.isRange()) {
		    for (uint32_t j = *idx0; j < idx0[1]; ++j, ++i) {
			(*array)[i] = (prop[j]);
		    }
		}
		else {
		    for (uint32_t j = 0; j<index.nIndices(); ++j, ++i) {
			(*array)[i] = (prop[idx0[j]]);
		    }
		}
		++ index;
	    }
	}
	else { // need to check loop bounds against nprop
	    while (index.nIndices() > 0) {
		const ibis::bitvector::word_t *idx0 = index.indices();
		if (*idx0 >= nprop) break;
		if (index.isRange()) {
		    for (uint32_t j = *idx0;
			 j < (idx0[1]<=nprop ? idx0[1] : nprop);
			 ++j, ++i) {
			(*array)[i] = (prop[j]);
		    }
		}
		else {
		    for (uint32_t j = 0; j<index.nIndices(); ++j, ++i) {
			if (idx0[j] < nprop)
			    (*array)[i] = (prop[idx0[j]]);
			else
			    break;
		    }
		}
		++ index;
	    }
	}
	if (i != tot) {
	    array->resize(i);
	    logWarning("selectInts", "expects to retrieve %lu elements "
		       "but only got %lu", static_cast<long unsigned>(tot),
		       static_cast<long unsigned>(i));
	}
    }
    else if (m_type == ibis::USHORT) {
	const array_t<uint16_t> &prop =
	    * static_cast<const array_t<uint16_t>*>(buffer);
	uint32_t i = 0;
	array->resize(tot);
	const uint32_t nprop = prop.size();
	ibis::bitvector::indexSet index = mask.firstIndexSet();
	if (nprop >= mask.size()) { // no need to check loop bounds
	    while (index.nIndices() > 0) {
		const ibis::bitvector::word_t *idx0 = index.indices();
		if (index.isRange()) {
		    for (uint32_t j = *idx0; j < idx0[1]; ++j, ++i) {
			(*array)[i] = (prop[j]);
		    }
		}
		else {
		    for (uint32_t j = 0; j<index.nIndices(); ++j, ++i) {
			(*array)[i] = (prop[idx0[j]]);
		    }
		}
		++ index;
	    }
	}
	else { // need to check loop bounds against nprop
	    while (index.nIndices() > 0) {
		const ibis::bitvector::word_t *idx0 = index.indices();
		if (*idx0 >= nprop) break;
		if (index.isRange()) {
		    for (uint32_t j = *idx0;
			 j < (idx0[1]<=nprop ? idx0[1] : nprop);
			 ++j, ++i) {
			(*array)[i] = (prop[j]);
		    }
		}
		else {
		    for (uint32_t j = 0; j<index.nIndices(); ++j, ++i) {
			if (idx0[j] < nprop)
			    (*array)[i] = (prop[idx0[j]]);
			else
			    break;
		    }
		}
		++ index;
	    }
	}
	if (i != tot) {
	    array->resize(i);
	    logWarning("selectInts", "expects to retrieve %lu elements "
		       "but only got %lu", static_cast<long unsigned>(tot),
		       static_cast<long unsigned>(i));
	}
    }
    else if (m_type == ibis::BYTE) {
	const array_t<signed char> &prop =
	    * static_cast<const array_t<signed char>*>(buffer);
	uint32_t i = 0;
	array->resize(tot);
	const uint32_t nprop = prop.size();
	ibis::bitvector::indexSet index = mask.firstIndexSet();
	if (nprop >= mask.size()) { // no need to check loop bounds
	    while (index.nIndices() > 0) {
		const ibis::bitvector::word_t *idx0 = index.indices();
		if (index.isRange()) {
		    for (uint32_t j = *idx0; j < idx0[1]; ++j, ++i) {
			(*array)[i] = (prop[j]);
		    }
		}
		else {
		    for (uint32_t j = 0; j<index.nIndices(); ++j, ++i) {
			(*array)[i] = (prop[idx0[j]]);
		    }
		}
		++ index;
	    }
	}
	else { // need to check loop bounds against nprop
	    while (index.nIndices() > 0) {
		const ibis::bitvector::word_t *idx0 = index.indices();
		if (*idx0 >= nprop) break;
		if (index.isRange()) {
		    for (uint32_t j = *idx0;
			 j < (idx0[1]<=nprop ? idx0[1] : nprop);
			 ++j, ++i) {
			(*array)[i] = (prop[j]);
		    }
		}
		else {
		    for (uint32_t j = 0; j<index.nIndices(); ++j, ++i) {
			if (idx0[j] < nprop)
			    (*array)[i] = (prop[idx0[j]]);
			else
			    break;
		    }
		}
		++ index;
	    }
	}
	if (i != tot) {
	    array->resize(i);
	    logWarning("selectInts", "expects to retrieve %lu elements "
		       "but only got %lu", static_cast<long unsigned>(tot),
		       static_cast<long unsigned>(i));
	}
    }
    else if (m_type == ibis::UBYTE) {
	const array_t<unsigned char> &prop =
	    * static_cast<const array_t<unsigned char>*>(buffer);
	uint32_t i = 0;
	array->resize(tot);
	const uint32_t nprop = prop.size();
	ibis::bitvector::indexSet index = mask.firstIndexSet();
	if (nprop >= mask.size()) { // no need to check loop bounds
	    while (index.nIndices() > 0) {
		const ibis::bitvector::word_t *idx0 = index.indices();
		if (index.isRange()) {
		    for (uint32_t j = *idx0; j < idx0[1]; ++j, ++i) {
			(*array)[i] = (prop[j]);
		    }
		}
		else {
		    for (uint32_t j = 0; j<index.nIndices(); ++j, ++i) {
			(*array)[i] = (prop[idx0[j]]);
		    }
		}
		++ index;
	    }
	}
	else { // need to check loop bounds against nprop
	    while (index.nIndices() > 0) {
		const ibis::bitvector::word_t *idx0 = index.indices();
		if (*idx0 >= nprop) break;
		if (index.isRange()) {
		    for (uint32_t j = *idx0;
			 j < (idx0[1]<=nprop ? idx0[1] : nprop);
			 ++j, ++i) {
			(*array)[i] = (prop[j]);
		    }
		}
		else {
		    for (uint32_t j = 0; j<index.nIndices(); ++j, ++i) {
			if (idx0[j] < nprop)
			    (*array)[i] = (prop[idx0[j]]);
			else
			    break;
		    }
		}
		++ index;
	    }
	}
	if (i != tot) {
	    array->resize(i);
	    logWarning("selectInts", "expects to retrieve %lu elements "
		       "but only got %lu", static_cast<long unsigned>(tot),
		       static_cast<long unsigned>(i));
	}
    }
    else {
	logWarning("selectInts", "incompatible data type");
    }
    if (ibis::gVerbose > 5) {
	timer.stop();
	long unsigned cnt = mask.cnt();
	logMessage("selectInts", "retrieving %lu integer%s "
		   "took %g sec(CPU), %g sec(elapsed)",
		   static_cast<long unsigned>(cnt), (cnt > 1 ? "s" : ""),
		   timer.CPUTime(), timer.realTime());
    }
    return array;
} // ibis::bord::column::selectInts

/// Can be called on columns of unsigned integral types, UINT, CATEGORY,
/// USHORT, and UBYTE.
ibis::array_t<uint32_t>*
ibis::bord::column::selectUInts(const ibis::bitvector& mask) const {
    array_t<uint32_t>* array = new array_t<uint32_t>;
    const uint32_t tot = mask.cnt();
    if (tot == 0)
	return array;
    ibis::horometer timer;
    if (ibis::gVerbose > 5)
	timer.start();

    if (m_type == ibis::UINT || m_type == ibis::CATEGORY ||
	m_type == ibis::TEXT) {
	const array_t<uint32_t> &prop =
	    * static_cast<const array_t<uint32_t>*>(buffer);

	uint32_t i = 0;
	array->resize(tot);
	const uint32_t nprop = prop.size();
	ibis::bitvector::indexSet index = mask.firstIndexSet();
	if (nprop >= mask.size()) {
	    while (index.nIndices() > 0) {
		const ibis::bitvector::word_t *idx0 = index.indices();
		if (index.isRange()) {
		    for (uint32_t j = *idx0; j<idx0[1]; ++j, ++i) {
			(*array)[i] = (prop[j]);
		    }
		}
		else {
		    for (uint32_t j = 0; j<index.nIndices(); ++j, ++i) {
			(*array)[i] = (prop[idx0[j]]);
		    }
		}
		++ index;
	    }
	}
	else { // need to check loop bounds against nprop
	    while (index.nIndices() > 0) {
		const ibis::bitvector::word_t *idx0 = index.indices();
		if (*idx0 >= nprop) break;
		if (index.isRange()) {
		    for (uint32_t j = *idx0;
			 j < (idx0[1]<=nprop ? idx0[1] : nprop);
			 ++j, ++i) {
			(*array)[i] = (prop[j]);
		    }
		}
		else {
		    for (uint32_t j = 0; j<index.nIndices(); ++j, ++i) {
			if (idx0[j] < nprop)
			    (*array)[i] = (prop[idx0[j]]);
			else
			    break;
		    }
		}
		++ index;
	    }
	}

	if (i != tot) {
	    array->resize(i);
	    logWarning("selectUInts", "expects to retrieve %lu elements "
		       "but only got %lu", static_cast<long unsigned>(tot),
		       static_cast<long unsigned>(i));
	}
    }
    else if (m_type == USHORT) {
	const array_t<uint16_t> &prop =
	    * static_cast<const array_t<uint16_t>*>(buffer);

	uint32_t i = 0;
	array->resize(tot);
	const uint32_t nprop = prop.size();
	ibis::bitvector::indexSet index = mask.firstIndexSet();
	if (nprop >= mask.size()) {
	    while (index.nIndices() > 0) {
		const ibis::bitvector::word_t *idx0 = index.indices();
		if (index.isRange()) {
		    for (uint32_t j = *idx0; j<idx0[1]; ++j, ++i) {
			(*array)[i] = (prop[j]);
		    }
		}
		else {
		    for (uint32_t j = 0; j<index.nIndices(); ++j, ++i) {
			(*array)[i] = (prop[idx0[j]]);
		    }
		}
		++ index;
	    }
	}
	else { // need to check loop bounds against nprop
	    while (index.nIndices() > 0) {
		const ibis::bitvector::word_t *idx0 = index.indices();
		if (*idx0 >= nprop) break;
		if (index.isRange()) {
		    for (uint32_t j = *idx0;
			 j < (idx0[1]<=nprop ? idx0[1] : nprop);
			 ++j, ++i) {
			(*array)[i] = (prop[j]);
		    }
		}
		else {
		    for (uint32_t j = 0; j<index.nIndices(); ++j, ++i) {
			if (idx0[j] < nprop)
			    (*array)[i] = (prop[idx0[j]]);
			else
			    break;
		    }
		}
		++ index;
	    }
	}

	if (i != tot) {
	    array->resize(i);
	    logWarning("selectUInts", "expects to retrieve %lu elements "
		       "but only got %lu", static_cast<long unsigned>(tot),
		       static_cast<long unsigned>(i));
	}
    }
    else if (m_type == UBYTE) {
	const array_t<unsigned char> &prop =
	    * static_cast<const array_t<unsigned char>*>(buffer);

	uint32_t i = 0;
	array->resize(tot);
	const uint32_t nprop = prop.size();
	ibis::bitvector::indexSet index = mask.firstIndexSet();
	if (nprop >= mask.size()) {
	    while (index.nIndices() > 0) {
		const ibis::bitvector::word_t *idx0 = index.indices();
		if (index.isRange()) {
		    for (uint32_t j = *idx0; j<idx0[1]; ++j, ++i) {
			(*array)[i] = (prop[j]);
		    }
		}
		else {
		    for (uint32_t j = 0; j<index.nIndices(); ++j, ++i) {
			(*array)[i] = (prop[idx0[j]]);
		    }
		}
		++ index;
	    }
	}
	else { // need to check loop bounds against nprop
	    while (index.nIndices() > 0) {
		const ibis::bitvector::word_t *idx0 = index.indices();
		if (*idx0 >= nprop) break;
		if (index.isRange()) {
		    for (uint32_t j = *idx0;
			 j < (idx0[1]<=nprop ? idx0[1] : nprop);
			 ++j, ++i) {
			(*array)[i] = (prop[j]);
		    }
		}
		else {
		    for (uint32_t j = 0; j<index.nIndices(); ++j, ++i) {
			if (idx0[j] < nprop)
			    (*array)[i] = (prop[idx0[j]]);
			else
			    break;
		    }
		}
		++ index;
	    }
	}

	if (i != tot) {
	    array->resize(i);
	    logWarning("selectUInts", "expects to retrieve %lu elements "
		       "but only got %lu", static_cast<long unsigned>(tot),
		       static_cast<long unsigned>(i));
	}
    }
    else {
	logWarning("selectUInts", "incompatible data type");
    }
    if (ibis::gVerbose > 5) {
	timer.stop();
	long unsigned cnt = mask.cnt();
	logMessage("selectUInts", "retrieving %lu unsigned integer%s "
		   "took %g sec(CPU), %g sec(elapsed)",
		   static_cast<long unsigned>(cnt), (cnt > 1 ? "s" : ""),
		   timer.CPUTime(), timer.realTime());
    }
    return array;
} // ibis::bord::column::selectUInts

/// Can be called on all integral types.  Note that 64-byte unsigned
/// integers are simply treated as signed integer.  This may cause the
/// values to be interperted incorrectly.  Shorter version of unsigned
/// integers are treated correctly as positive values.
ibis::array_t<int64_t>*
ibis::bord::column::selectLongs(const ibis::bitvector& mask) const {
    ibis::array_t<int64_t>* array = new array_t<int64_t>;
    const uint32_t tot = mask.cnt();
    if (tot == 0)
	return array;

    ibis::horometer timer;
    if (ibis::gVerbose > 5)
	timer.start();
    if (m_type == ibis::LONG) {
	const array_t<int64_t> &prop =
	    * static_cast<const array_t<int64_t>*>(buffer);

	uint32_t i = 0;
	array->resize(tot);
	const long unsigned nprop = prop.size();
#if DEBUG+0 > 0 || _DEBUG+0 > 0
	logMessage("DEBUG", "selectLongs mask.size(%lu) and nprop=%lu",
		   static_cast<long unsigned>(mask.size()), nprop);
#endif
	ibis::bitvector::indexSet index = mask.firstIndexSet();
	if (nprop >= mask.size()) { // no need to check loop bounds
#if DEBUG+0 > 0 || _DEBUG+0 > 0
	    logMessage("DEBUG", "entering unchecked loops");
#endif
	    while (index.nIndices() > 0) {
		const ibis::bitvector::word_t *idx0 = index.indices();
		if (index.isRange()) {
#if DEBUG+0 > 0 || _DEBUG+0 > 0
		    logMessage("DEBUG", "copying range [%lu, %lu), i=%lu",
			       static_cast<long unsigned>(*idx0),
			       static_cast<long unsigned>(idx0[1]),
			       static_cast<long unsigned>(i));
#endif
		    for (uint32_t j = *idx0; j < idx0[1]; ++j, ++i) {
			(*array)[i] = (prop[j]);
		    }
		}
		else {
		    for (uint32_t j = 0; j<index.nIndices(); ++j, ++i) {
#if DEBUG+0 > 0 || _DEBUG+0 > 0
			logMessage("DEBUG", "copying value %lu to i=%lu",
				   static_cast<long unsigned>(idx0[j]),
				   static_cast<long unsigned>(i));
#endif
			(*array)[i] = (prop[idx0[j]]);
		    }
		}
		++ index;
	    }
	}
	else { // need to check loop bounds against nprop
#if DEBUG+0 > 0 || _DEBUG+0 > 0
	    logMessage("DEBUG", "entering checked loops");
#endif
	    while (index.nIndices() > 0) {
		const ibis::bitvector::word_t *idx0 = index.indices();
		if (*idx0 >= nprop) break;
		if (index.isRange()) {
#if DEBUG+0 > 0 || _DEBUG+0 > 0
		    logMessage("DEBUG", "copying range [%lu, %lu), i=%lu",
			       static_cast<long unsigned>(*idx0),
			       static_cast<long unsigned>
			       (idx0[1]<=nprop ? idx0[1] : nprop),
			       static_cast<long unsigned>(i));
#endif
		    for (uint32_t j = *idx0;
			 j < (idx0[1]<=nprop ? idx0[1] : nprop);
			 ++j, ++i) {
			(*array)[i] = (prop[j]);
		    }
		}
		else {
		    for (uint32_t j = 0; j<index.nIndices(); ++j, ++i) {
#if DEBUG+0 > 0 || _DEBUG+0 > 0
			logMessage("DEBUG", "copying value %lu to i=%lu",
				   static_cast<long unsigned>(idx0[j]),
				   static_cast<long unsigned>(i));
#endif
			if (idx0[j] < nprop)
			    (*array)[i] = (prop[idx0[j]]);
			else
			    break;
		    }
		}
		++ index;
	    }
	}
	if (i != tot) {
	    array->resize(i);
	    logWarning("selectLongs", "expects to retrieve %lu elements "
		       "but only got %lu", static_cast<long unsigned>(tot),
		       static_cast<long unsigned>(i));
	}
    }
    else if (m_type == ibis::UINT || m_type == ibis::CATEGORY ||
	     m_type == ibis::TEXT) {
	const array_t<uint32_t> &prop =
	    * static_cast<const array_t<uint32_t>*>(buffer);

	uint32_t i = 0;
	array->resize(tot);
	const long unsigned nprop = prop.size();
#if DEBUG+0 > 0 || _DEBUG+0 > 0
	logMessage("DEBUG", "selectLongs mask.size(%lu) and nprop=%lu",
		   static_cast<long unsigned>(mask.size()), nprop);
#endif
	ibis::bitvector::indexSet index = mask.firstIndexSet();
	if (nprop >= mask.size()) { // no need to check loop bounds
#if DEBUG+0 > 0 || _DEBUG+0 > 0
	    logMessage("DEBUG", "entering unchecked loops");
#endif
	    while (index.nIndices() > 0) {
		const ibis::bitvector::word_t *idx0 = index.indices();
		if (index.isRange()) {
#if DEBUG+0 > 0 || _DEBUG+0 > 0
		    logMessage("DEBUG", "copying range [%lu, %lu), i=%lu",
			       static_cast<long unsigned>(*idx0),
			       static_cast<long unsigned>(idx0[1]),
			       static_cast<long unsigned>(i));
#endif
		    for (uint32_t j = *idx0; j < idx0[1]; ++j, ++i) {
			(*array)[i] = (prop[j]);
		    }
		}
		else {
		    for (uint32_t j = 0; j<index.nIndices(); ++j, ++i) {
#if DEBUG+0 > 0 || _DEBUG+0 > 0
			logMessage("DEBUG", "copying value %lu to i=%lu",
				   static_cast<long unsigned>(idx0[j]),
				   static_cast<long unsigned>(i));
#endif
			(*array)[i] = (prop[idx0[j]]);
		    }
		}
		++ index;
	    }
	}
	else { // need to check loop bounds against nprop
#if DEBUG+0 > 0 || _DEBUG+0 > 0
	    logMessage("DEBUG", "entering checked loops");
#endif
	    while (index.nIndices() > 0) {
		const ibis::bitvector::word_t *idx0 = index.indices();
		if (*idx0 >= nprop) break;
		if (index.isRange()) {
#if DEBUG+0 > 0 || _DEBUG+0 > 0
		    logMessage("DEBUG", "copying range [%lu, %lu), i=%lu",
			       static_cast<long unsigned>(*idx0),
			       static_cast<long unsigned>
			       (idx0[1]<=nprop ? idx0[1] : nprop),
			       static_cast<long unsigned>(i));
#endif
		    for (uint32_t j = *idx0;
			 j < (idx0[1]<=nprop ? idx0[1] : nprop);
			 ++j, ++i) {
			(*array)[i] = (prop[j]);
		    }
		}
		else {
		    for (uint32_t j = 0; j<index.nIndices(); ++j, ++i) {
#if DEBUG+0 > 0 || _DEBUG+0 > 0
			logMessage("DEBUG", "copying value %lu to i=%lu",
				   static_cast<long unsigned>(idx0[j]),
				   static_cast<long unsigned>(i));
#endif
			if (idx0[j] < nprop)
			    (*array)[i] = (prop[idx0[j]]);
			else
			    break;
		    }
		}
		++ index;
	    }
	}
	if (i != tot) {
	    array->resize(i);
	    logWarning("selectLongs", "expects to retrieve %lu elements "
		       "but only got %lu", static_cast<long unsigned>(tot),
		       static_cast<long unsigned>(i));
	}
    }
    else if (m_type == ibis::INT) {
	const array_t<int32_t> &prop =
	    * static_cast<const array_t<int32_t>*>(buffer);

	uint32_t i = 0;
	array->resize(tot);
	const long unsigned nprop = prop.size();
#if DEBUG+0 > 0 || _DEBUG+0 > 0
	logMessage("DEBUG", "selectLongs mask.size(%lu) and nprop=%lu",
		   static_cast<long unsigned>(mask.size()), nprop);
#endif
	ibis::bitvector::indexSet index = mask.firstIndexSet();
	if (nprop >= mask.size()) { // no need to check loop bounds
#if DEBUG+0 > 0 || _DEBUG+0 > 0
	    logMessage("DEBUG", "entering unchecked loops");
#endif
	    while (index.nIndices() > 0) {
		const ibis::bitvector::word_t *idx0 = index.indices();
		if (index.isRange()) {
#if DEBUG+0 > 0 || _DEBUG+0 > 0
		    logMessage("DEBUG", "copying range [%lu, %lu), i=%lu",
			       static_cast<long unsigned>(*idx0),
			       static_cast<long unsigned>(idx0[1]),
			       static_cast<long unsigned>(i));
#endif
		    for (uint32_t j = *idx0; j < idx0[1]; ++j, ++i) {
			(*array)[i] = (prop[j]);
		    }
		}
		else {
		    for (uint32_t j = 0; j<index.nIndices(); ++j, ++i) {
#if DEBUG+0 > 0 || _DEBUG+0 > 0
			logMessage("DEBUG", "copying value %lu to i=%lu",
				   static_cast<long unsigned>(idx0[j]),
				   static_cast<long unsigned>(i));
#endif
			(*array)[i] = (prop[idx0[j]]);
		    }
		}
		++ index;
	    }
	}
	else { // need to check loop bounds against nprop
#if DEBUG+0 > 0 || _DEBUG+0 > 0
	    logMessage("DEBUG", "entering checked loops");
#endif
	    while (index.nIndices() > 0) {
		const ibis::bitvector::word_t *idx0 = index.indices();
		if (*idx0 >= nprop) break;
		if (index.isRange()) {
#if DEBUG+0 > 0 || _DEBUG+0 > 0
		    logMessage("DEBUG", "copying range [%lu, %lu), i=%lu",
			       static_cast<long unsigned>(*idx0),
			       static_cast<long unsigned>
			       (idx0[1]<=nprop ? idx0[1] : nprop),
			       static_cast<long unsigned>(i));
#endif
		    for (uint32_t j = *idx0;
			 j < (idx0[1]<=nprop ? idx0[1] : nprop);
			 ++j, ++i) {
			(*array)[i] = (prop[j]);
		    }
		}
		else {
		    for (uint32_t j = 0; j<index.nIndices(); ++j, ++i) {
#if DEBUG+0 > 0 || _DEBUG+0 > 0
			logMessage("DEBUG", "copying value %lu to i=%lu",
				   static_cast<long unsigned>(idx0[j]),
				   static_cast<long unsigned>(i));
#endif
			if (idx0[j] < nprop)
			    (*array)[i] = (prop[idx0[j]]);
			else
			    break;
		    }
		}
		++ index;
	    }
	}
	if (i != tot) {
	    array->resize(i);
	    logWarning("selectLongs", "expects to retrieve %lu elements "
		       "but only got %lu", static_cast<long unsigned>(tot),
		       static_cast<long unsigned>(i));
	}
    }
    else if (m_type == ibis::USHORT) {
	const array_t<uint16_t> &prop =
	    * static_cast<const array_t<uint16_t>*>(buffer);

	uint32_t i = 0;
	array->resize(tot);
	const uint32_t nprop = prop.size();
	ibis::bitvector::indexSet index = mask.firstIndexSet();
	if (nprop >= mask.size()) { // no need to check loop bounds
	    while (index.nIndices() > 0) {
		const ibis::bitvector::word_t *idx0 = index.indices();
		if (index.isRange()) {
		    for (uint32_t j = *idx0; j < idx0[1]; ++j, ++i) {
			(*array)[i] = (prop[j]);
		    }
		}
		else {
		    for (uint32_t j = 0; j<index.nIndices(); ++j, ++i) {
			(*array)[i] = (prop[idx0[j]]);
		    }
		}
		++ index;
	    }
	}
	else { // need to check loop bounds against nprop
	    while (index.nIndices() > 0) {
		const ibis::bitvector::word_t *idx0 = index.indices();
		if (*idx0 >= nprop) break;
		if (index.isRange()) {
		    for (uint32_t j = *idx0;
			 j < (idx0[1]<=nprop ? idx0[1] : nprop);
			 ++j, ++i) {
			(*array)[i] = (prop[j]);
		    }
		}
		else {
		    for (uint32_t j = 0; j<index.nIndices(); ++j, ++i) {
			if (idx0[j] < nprop)
			    (*array)[i] = (prop[idx0[j]]);
			else
			    break;
		    }
		}
		++ index;
	    }
	}
	if (i != tot) {
	    array->resize(i);
	    logWarning("selectLongs", "expects to retrieve %lu elements "
		       "but only got %lu", static_cast<long unsigned>(tot),
		       static_cast<long unsigned>(i));
	}
    }
    else if (m_type == SHORT) {
	const array_t<int16_t> &prop =
	    * static_cast<const array_t<int16_t>*>(buffer);

	uint32_t i = 0;
	array->resize(tot);
	const uint32_t nprop = prop.size();
	ibis::bitvector::indexSet index = mask.firstIndexSet();
	if (nprop >= mask.size()) { // no need to check loop bounds
	    while (index.nIndices() > 0) {
		const ibis::bitvector::word_t *idx0 = index.indices();
		if (index.isRange()) {
		    for (uint32_t j = *idx0; j < idx0[1]; ++j, ++i) {
			(*array)[i] = (prop[j]);
		    }
		}
		else {
		    for (uint32_t j = 0; j<index.nIndices(); ++j, ++i) {
			(*array)[i] = (prop[idx0[j]]);
		    }
		}
		++ index;
	    }
	}
	else { // need to check loop bounds against nprop
	    while (index.nIndices() > 0) {
		const ibis::bitvector::word_t *idx0 = index.indices();
		if (*idx0 >= nprop) break;
		if (index.isRange()) {
		    for (uint32_t j = *idx0;
			 j < (idx0[1]<=nprop ? idx0[1] : nprop);
			 ++j, ++i) {
			(*array)[i] = (prop[j]);
		    }
		}
		else {
		    for (uint32_t j = 0; j<index.nIndices(); ++j, ++i) {
			if (idx0[j] < nprop)
			    (*array)[i] = (prop[idx0[j]]);
			else
			    break;
		    }
		}
		++ index;
	    }
	}
	if (i != tot) {
	    array->resize(i);
	    logWarning("selectLongs", "expects to retrieve %lu elements "
		       "but only got %lu", static_cast<long unsigned>(tot),
		       static_cast<long unsigned>(i));
	}
    }
    else if (m_type == UBYTE) {
	const array_t<unsigned char> &prop =
	    * static_cast<const array_t<unsigned char>*>(buffer);

	uint32_t i = 0;
	array->resize(tot);
	const uint32_t nprop = prop.size();
	ibis::bitvector::indexSet index = mask.firstIndexSet();
	if (nprop >= mask.size()) { // no need to check loop bounds
	    while (index.nIndices() > 0) {
		const ibis::bitvector::word_t *idx0 = index.indices();
		if (index.isRange()) {
		    for (uint32_t j = *idx0; j < idx0[1]; ++j, ++i) {
			(*array)[i] = (prop[j]);
		    }
		}
		else {
		    for (uint32_t j = 0; j<index.nIndices(); ++j, ++i) {
			(*array)[i] = (prop[idx0[j]]);
		    }
		}
		++ index;
	    }
	}
	else { // need to check loop bounds against nprop
	    while (index.nIndices() > 0) {
		const ibis::bitvector::word_t *idx0 = index.indices();
		if (*idx0 >= nprop) break;
		if (index.isRange()) {
		    for (uint32_t j = *idx0;
			 j < (idx0[1]<=nprop ? idx0[1] : nprop);
			 ++j, ++i) {
			(*array)[i] = (prop[j]);
		    }
		}
		else {
		    for (uint32_t j = 0; j<index.nIndices(); ++j, ++i) {
			if (idx0[j] < nprop)
			    (*array)[i] = (prop[idx0[j]]);
			else
			    break;
		    }
		}
		++ index;
	    }
	}
	if (i != tot) {
	    array->resize(i);
	    logWarning("selectLongs", "expects to retrieve %lu elements "
		       "but only got %lu", static_cast<long unsigned>(tot),
		       static_cast<long unsigned>(i));
	}
    }
    else if (m_type == BYTE) {
	const array_t<signed char> &prop =
	    * static_cast<const array_t<signed char>*>(buffer);

	uint32_t i = 0;
	array->resize(tot);
	const uint32_t nprop = prop.size();
	ibis::bitvector::indexSet index = mask.firstIndexSet();
	if (nprop >= mask.size()) { // no need to check loop bounds
	    while (index.nIndices() > 0) {
		const ibis::bitvector::word_t *idx0 = index.indices();
		if (index.isRange()) {
		    for (uint32_t j = *idx0; j < idx0[1]; ++j, ++i) {
			(*array)[i] = (prop[j]);
		    }
		}
		else {
		    for (uint32_t j = 0; j<index.nIndices(); ++j, ++i) {
			(*array)[i] = (prop[idx0[j]]);
		    }
		}
		++ index;
	    }
	}
	else { // need to check loop bounds against nprop
	    while (index.nIndices() > 0) {
		const ibis::bitvector::word_t *idx0 = index.indices();
		if (*idx0 >= nprop) break;
		if (index.isRange()) {
		    for (uint32_t j = *idx0;
			 j < (idx0[1]<=nprop ? idx0[1] : nprop);
			 ++j, ++i) {
			(*array)[i] = (prop[j]);
		    }
		}
		else {
		    for (uint32_t j = 0; j<index.nIndices(); ++j, ++i) {
			if (idx0[j] < nprop)
			    (*array)[i] = (prop[idx0[j]]);
			else
			    break;
		    }
		}
		++ index;
	    }
	}
	if (i != tot) {
	    array->resize(i);
	    logWarning("selectLongs", "expects to retrieve %lu elements "
		       "but only got %lu", static_cast<long unsigned>(tot),
		       static_cast<long unsigned>(i));
	}
    }
    else {
	logWarning("selectLongs", "incompatible data type");
    }
    if (ibis::gVerbose > 5) {
	timer.stop();
	long unsigned cnt = mask.cnt();
	logMessage("selectLongs", "retrieving %lu integer%s "
		   "took %g sec(CPU), %g sec(elapsed)",
		   static_cast<long unsigned>(cnt), (cnt > 1 ? "s" : ""),
		   timer.CPUTime(), timer.realTime());
    }
    return array;
} // ibis::bord::column::selectLongs

ibis::array_t<uint64_t>*
ibis::bord::column::selectULongs(const ibis::bitvector& mask) const {
    ibis::array_t<uint64_t>* array = new array_t<uint64_t>;
    const uint32_t tot = mask.cnt();
    if (tot == 0)
	return array;

    ibis::horometer timer;
    if (ibis::gVerbose > 5)
	timer.start();
    if (m_type == ibis::ULONG) {
	const array_t<uint64_t> &prop =
	    * static_cast<const array_t<uint64_t>*>(buffer);

	uint32_t i = 0;
	array->resize(tot);
	const long unsigned nprop = prop.size();
#if DEBUG+0 > 0 || _DEBUG+0 > 0
	logMessage("DEBUG", "selectULongs mask.size(%lu) and nprop=%lu",
		   static_cast<long unsigned>(mask.size()), nprop);
#endif
	ibis::bitvector::indexSet index = mask.firstIndexSet();
	if (nprop >= mask.size()) { // no need to check loop bounds
#if DEBUG+0 > 0 || _DEBUG+0 > 0
	    logMessage("DEBUG", "entering unchecked loops");
#endif
	    while (index.nIndices() > 0) {
		const ibis::bitvector::word_t *idx0 = index.indices();
		if (index.isRange()) {
#if DEBUG+0 > 0 || _DEBUG+0 > 0
		    logMessage("DEBUG", "copying range [%lu, %lu), i=%lu",
			       static_cast<long unsigned>(*idx0),
			       static_cast<long unsigned>(idx0[1]),
			       static_cast<long unsigned>(i));
#endif
		    for (uint32_t j = *idx0; j < idx0[1]; ++j, ++i) {
			(*array)[i] = (prop[j]);
		    }
		}
		else {
		    for (uint32_t j = 0; j<index.nIndices(); ++j, ++i) {
#if DEBUG+0 > 0 || _DEBUG+0 > 0
			logMessage("DEBUG", "copying value %lu to i=%lu",
				   static_cast<long unsigned>(idx0[j]),
				   static_cast<long unsigned>(i));
#endif
			(*array)[i] = (prop[idx0[j]]);
		    }
		}
		++ index;
	    }
	}
	else { // need to check loop bounds against nprop
#if DEBUG+0 > 0 || _DEBUG+0 > 0
	    logMessage("DEBUG", "entering checked loops");
#endif
	    while (index.nIndices() > 0) {
		const ibis::bitvector::word_t *idx0 = index.indices();
		if (*idx0 >= nprop) break;
		if (index.isRange()) {
#if DEBUG+0 > 0 || _DEBUG+0 > 0
		    logMessage("DEBUG", "copying range [%lu, %lu), i=%lu",
			       static_cast<long unsigned>(*idx0),
			       static_cast<long unsigned>
			       (idx0[1]<=nprop ? idx0[1] : nprop),
			       static_cast<long unsigned>(i));
#endif
		    for (uint32_t j = *idx0;
			 j < (idx0[1]<=nprop ? idx0[1] : nprop);
			 ++j, ++i) {
			(*array)[i] = (prop[j]);
		    }
		}
		else {
		    for (uint32_t j = 0; j<index.nIndices(); ++j, ++i) {
#if DEBUG+0 > 0 || _DEBUG+0 > 0
			logMessage("DEBUG", "copying value %lu to i=%lu",
				   static_cast<long unsigned>(idx0[j]),
				   static_cast<long unsigned>(i));
#endif
			if (idx0[j] < nprop)
			    (*array)[i] = (prop[idx0[j]]);
			else
			    break;
		    }
		}
		++ index;
	    }
	}
	if (i != tot) {
	    array->resize(i);
	    logWarning("selectULongs", "expects to retrieve %lu elements "
		       "but only got %lu", static_cast<long unsigned>(tot),
		       static_cast<long unsigned>(i));
	}
    }
    else if (m_type == ibis::UINT || m_type == ibis::CATEGORY ||
	     m_type == ibis::TEXT) {
	const array_t<uint32_t> &prop =
	    * static_cast<const array_t<uint32_t>*>(buffer);

	uint32_t i = 0;
	array->resize(tot);
	const long unsigned nprop = prop.size();
#if DEBUG+0 > 0 || _DEBUG+0 > 0
	logMessage("DEBUG", "selectULongs mask.size(%lu) and nprop=%lu",
		   static_cast<long unsigned>(mask.size()), nprop);
#endif
	ibis::bitvector::indexSet index = mask.firstIndexSet();
	if (nprop >= mask.size()) { // no need to check loop bounds
#if DEBUG+0 > 0 || _DEBUG+0 > 0
	    logMessage("DEBUG", "entering unchecked loops");
#endif
	    while (index.nIndices() > 0) {
		const ibis::bitvector::word_t *idx0 = index.indices();
		if (index.isRange()) {
#if DEBUG+0 > 0 || _DEBUG+0 > 0
		    logMessage("DEBUG", "copying range [%lu, %lu), i=%lu",
			       static_cast<long unsigned>(*idx0),
			       static_cast<long unsigned>(idx0[1]),
			       static_cast<long unsigned>(i));
#endif
		    for (uint32_t j = *idx0; j < idx0[1]; ++j, ++i) {
			(*array)[i] = (prop[j]);
		    }
		}
		else {
		    for (uint32_t j = 0; j<index.nIndices(); ++j, ++i) {
#if DEBUG+0 > 0 || _DEBUG+0 > 0
			logMessage("DEBUG", "copying value %lu to i=%lu",
				   static_cast<long unsigned>(idx0[j]),
				   static_cast<long unsigned>(i));
#endif
			(*array)[i] = (prop[idx0[j]]);
		    }
		}
		++ index;
	    }
	}
	else { // need to check loop bounds against nprop
#if DEBUG+0 > 0 || _DEBUG+0 > 0
	    logMessage("DEBUG", "entering checked loops");
#endif
	    while (index.nIndices() > 0) {
		const ibis::bitvector::word_t *idx0 = index.indices();
		if (*idx0 >= nprop) break;
		if (index.isRange()) {
#if DEBUG+0 > 0 || _DEBUG+0 > 0
		    logMessage("DEBUG", "copying range [%lu, %lu), i=%lu",
			       static_cast<long unsigned>(*idx0),
			       static_cast<long unsigned>
			       (idx0[1]<=nprop ? idx0[1] : nprop),
			       static_cast<long unsigned>(i));
#endif
		    for (uint32_t j = *idx0;
			 j < (idx0[1]<=nprop ? idx0[1] : nprop);
			 ++j, ++i) {
			(*array)[i] = (prop[j]);
		    }
		}
		else {
		    for (uint32_t j = 0; j<index.nIndices(); ++j, ++i) {
#if DEBUG+0 > 0 || _DEBUG+0 > 0
			logMessage("DEBUG", "copying value %lu to i=%lu",
				   static_cast<long unsigned>(idx0[j]),
				   static_cast<long unsigned>(i));
#endif
			if (idx0[j] < nprop)
			    (*array)[i] = (prop[idx0[j]]);
			else
			    break;
		    }
		}
		++ index;
	    }
	}
	if (i != tot) {
	    array->resize(i);
	    logWarning("selectULongs", "expects to retrieve %lu elements "
		       "but only got %lu", static_cast<long unsigned>(tot),
		       static_cast<long unsigned>(i));
	}
    }
    else if (m_type == ibis::INT) {
	const array_t<int32_t> &prop =
	    * static_cast<const array_t<int32_t>*>(buffer);

	uint32_t i = 0;
	array->resize(tot);
	const long unsigned nprop = prop.size();
#if DEBUG+0 > 0 || _DEBUG+0 > 0
	logMessage("DEBUG", "selectULongs mask.size(%lu) and nprop=%lu",
		   static_cast<long unsigned>(mask.size()), nprop);
#endif
	ibis::bitvector::indexSet index = mask.firstIndexSet();
	if (nprop >= mask.size()) { // no need to check loop bounds
#if DEBUG+0 > 0 || _DEBUG+0 > 0
	    logMessage("DEBUG", "entering unchecked loops");
#endif
	    while (index.nIndices() > 0) {
		const ibis::bitvector::word_t *idx0 = index.indices();
		if (index.isRange()) {
#if DEBUG+0 > 0 || _DEBUG+0 > 0
		    logMessage("DEBUG", "copying range [%lu, %lu), i=%lu",
			       static_cast<long unsigned>(*idx0),
			       static_cast<long unsigned>(idx0[1]),
			       static_cast<long unsigned>(i));
#endif
		    for (uint32_t j = *idx0; j < idx0[1]; ++j, ++i) {
			(*array)[i] = (prop[j]);
		    }
		}
		else {
		    for (uint32_t j = 0; j<index.nIndices(); ++j, ++i) {
#if DEBUG+0 > 0 || _DEBUG+0 > 0
			logMessage("DEBUG", "copying value %lu to i=%lu",
				   static_cast<long unsigned>(idx0[j]),
				   static_cast<long unsigned>(i));
#endif
			(*array)[i] = (prop[idx0[j]]);
		    }
		}
		++ index;
	    }
	}
	else { // need to check loop bounds against nprop
#if DEBUG+0 > 0 || _DEBUG+0 > 0
	    logMessage("DEBUG", "entering checked loops");
#endif
	    while (index.nIndices() > 0) {
		const ibis::bitvector::word_t *idx0 = index.indices();
		if (*idx0 >= nprop) break;
		if (index.isRange()) {
#if DEBUG+0 > 0 || _DEBUG+0 > 0
		    logMessage("DEBUG", "copying range [%lu, %lu), i=%lu",
			       static_cast<long unsigned>(*idx0),
			       static_cast<long unsigned>
			       (idx0[1]<=nprop ? idx0[1] : nprop),
			       static_cast<long unsigned>(i));
#endif
		    for (uint32_t j = *idx0;
			 j < (idx0[1]<=nprop ? idx0[1] : nprop);
			 ++j, ++i) {
			(*array)[i] = (prop[j]);
		    }
		}
		else {
		    for (uint32_t j = 0; j<index.nIndices(); ++j, ++i) {
#if DEBUG+0 > 0 || _DEBUG+0 > 0
			logMessage("DEBUG", "copying value %lu to i=%lu",
				   static_cast<long unsigned>(idx0[j]),
				   static_cast<long unsigned>(i));
#endif
			if (idx0[j] < nprop)
			    (*array)[i] = (prop[idx0[j]]);
			else
			    break;
		    }
		}
		++ index;
	    }
	}
	if (i != tot) {
	    array->resize(i);
	    logWarning("selectULongs", "expects to retrieve %lu elements "
		       "but only got %lu", static_cast<long unsigned>(tot),
		       static_cast<long unsigned>(i));
	}
    }
    else if (m_type == ibis::USHORT) {
	const array_t<uint16_t> &prop =
	    * static_cast<const array_t<uint16_t>*>(buffer);

	uint32_t i = 0;
	array->resize(tot);
	const uint32_t nprop = prop.size();
	ibis::bitvector::indexSet index = mask.firstIndexSet();
	if (nprop >= mask.size()) { // no need to check loop bounds
	    while (index.nIndices() > 0) {
		const ibis::bitvector::word_t *idx0 = index.indices();
		if (index.isRange()) {
		    for (uint32_t j = *idx0; j < idx0[1]; ++j, ++i) {
			(*array)[i] = (prop[j]);
		    }
		}
		else {
		    for (uint32_t j = 0; j<index.nIndices(); ++j, ++i) {
			(*array)[i] = (prop[idx0[j]]);
		    }
		}
		++ index;
	    }
	}
	else { // need to check loop bounds against nprop
	    while (index.nIndices() > 0) {
		const ibis::bitvector::word_t *idx0 = index.indices();
		if (*idx0 >= nprop) break;
		if (index.isRange()) {
		    for (uint32_t j = *idx0;
			 j < (idx0[1]<=nprop ? idx0[1] : nprop);
			 ++j, ++i) {
			(*array)[i] = (prop[j]);
		    }
		}
		else {
		    for (uint32_t j = 0; j<index.nIndices(); ++j, ++i) {
			if (idx0[j] < nprop)
			    (*array)[i] = (prop[idx0[j]]);
			else
			    break;
		    }
		}
		++ index;
	    }
	}
	if (i != tot) {
	    array->resize(i);
	    logWarning("selectULongs", "expects to retrieve %lu elements "
		       "but only got %lu", static_cast<long unsigned>(tot),
		       static_cast<long unsigned>(i));
	}
    }
    else if (m_type == SHORT) {
	const array_t<int16_t> &prop =
	    * static_cast<const array_t<int16_t>*>(buffer);

	uint32_t i = 0;
	array->resize(tot);
	const uint32_t nprop = prop.size();
	ibis::bitvector::indexSet index = mask.firstIndexSet();
	if (nprop >= mask.size()) { // no need to check loop bounds
	    while (index.nIndices() > 0) {
		const ibis::bitvector::word_t *idx0 = index.indices();
		if (index.isRange()) {
		    for (uint32_t j = *idx0; j < idx0[1]; ++j, ++i) {
			(*array)[i] = (prop[j]);
		    }
		}
		else {
		    for (uint32_t j = 0; j<index.nIndices(); ++j, ++i) {
			(*array)[i] = (prop[idx0[j]]);
		    }
		}
		++ index;
	    }
	}
	else { // need to check loop bounds against nprop
	    while (index.nIndices() > 0) {
		const ibis::bitvector::word_t *idx0 = index.indices();
		if (*idx0 >= nprop) break;
		if (index.isRange()) {
		    for (uint32_t j = *idx0;
			 j < (idx0[1]<=nprop ? idx0[1] : nprop);
			 ++j, ++i) {
			(*array)[i] = (prop[j]);
		    }
		}
		else {
		    for (uint32_t j = 0; j<index.nIndices(); ++j, ++i) {
			if (idx0[j] < nprop)
			    (*array)[i] = (prop[idx0[j]]);
			else
			    break;
		    }
		}
		++ index;
	    }
	}
	if (i != tot) {
	    array->resize(i);
	    logWarning("selectULongs", "expects to retrieve %lu elements "
		       "but only got %lu", static_cast<long unsigned>(tot),
		       static_cast<long unsigned>(i));
	}
    }
    else if (m_type == UBYTE) {
	const array_t<unsigned char> &prop =
	    * static_cast<const array_t<unsigned char>*>(buffer);

	uint32_t i = 0;
	array->resize(tot);
	const uint32_t nprop = prop.size();
	ibis::bitvector::indexSet index = mask.firstIndexSet();
	if (nprop >= mask.size()) { // no need to check loop bounds
	    while (index.nIndices() > 0) {
		const ibis::bitvector::word_t *idx0 = index.indices();
		if (index.isRange()) {
		    for (uint32_t j = *idx0; j < idx0[1]; ++j, ++i) {
			(*array)[i] = (prop[j]);
		    }
		}
		else {
		    for (uint32_t j = 0; j<index.nIndices(); ++j, ++i) {
			(*array)[i] = (prop[idx0[j]]);
		    }
		}
		++ index;
	    }
	}
	else { // need to check loop bounds against nprop
	    while (index.nIndices() > 0) {
		const ibis::bitvector::word_t *idx0 = index.indices();
		if (*idx0 >= nprop) break;
		if (index.isRange()) {
		    for (uint32_t j = *idx0;
			 j < (idx0[1]<=nprop ? idx0[1] : nprop);
			 ++j, ++i) {
			(*array)[i] = (prop[j]);
		    }
		}
		else {
		    for (uint32_t j = 0; j<index.nIndices(); ++j, ++i) {
			if (idx0[j] < nprop)
			    (*array)[i] = (prop[idx0[j]]);
			else
			    break;
		    }
		}
		++ index;
	    }
	}
	if (i != tot) {
	    array->resize(i);
	    logWarning("selectULongs", "expects to retrieve %lu elements "
		       "but only got %lu", static_cast<long unsigned>(tot),
		       static_cast<long unsigned>(i));
	}
    }
    else if (m_type == BYTE) {
	const array_t<signed char> &prop =
	    * static_cast<const array_t<signed char>*>(buffer);

	uint32_t i = 0;
	array->resize(tot);
	const uint32_t nprop = prop.size();
	ibis::bitvector::indexSet index = mask.firstIndexSet();
	if (nprop >= mask.size()) { // no need to check loop bounds
	    while (index.nIndices() > 0) {
		const ibis::bitvector::word_t *idx0 = index.indices();
		if (index.isRange()) {
		    for (uint32_t j = *idx0; j < idx0[1]; ++j, ++i) {
			(*array)[i] = (prop[j]);
		    }
		}
		else {
		    for (uint32_t j = 0; j<index.nIndices(); ++j, ++i) {
			(*array)[i] = (prop[idx0[j]]);
		    }
		}
		++ index;
	    }
	}
	else { // need to check loop bounds against nprop
	    while (index.nIndices() > 0) {
		const ibis::bitvector::word_t *idx0 = index.indices();
		if (*idx0 >= nprop) break;
		if (index.isRange()) {
		    for (uint32_t j = *idx0;
			 j < (idx0[1]<=nprop ? idx0[1] : nprop);
			 ++j, ++i) {
			(*array)[i] = (prop[j]);
		    }
		}
		else {
		    for (uint32_t j = 0; j<index.nIndices(); ++j, ++i) {
			if (idx0[j] < nprop)
			    (*array)[i] = (prop[idx0[j]]);
			else
			    break;
		    }
		}
		++ index;
	    }
	}
	if (i != tot) {
	    array->resize(i);
	    logWarning("selectULongs", "expects to retrieve %lu elements "
		       "but only got %lu", static_cast<long unsigned>(tot),
		       static_cast<long unsigned>(i));
	}
    }
    else {
	logWarning("selectULongs", "incompatible data type");
    }
    if (ibis::gVerbose > 5) {
	timer.stop();
	long unsigned cnt = mask.cnt();
	logMessage("selectULongs", "retrieving %lu integer%s "
		   "took %g sec(CPU), %g sec(elapsed)",
		   static_cast<long unsigned>(cnt), (cnt > 1 ? "s" : ""),
		   timer.CPUTime(), timer.realTime());
    }
    return array;
} // ibis::bord::column::selectULongs

/// Put selected values of a float column into an array.
ibis::array_t<float>*
ibis::bord::column::selectFloats(const ibis::bitvector& mask) const {
    ibis::array_t<float>* array = new array_t<float>;
    const uint32_t tot = mask.cnt();
    if (tot == 0)
	return array;
    ibis::horometer timer;
    if (ibis::gVerbose > 5)
	timer.start();

    if (m_type == FLOAT) {
	const array_t<float> &prop =
	    * static_cast<const array_t<float>*>(buffer);

	uint32_t i = 0;
	array->resize(tot);
	const uint32_t nprop = prop.size();
	ibis::bitvector::indexSet index = mask.firstIndexSet();
	if (nprop >= mask.size()) { // no need to check loop bounds
	    while (index.nIndices() > 0) {
		const ibis::bitvector::word_t *idx0 = index.indices();
		if (index.isRange()) {
		    for (uint32_t j = *idx0; j<idx0[1]; ++j, ++i) {
			(*array)[i] = (prop[j]);
		    }
		}
		else {
		    for (uint32_t j = 0; j<index.nIndices(); ++j, ++i) {
			(*array)[i] = (prop[idx0[j]]);
		    }
		}
		++ index;
	    }
	}
	else { // check loop bounds against nprop
	    while (index.nIndices() > 0) {
		const ibis::bitvector::word_t *idx0 = index.indices();
		if (*idx0 >= nprop) break;
		if (index.isRange()) {
		    for (uint32_t j = *idx0;
			 j < (idx0[1]<=nprop ? idx0[1] : nprop);
			 ++j, ++i) {
			(*array)[i] = (prop[j]);
		    }
		}
		else {
		    for (uint32_t j = 0; j<index.nIndices(); ++j, ++i) {
			if (idx0[j] < nprop)
			    (*array)[i] = (prop[idx0[j]]);
			else
			    break;
		    }
		}
		++ index;
	    }
	}

	if (i != tot) {
	    array->resize(i);
	    logWarning("selectFloats", "expects to retrieve %lu elements "
		       "but only got %lu", static_cast<long unsigned>(tot),
		       static_cast<long unsigned>(i));
	}
    }
    else if (m_type == ibis::USHORT) {
	const array_t<uint16_t> &prop =
	    * static_cast<const array_t<uint16_t>*>(buffer);

	uint32_t i = 0;
	array->resize(tot);
	const uint32_t nprop = prop.size();
	ibis::bitvector::indexSet index = mask.firstIndexSet();
	if (nprop >= mask.size()) { // no need to check loop bounds
	    while (index.nIndices() > 0) {
		const ibis::bitvector::word_t *idx0 = index.indices();
		if (index.isRange()) {
		    for (uint32_t j = *idx0; j < idx0[1]; ++j, ++i) {
			(*array)[i] = (prop[j]);
		    }
		}
		else {
		    for (uint32_t j = 0; j<index.nIndices(); ++j, ++i) {
			(*array)[i] = (prop[idx0[j]]);
		    }
		}
		++ index;
	    }
	}
	else { // need to check loop bounds against nprop
	    while (index.nIndices() > 0) {
		const ibis::bitvector::word_t *idx0 = index.indices();
		if (*idx0 >= nprop) break;
		if (index.isRange()) {
		    for (uint32_t j = *idx0;
			 j < (idx0[1]<=nprop ? idx0[1] : nprop);
			 ++j, ++i) {
			(*array)[i] = (prop[j]);
		    }
		}
		else {
		    for (uint32_t j = 0; j<index.nIndices(); ++j, ++i) {
			if (idx0[j] < nprop)
			    (*array)[i] = (prop[idx0[j]]);
			else
			    break;
		    }
		}
		++ index;
	    }
	}
	if (i != tot) {
	    array->resize(i);
	    logWarning("selectFloats", "expects to retrieve %lu elements "
		       "but only got %lu", static_cast<long unsigned>(tot),
		       static_cast<long unsigned>(i));
	}
    }
    else if (m_type == SHORT) {
	const array_t<int16_t> &prop =
	    * static_cast<const array_t<int16_t>*>(buffer);

	uint32_t i = 0;
	array->resize(tot);
	const uint32_t nprop = prop.size();
	ibis::bitvector::indexSet index = mask.firstIndexSet();
	if (nprop >= mask.size()) { // no need to check loop bounds
	    while (index.nIndices() > 0) {
		const ibis::bitvector::word_t *idx0 = index.indices();
		if (index.isRange()) {
		    for (uint32_t j = *idx0; j < idx0[1]; ++j, ++i) {
			(*array)[i] = (prop[j]);
		    }
		}
		else {
		    for (uint32_t j = 0; j<index.nIndices(); ++j, ++i) {
			(*array)[i] = (prop[idx0[j]]);
		    }
		}
		++ index;
	    }
	}
	else { // need to check loop bounds against nprop
	    while (index.nIndices() > 0) {
		const ibis::bitvector::word_t *idx0 = index.indices();
		if (*idx0 >= nprop) break;
		if (index.isRange()) {
		    for (uint32_t j = *idx0;
			 j < (idx0[1]<=nprop ? idx0[1] : nprop);
			 ++j, ++i) {
			(*array)[i] = (prop[j]);
		    }
		}
		else {
		    for (uint32_t j = 0; j<index.nIndices(); ++j, ++i) {
			if (idx0[j] < nprop)
			    (*array)[i] = (prop[idx0[j]]);
			else
			    break;
		    }
		}
		++ index;
	    }
	}
	if (i != tot) {
	    array->resize(i);
	    logWarning("selectFloats", "expects to retrieve %lu elements "
		       "but only got %lu", static_cast<long unsigned>(tot),
		       static_cast<long unsigned>(i));
	}
    }
    else if (m_type == UBYTE) {
	const array_t<unsigned char> &prop =
	    * static_cast<const array_t<unsigned char>*>(buffer);

	uint32_t i = 0;
	array->resize(tot);
	const uint32_t nprop = prop.size();
	ibis::bitvector::indexSet index = mask.firstIndexSet();
	if (nprop >= mask.size()) { // no need to check loop bounds
	    while (index.nIndices() > 0) {
		const ibis::bitvector::word_t *idx0 = index.indices();
		if (index.isRange()) {
		    for (uint32_t j = *idx0; j < idx0[1]; ++j, ++i) {
			(*array)[i] = (prop[j]);
		    }
		}
		else {
		    for (uint32_t j = 0; j<index.nIndices(); ++j, ++i) {
			(*array)[i] = (prop[idx0[j]]);
		    }
		}
		++ index;
	    }
	}
	else { // need to check loop bounds against nprop
	    while (index.nIndices() > 0) {
		const ibis::bitvector::word_t *idx0 = index.indices();
		if (*idx0 >= nprop) break;
		if (index.isRange()) {
		    for (uint32_t j = *idx0;
			 j < (idx0[1]<=nprop ? idx0[1] : nprop);
			 ++j, ++i) {
			(*array)[i] = (prop[j]);
		    }
		}
		else {
		    for (uint32_t j = 0; j<index.nIndices(); ++j, ++i) {
			if (idx0[j] < nprop)
			    (*array)[i] = (prop[idx0[j]]);
			else
			    break;
		    }
		}
		++ index;
	    }
	}
	if (i != tot) {
	    array->resize(i);
	    logWarning("selectFloats", "expects to retrieve %lu elements "
		       "but only got %lu", static_cast<long unsigned>(tot),
		       static_cast<long unsigned>(i));
	}
    }
    else if (m_type == BYTE) {
	const array_t<signed char> &prop =
	    * static_cast<const array_t<signed char>*>(buffer);

	uint32_t i = 0;
	array->resize(tot);
	const uint32_t nprop = prop.size();
	ibis::bitvector::indexSet index = mask.firstIndexSet();
	if (nprop >= mask.size()) { // no need to check loop bounds
	    while (index.nIndices() > 0) {
		const ibis::bitvector::word_t *idx0 = index.indices();
		if (index.isRange()) {
		    for (uint32_t j = *idx0; j < idx0[1]; ++j, ++i) {
			(*array)[i] = (prop[j]);
		    }
		}
		else {
		    for (uint32_t j = 0; j<index.nIndices(); ++j, ++i) {
			(*array)[i] = (prop[idx0[j]]);
		    }
		}
		++ index;
	    }
	}
	else { // need to check loop bounds against nprop
	    while (index.nIndices() > 0) {
		const ibis::bitvector::word_t *idx0 = index.indices();
		if (*idx0 >= nprop) break;
		if (index.isRange()) {
		    for (uint32_t j = *idx0;
			 j < (idx0[1]<=nprop ? idx0[1] : nprop);
			 ++j, ++i) {
			(*array)[i] = (prop[j]);
		    }
		}
		else {
		    for (uint32_t j = 0; j<index.nIndices(); ++j, ++i) {
			if (idx0[j] < nprop)
			    (*array)[i] = (prop[idx0[j]]);
			else
			    break;
		    }
		}
		++ index;
	    }
	}
	if (i != tot) {
	    array->resize(i);
	    logWarning("selectFloats", "expects to retrieve %lu elements "
		       "but only got %lu", static_cast<long unsigned>(tot),
		       static_cast<long unsigned>(i));
	}
    }
    else {
	logWarning("selectFloats", "incompatible data type");
    }
    if (ibis::gVerbose > 5) {
	timer.stop();
	long unsigned cnt = mask.cnt();
	logMessage("selectFloats", "retrieving %lu float value%s "
		   "took %g sec(CPU), %g sec(elapsed)",
		   static_cast<long unsigned>(cnt), (cnt > 1 ? "s" : ""),
		   timer.CPUTime(), timer.realTime());
    }
    return array;
} // ibis::bord::column::selectFloats

/// Put the selected values into an array as doubles.
/// @note Any column type could be selected as doubles.  Other selectXXs
/// function only work on the same data type.  This is the only function
/// that allows one to convert to a different type.  This is mainly to 
ibis::array_t<double>*
ibis::bord::column::selectDoubles(const ibis::bitvector& mask) const {
    ibis::array_t<double>* array = new array_t<double>;
    const uint32_t tot = mask.cnt();
    if (tot == 0)
	return array;
    ibis::horometer timer;
    if (ibis::gVerbose > 5)
	timer.start();

    switch(m_type) {
    case ibis::CATEGORY:
    case ibis::UINT: {
	const array_t<uint32_t> &prop =
	    * static_cast<const array_t<uint32_t>*>(buffer);

	uint32_t i = 0;
	array->resize(tot);
	const uint32_t nprop = prop.size();
	ibis::bitvector::indexSet index = mask.firstIndexSet();
	if (nprop >= mask.size()) {
	    while (index.nIndices() > 0) {
		const ibis::bitvector::word_t *idx0 = index.indices();
		if (index.isRange()) {
		    for (uint32_t j = *idx0; j<idx0[1]; ++j, ++i) {
			(*array)[i] = (prop[j]);
		    }
		}
		else {
		    for (uint32_t j = 0; j<index.nIndices(); ++j, ++i) {
			(*array)[i] = (prop[idx0[j]]);
		    }
		}
		++ index;
	    }
	}
	else {
	    while (index.nIndices() > 0) {
		const ibis::bitvector::word_t *idx0 = index.indices();
		if (*idx0 >= nprop) break;
		if (index.isRange()) {
		    for (uint32_t j = *idx0;
			 j<(idx0[1]<=nprop ? idx0[1] : nprop);
			 ++j, ++i) {
			(*array)[i] = (prop[j]);
		    }
		}
		else {
		    for (uint32_t j = 0; j<index.nIndices(); ++j, ++i) {
			if (idx0[j] < nprop)
			    (*array)[i] = (prop[idx0[j]]);
			else
			    break;
		    }
		}
		++ index;
	    }
	}

	if (i != tot) {
	    array->resize(i);
	    logWarning("selectDoubles", "expects to retrieve %lu elements "
		       "but only got %lu", static_cast<long unsigned>(tot),
		       static_cast<long unsigned>(i));
	}
	else if (ibis::gVerbose > 5) {
	    timer.stop();
	    long unsigned cnt = mask.cnt();
	    logMessage("selectDoubles", "retrieving %lu unsigned integer%s "
		       "took %g sec(CPU), %g sec(elapsed)",
		       static_cast<long unsigned>(cnt), (cnt > 1 ? "s" : ""),
		       timer.CPUTime(), timer.realTime());
	}
	break;}
    case ibis::INT: {
	const array_t<int32_t> &prop =
	    * static_cast<const array_t<int32_t>*>(buffer);

	uint32_t i = 0;
	array->resize(tot);
	const uint32_t nprop = prop.size();
	ibis::bitvector::indexSet index = mask.firstIndexSet();
	if (nprop >= mask.size()) {
	    while (index.nIndices() > 0) {
		const ibis::bitvector::word_t *idx0 = index.indices();
		if (index.isRange()) {
		    for (uint32_t j = *idx0; j<idx0[1]; ++j, ++i) {
			(*array)[i] = (prop[j]);
		    }
		}
		else {
		    for (uint32_t j = 0; j<index.nIndices(); ++j, ++i) {
			(*array)[i] = (prop[idx0[j]]);
		    }
		}
		++ index;
	    }
	}
	else {
	    while (index.nIndices() > 0) {
		const ibis::bitvector::word_t *idx0 = index.indices();
		if (*idx0 >= nprop) break;
		if (index.isRange()) {
		    for (uint32_t j = *idx0;
			 j<(idx0[1]<=nprop ? idx0[1] : nprop);
			 ++j, ++i) {
			(*array)[i] = (prop[j]);
		    }
		}
		else {
		    for (uint32_t j = 0; j<index.nIndices(); ++j, ++i) {
			if (idx0[j] < nprop)
			    (*array)[i] = (prop[idx0[j]]);
			else
			    break;
		    }
		}
		++ index;
	    }
	}

	if (i != tot) {
	    array->resize(i);
	    logWarning("selectDoubles", "expects to retrieve %lu elements "
		       "but only got %lu", static_cast<long unsigned>(tot),
		       static_cast<long unsigned>(i));
	}
	else if (ibis::gVerbose > 5) {
	    timer.stop();
	    long unsigned cnt = mask.cnt();
	    logMessage("selectDoubles", "retrieving %lu integer%s "
		       "took %g sec(CPU), %g sec(elapsed)",
		       static_cast<long unsigned>(cnt), (cnt > 1 ? "s" : ""),
		       timer.CPUTime(), timer.realTime());
	}
	break;}
    case ibis::USHORT: {
	const array_t<uint16_t> &prop =
	    * static_cast<const array_t<uint16_t>*>(buffer);

	uint32_t i = 0;
	array->resize(tot);
	const uint32_t nprop = prop.size();
	ibis::bitvector::indexSet index = mask.firstIndexSet();
	if (nprop >= mask.size()) {
	    while (index.nIndices() > 0) {
		const ibis::bitvector::word_t *idx0 = index.indices();
		if (index.isRange()) {
		    for (uint32_t j = *idx0; j<idx0[1]; ++j, ++i) {
			(*array)[i] = (prop[j]);
		    }
		}
		else {
		    for (uint32_t j = 0; j<index.nIndices(); ++j, ++i) {
			(*array)[i] = (prop[idx0[j]]);
		    }
		}
		++ index;
	    }
	}
	else {
	    while (index.nIndices() > 0) {
		const ibis::bitvector::word_t *idx0 = index.indices();
		if (*idx0 >= nprop) break;
		if (index.isRange()) {
		    for (uint32_t j = *idx0;
			 j<(idx0[1]<=nprop ? idx0[1] : nprop);
			 ++j, ++i) {
			(*array)[i] = (prop[j]);
		    }
		}
		else {
		    for (uint32_t j = 0; j<index.nIndices(); ++j, ++i) {
			if (idx0[j] < nprop)
			    (*array)[i] = (prop[idx0[j]]);
			else
			    break;
		    }
		}
		++ index;
	    }
	}

	if (i != tot) {
	    array->resize(i);
	    logWarning("selectDoubles", "expects to retrieve %lu elements "
		       "but only got %lu", static_cast<long unsigned>(tot),
		       static_cast<long unsigned>(i));
	}
	else if (ibis::gVerbose > 5) {
	    timer.stop();
	    long unsigned cnt = mask.cnt();
	    logMessage("selectDoubles", "retrieving %lu unsigned short "
		       "integer%s took %g sec(CPU), %g sec(elapsed)",
		       static_cast<long unsigned>(cnt), (cnt > 1 ? "s" : ""),
		       timer.CPUTime(), timer.realTime());
	}
	break;}
    case ibis::SHORT: {
	const array_t<int16_t> &prop =
	    * static_cast<const array_t<int16_t>*>(buffer);

	uint32_t i = 0;
	array->resize(tot);
	const uint32_t nprop = prop.size();
	ibis::bitvector::indexSet index = mask.firstIndexSet();
	if (nprop >= mask.size()) {
	    while (index.nIndices() > 0) {
		const ibis::bitvector::word_t *idx0 = index.indices();
		if (index.isRange()) {
		    for (uint32_t j = *idx0; j<idx0[1]; ++j, ++i) {
			(*array)[i] = (prop[j]);
		    }
		}
		else {
		    for (uint32_t j = 0; j<index.nIndices(); ++j, ++i) {
			(*array)[i] = (prop[idx0[j]]);
		    }
		}
		++ index;
	    }
	}
	else {
	    while (index.nIndices() > 0) {
		const ibis::bitvector::word_t *idx0 = index.indices();
		if (*idx0 >= nprop) break;
		if (index.isRange()) {
		    for (uint32_t j = *idx0;
			 j<(idx0[1]<=nprop ? idx0[1] : nprop);
			 ++j, ++i) {
			(*array)[i] = (prop[j]);
		    }
		}
		else {
		    for (uint32_t j = 0; j<index.nIndices(); ++j, ++i) {
			if (idx0[j] < nprop)
			    (*array)[i] = (prop[idx0[j]]);
			else
			    break;
		    }
		}
		++ index;
	    }
	}

	if (i != tot) {
	    array->resize(i);
	    logWarning("selectDoubles", "expects to retrieve %lu elements "
		       "but only got %lu", static_cast<long unsigned>(tot),
		       static_cast<long unsigned>(i));
	}
	else if (ibis::gVerbose > 5) {
	    timer.stop();
	    long unsigned cnt = mask.cnt();
	    logMessage("selectDoubles", "retrieving %lu short integer%s "
		       "took %g sec(CPU), %g sec(elapsed)",
		       static_cast<long unsigned>(cnt), (cnt > 1 ? "s" : ""),
		       timer.CPUTime(), timer.realTime());
	}
	break;}
    case ibis::UBYTE: {
	const array_t<unsigned char> &prop =
	    * static_cast<const array_t<unsigned char>*>(buffer);

	uint32_t i = 0;
	array->resize(tot);
	const uint32_t nprop = prop.size();
	ibis::bitvector::indexSet index = mask.firstIndexSet();
	if (nprop >= mask.size()) {
	    while (index.nIndices() > 0) {
		const ibis::bitvector::word_t *idx0 = index.indices();
		if (index.isRange()) {
		    for (uint32_t j = *idx0; j<idx0[1]; ++j, ++i) {
			(*array)[i] = (prop[j]);
		    }
		}
		else {
		    for (uint32_t j = 0; j<index.nIndices(); ++j, ++i) {
			(*array)[i] = (prop[idx0[j]]);
		    }
		}
		++ index;
	    }
	}
	else {
	    while (index.nIndices() > 0) {
		const ibis::bitvector::word_t *idx0 = index.indices();
		if (*idx0 >= nprop) break;
		if (index.isRange()) {
		    for (uint32_t j = *idx0;
			 j<(idx0[1]<=nprop ? idx0[1] : nprop);
			 ++j, ++i) {
			(*array)[i] = (prop[j]);
		    }
		}
		else {
		    for (uint32_t j = 0; j<index.nIndices(); ++j, ++i) {
			if (idx0[j] < nprop)
			    (*array)[i] = (prop[idx0[j]]);
			else
			    break;
		    }
		}
		++ index;
	    }
	}

	if (i != tot) {
	    array->resize(i);
	    logWarning("selectDoubles", "expects to retrieve %lu elements "
		       "but only got %lu", static_cast<long unsigned>(tot),
		       static_cast<long unsigned>(i));
	}
	else if (ibis::gVerbose > 5) {
	    timer.stop();
	    long unsigned cnt = mask.cnt();
	    logMessage("selectDoubles", "retrieving %lu unsigned 1-byte "
		       "integer%s took %g sec(CPU), %g sec(elapsed)",
		       static_cast<long unsigned>(cnt), (cnt > 1 ? "s" : ""),
		       timer.CPUTime(), timer.realTime());
	}
	break;}
    case ibis::BYTE: {
	const array_t<signed char> &prop =
	    * static_cast<const array_t<signed char>*>(buffer);

	uint32_t i = 0;
	array->resize(tot);
	const uint32_t nprop = prop.size();
	ibis::bitvector::indexSet index = mask.firstIndexSet();
	if (nprop >= mask.size()) {
	    while (index.nIndices() > 0) {
		const ibis::bitvector::word_t *idx0 = index.indices();
		if (index.isRange()) {
		    for (uint32_t j = *idx0; j<idx0[1]; ++j, ++i) {
			(*array)[i] = (prop[j]);
		    }
		}
		else {
		    for (uint32_t j = 0; j<index.nIndices(); ++j, ++i) {
			(*array)[i] = (prop[idx0[j]]);
		    }
		}
		++ index;
	    }
	}
	else {
	    while (index.nIndices() > 0) {
		const ibis::bitvector::word_t *idx0 = index.indices();
		if (*idx0 >= nprop) break;
		if (index.isRange()) {
		    for (uint32_t j = *idx0;
			 j<(idx0[1]<=nprop ? idx0[1] : nprop);
			 ++j, ++i) {
			(*array)[i] = (prop[j]);
		    }
		}
		else {
		    for (uint32_t j = 0; j<index.nIndices(); ++j, ++i) {
			if (idx0[j] < nprop)
			    (*array)[i] = (prop[idx0[j]]);
			else
			    break;
		    }
		}
		++ index;
	    }
	}

	if (i != tot) {
	    array->resize(i);
	    logWarning("selectDoubles", "expects to retrieve %lu elements "
		       "but only got %lu", static_cast<long unsigned>(tot),
		       static_cast<long unsigned>(i));
	}
	else if (ibis::gVerbose > 5) {
	    timer.stop();
	    long unsigned cnt = mask.cnt();
	    logMessage("selectDoubles", "retrieving %lu 1-byte integer%s "
		       "took %g sec(CPU), %g sec(elapsed)",
		       static_cast<long unsigned>(cnt), (cnt > 1 ? "s" : ""),
		       timer.CPUTime(), timer.realTime());
	}
	break;}
    case ibis::FLOAT: {
	const array_t<float> &prop =
	    * static_cast<const array_t<float>*>(buffer);

	uint32_t i = 0;
	array->resize(tot);
	const uint32_t nprop = prop.size();
	ibis::bitvector::indexSet index = mask.firstIndexSet();
	if (nprop >= mask.size()) {
	    while (index.nIndices() > 0) {
		const ibis::bitvector::word_t *idx0 = index.indices();
		if (index.isRange()) {
		    for (uint32_t j = *idx0; j<idx0[1]; ++j, ++i) {
			(*array)[i] = (prop[j]);
		    }
		}
		else {
		    for (uint32_t j = 0; j<index.nIndices(); ++j, ++i) {
			(*array)[i] = (prop[idx0[j]]);
		    }
		}
		++ index;
	    }
	}
	else {
	    while (index.nIndices() > 0) {
		const ibis::bitvector::word_t *idx0 = index.indices();
		if (*idx0 >= nprop) break;
		if (index.isRange()) {
		    for (uint32_t j = *idx0;
			 j<(idx0[1]<=nprop ? idx0[1] : nprop);
			 ++j, ++i) {
			(*array)[i] = (prop[j]);
		    }
		}
		else {
		    for (uint32_t j = 0; j<index.nIndices(); ++j, ++i) {
			if (idx0[j] < nprop)
			    (*array)[i] = (prop[idx0[j]]);
			else
			    break;
		    }
		}
		++ index;
	    }
	}

	if (i != tot) {
	    array->resize(i);
	    logWarning("selectDoubles", "expects to retrieve %lu elements "
		       "but only got %lu", static_cast<long unsigned>(tot),
		       static_cast<long unsigned>(i));
	}
	else if (ibis::gVerbose > 5) {
	    timer.stop();
	    long unsigned cnt = mask.cnt();
	    logMessage("selectDoubles", "retrieving %lu float value%s "
		       "took %g sec(CPU), %g sec(elapsed)",
		       static_cast<long unsigned>(cnt), (cnt > 1 ? "s" : ""),
		       timer.CPUTime(), timer.realTime());
	}
	break;}
    case ibis::DOUBLE: {
	const array_t<double> &prop =
	    * static_cast<const array_t<double>*>(buffer);

	uint32_t i = 0;
	array->resize(tot);
	const uint32_t nprop = prop.size();
	ibis::bitvector::indexSet index = mask.firstIndexSet();
	if (nprop >= mask.size()) {
	    while (index.nIndices() > 0) {
		const ibis::bitvector::word_t *idx0 = index.indices();
		if (index.isRange()) {
		    for (uint32_t j = *idx0; j<idx0[1]; ++j, ++i) {
			(*array)[i] = (prop[j]);
		    }
		}
		else {
		    for (uint32_t j = 0; j<index.nIndices(); ++j, ++i) {
			(*array)[i] = (prop[idx0[j]]);
		    }
		}
		++ index;
	    }
	}
	else {
	    while (index.nIndices() > 0) {
		const ibis::bitvector::word_t *idx0 = index.indices();
		if (*idx0 >= nprop) break;
		if (index.isRange()) {
		    for (uint32_t j = *idx0;
			 j<(idx0[1]<=nprop ? idx0[1] : nprop);
			 ++j, ++i) {
			(*array)[i] = (prop[j]);
		    }
		}
		else {
		    for (uint32_t j = 0; j<index.nIndices(); ++j, ++i) {
			if (idx0[j] < nprop)
			    (*array)[i] = (prop[idx0[j]]);
			else
			    break;
		    }
		}
		++ index;
	    }
	}

	if (i != tot) {
	    array->resize(i);
	    logWarning("selectDoubles", "expects to retrieve %lu elements "
		       "but only got %lu", static_cast<long unsigned>(tot),
		       static_cast<long unsigned>(i));
	}
	else if (ibis::gVerbose > 5) {
	    timer.stop();
	    long unsigned cnt = mask.cnt();
	    logMessage("selectDoubles", "retrieving %lu double value%s "
		       "took %g sec(CPU), %g sec(elapsed)",
		       static_cast<long unsigned>(cnt), (cnt > 1 ? "s" : ""),
		       timer.CPUTime(), timer.realTime());
	}
	break;}
    default: {
	logWarning("selectDoubles", "incompatible data type");
	break;}
    }
    return array;
} // ibis::bord::column::selectDoubles

/// Output the selected values as strings.  Most data types can be
/// converted and shown as strings.
std::vector<std::string>*
ibis::bord::column::selectStrings(const ibis::bitvector& mask) const {
    std::vector<std::string>* array = new std::vector<std::string>;
    const uint32_t tot = mask.cnt();
    if (tot == 0)
	return array;
    ibis::horometer timer;
    if (ibis::gVerbose > 5)
	timer.start();

    switch(m_type) {
    case ibis::UINT: {
	const array_t<uint32_t> &prop =
	    * static_cast<const array_t<uint32_t>*>(buffer);

	uint32_t i = 0;
	array->resize(tot);
	const uint32_t nprop = prop.size();
	ibis::bitvector::indexSet index = mask.firstIndexSet();
	if (nprop >= mask.size()) {
	    while (index.nIndices() > 0) {
		const ibis::bitvector::word_t *idx0 = index.indices();
		if (index.isRange()) {
		    for (uint32_t j = *idx0; j<idx0[1]; ++j, ++i) {
			std::ostringstream oss;
			oss << prop[j];
			(*array)[i] = oss.str();
		    }
		}
		else {
		    for (uint32_t j = 0; j<index.nIndices(); ++j, ++i) {
			std::ostringstream oss;
			oss << (prop[idx0[j]]);
			(*array)[i] = oss.str();
		    }
		}
		++ index;
	    }
	}
	else {
	    while (index.nIndices() > 0) {
		const ibis::bitvector::word_t *idx0 = index.indices();
		if (*idx0 >= nprop) break;
		if (index.isRange()) {
		    for (uint32_t j = *idx0;
			 j<(idx0[1]<=nprop ? idx0[1] : nprop);
			 ++j, ++i) {
			std::ostringstream oss;
			oss << (prop[j]);
			(*array)[i] = oss.str();
		    }
		}
		else {
		    for (uint32_t j = 0; j<index.nIndices(); ++j, ++i) {
			if (idx0[j] < nprop) {
			    std::ostringstream oss;
			    oss << (prop[idx0[j]]);
			    (*array)[i] = oss.str();
			}
			else
			    break;
		    }
		}
		++ index;
	    }
	}

	if (i != tot) {
	    array->resize(i);
	    logWarning("selectStrings", "expects to retrieve %lu elements "
		       "but only got %lu", static_cast<long unsigned>(tot),
		       static_cast<long unsigned>(i));
	}
	else if (ibis::gVerbose > 5) {
	    timer.stop();
	    long unsigned cnt = mask.cnt();
	    logMessage("selectStrings", "retrieving %lu unsigned integer%s "
		       "took %g sec(CPU), %g sec(elapsed)",
		       static_cast<long unsigned>(cnt), (cnt > 1 ? "s" : ""),
		       timer.CPUTime(), timer.realTime());
	}
	break;}
    case ibis::INT: {
	const array_t<int32_t> &prop =
	    * static_cast<const array_t<int32_t>*>(buffer);

	uint32_t i = 0;
	array->resize(tot);
	const uint32_t nprop = prop.size();
	ibis::bitvector::indexSet index = mask.firstIndexSet();
	if (nprop >= mask.size()) {
	    while (index.nIndices() > 0) {
		const ibis::bitvector::word_t *idx0 = index.indices();
		if (index.isRange()) {
		    for (uint32_t j = *idx0; j<idx0[1]; ++j, ++i) {
			std::ostringstream oss;
			oss << (prop[j]);
			(*array)[i] = oss.str();
		    }
		}
		else {
		    for (uint32_t j = 0; j<index.nIndices(); ++j, ++i) {
			std::ostringstream oss;
			oss << (prop[idx0[j]]);
			(*array)[i] = oss.str();
		    }
		}
		++ index;
	    }
	}
	else {
	    while (index.nIndices() > 0) {
		const ibis::bitvector::word_t *idx0 = index.indices();
		if (*idx0 >= nprop) break;
		if (index.isRange()) {
		    for (uint32_t j = *idx0;
			 j<(idx0[1]<=nprop ? idx0[1] : nprop);
			 ++j, ++i) {
			std::ostringstream oss;
			oss << (prop[j]);
			(*array)[i] = oss.str();
		    }
		}
		else {
		    for (uint32_t j = 0; j<index.nIndices(); ++j, ++i) {
			if (idx0[j] < nprop) {
			    std::ostringstream oss;
			    oss << (prop[idx0[j]]);
			    (*array)[i] = oss.str();
			}
			else
			    break;
		    }
		}
		++ index;
	    }
	}

	if (i != tot) {
	    array->resize(i);
	    logWarning("selectStrings", "expects to retrieve %lu elements "
		       "but only got %lu", static_cast<long unsigned>(tot),
		       static_cast<long unsigned>(i));
	}
	else if (ibis::gVerbose > 5) {
	    timer.stop();
	    long unsigned cnt = mask.cnt();
	    logMessage("selectStrings", "retrieving %lu integer%s "
		       "took %g sec(CPU), %g sec(elapsed)",
		       static_cast<long unsigned>(cnt), (cnt > 1 ? "s" : ""),
		       timer.CPUTime(), timer.realTime());
	}
	break;}
    case ibis::USHORT: {
	const array_t<uint16_t> &prop =
	    * static_cast<const array_t<uint16_t>*>(buffer);

	uint32_t i = 0;
	array->resize(tot);
	const uint32_t nprop = prop.size();
	ibis::bitvector::indexSet index = mask.firstIndexSet();
	if (nprop >= mask.size()) {
	    while (index.nIndices() > 0) {
		const ibis::bitvector::word_t *idx0 = index.indices();
		if (index.isRange()) {
		    for (uint32_t j = *idx0; j<idx0[1]; ++j, ++i) {
			std::ostringstream oss;
			oss << (prop[j]);
			(*array)[i] = oss.str();
		    }
		}
		else {
		    for (uint32_t j = 0; j<index.nIndices(); ++j, ++i) {
			std::ostringstream oss;
			oss << (prop[idx0[j]]);
			(*array)[i] = oss.str();
		    }
		}
		++ index;
	    }
	}
	else {
	    while (index.nIndices() > 0) {
		const ibis::bitvector::word_t *idx0 = index.indices();
		if (*idx0 >= nprop) break;
		if (index.isRange()) {
		    for (uint32_t j = *idx0;
			 j<(idx0[1]<=nprop ? idx0[1] : nprop);
			 ++j, ++i) {
			std::ostringstream oss;
			oss << (prop[j]);
			(*array)[i] = oss.str();
		    }
		}
		else {
		    for (uint32_t j = 0; j<index.nIndices(); ++j, ++i) {
			if (idx0[j] < nprop) {
			    std::ostringstream oss;
			    oss << (prop[idx0[j]]);
			    (*array)[i] = oss.str();
			}
			else
			    break;
		    }
		}
		++ index;
	    }
	}

	if (i != tot) {
	    array->resize(i);
	    logWarning("selectStrings", "expects to retrieve %lu elements "
		       "but only got %lu", static_cast<long unsigned>(tot),
		       static_cast<long unsigned>(i));
	}
	else if (ibis::gVerbose > 5) {
	    timer.stop();
	    long unsigned cnt = mask.cnt();
	    logMessage("selectStrings", "retrieving %lu unsigned short "
		       "integer%s took %g sec(CPU), %g sec(elapsed)",
		       static_cast<long unsigned>(cnt), (cnt > 1 ? "s" : ""),
		       timer.CPUTime(), timer.realTime());
	}
	break;}
    case ibis::SHORT: {
	const array_t<int16_t> &prop =
	    * static_cast<const array_t<int16_t>*>(buffer);

	uint32_t i = 0;
	array->resize(tot);
	const uint32_t nprop = prop.size();
	ibis::bitvector::indexSet index = mask.firstIndexSet();
	if (nprop >= mask.size()) {
	    while (index.nIndices() > 0) {
		const ibis::bitvector::word_t *idx0 = index.indices();
		if (index.isRange()) {
		    for (uint32_t j = *idx0; j<idx0[1]; ++j, ++i) {
			std::ostringstream oss;
			oss << (prop[j]);
			(*array)[i] = oss.str();
		    }
		}
		else {
		    for (uint32_t j = 0; j<index.nIndices(); ++j, ++i) {
			std::ostringstream oss;
			oss << (prop[idx0[j]]);
			(*array)[i] = oss.str();
		    }
		}
		++ index;
	    }
	}
	else {
	    while (index.nIndices() > 0) {
		const ibis::bitvector::word_t *idx0 = index.indices();
		if (*idx0 >= nprop) break;
		if (index.isRange()) {
		    for (uint32_t j = *idx0;
			 j<(idx0[1]<=nprop ? idx0[1] : nprop);
			 ++j, ++i) {
			std::ostringstream oss;
			oss << (prop[j]);
			(*array)[i] = oss.str();
		    }
		}
		else {
		    for (uint32_t j = 0; j<index.nIndices(); ++j, ++i) {
			if (idx0[j] < nprop) {
			    std::ostringstream oss;
			    oss << (prop[idx0[j]]);
			    (*array)[i] = oss.str();
			}
			else
			    break;
		    }
		}
		++ index;
	    }
	}

	if (i != tot) {
	    array->resize(i);
	    logWarning("selectStrings", "expects to retrieve %lu elements "
		       "but only got %lu", static_cast<long unsigned>(tot),
		       static_cast<long unsigned>(i));
	}
	else if (ibis::gVerbose > 5) {
	    timer.stop();
	    long unsigned cnt = mask.cnt();
	    logMessage("selectStrings", "retrieving %lu short integer%s "
		       "took %g sec(CPU), %g sec(elapsed)",
		       static_cast<long unsigned>(cnt), (cnt > 1 ? "s" : ""),
		       timer.CPUTime(), timer.realTime());
	}
	break;}
    case ibis::UBYTE: {
	const array_t<unsigned char> &prop =
	    * static_cast<const array_t<unsigned char>*>(buffer);

	uint32_t i = 0;
	array->resize(tot);
	const uint32_t nprop = prop.size();
	ibis::bitvector::indexSet index = mask.firstIndexSet();
	if (nprop >= mask.size()) {
	    while (index.nIndices() > 0) {
		const ibis::bitvector::word_t *idx0 = index.indices();
		if (index.isRange()) {
		    for (uint32_t j = *idx0; j<idx0[1]; ++j, ++i) {
			std::ostringstream oss;
			oss << (unsigned) (prop[j]);
			(*array)[i] = oss.str();
		    }
		}
		else {
		    for (uint32_t j = 0; j<index.nIndices(); ++j, ++i) {
			std::ostringstream oss;
			oss << (unsigned)(prop[idx0[j]]);
			(*array)[i] = oss.str();
		    }
		}
		++ index;
	    }
	}
	else {
	    while (index.nIndices() > 0) {
		const ibis::bitvector::word_t *idx0 = index.indices();
		if (*idx0 >= nprop) break;
		if (index.isRange()) {
		    for (uint32_t j = *idx0;
			 j<(idx0[1]<=nprop ? idx0[1] : nprop);
			 ++j, ++i) {
			std::ostringstream oss;
			oss << (unsigned) (prop[j]);
			(*array)[i] = oss.str();
		    }
		}
		else {
		    for (uint32_t j = 0; j<index.nIndices(); ++j, ++i) {
			if (idx0[j] < nprop) {
			    std::ostringstream oss;
			    oss << (unsigned)(prop[idx0[j]]);
			    (*array)[i] = oss.str();
			}
			else
			    break;
		    }
		}
		++ index;
	    }
	}

	if (i != tot) {
	    array->resize(i);
	    logWarning("selectStrings", "expects to retrieve %lu elements "
		       "but only got %lu", static_cast<long unsigned>(tot),
		       static_cast<long unsigned>(i));
	}
	else if (ibis::gVerbose > 5) {
	    timer.stop();
	    long unsigned cnt = mask.cnt();
	    logMessage("selectStrings", "retrieving %lu unsigned 1-byte "
		       "integer%s took %g sec(CPU), %g sec(elapsed)",
		       static_cast<long unsigned>(cnt), (cnt > 1 ? "s" : ""),
		       timer.CPUTime(), timer.realTime());
	}
	break;}
    case ibis::BYTE: {
	const array_t<signed char> &prop =
	    * static_cast<const array_t<signed char>*>(buffer);

	uint32_t i = 0;
	array->resize(tot);
	const uint32_t nprop = prop.size();
	ibis::bitvector::indexSet index = mask.firstIndexSet();
	if (nprop >= mask.size()) {
	    while (index.nIndices() > 0) {
		const ibis::bitvector::word_t *idx0 = index.indices();
		if (index.isRange()) {
		    for (uint32_t j = *idx0; j<idx0[1]; ++j, ++i) {
			std::ostringstream oss;
			oss << (int) (prop[j]);
			(*array)[i] = oss.str();
		    }
		}
		else {
		    for (uint32_t j = 0; j<index.nIndices(); ++j, ++i) {
			std::ostringstream oss;
			oss << (int) (prop[idx0[j]]);
			(*array)[i] = oss.str();
		    }
		}
		++ index;
	    }
	}
	else {
	    while (index.nIndices() > 0) {
		const ibis::bitvector::word_t *idx0 = index.indices();
		if (*idx0 >= nprop) break;
		if (index.isRange()) {
		    for (uint32_t j = *idx0;
			 j<(idx0[1]<=nprop ? idx0[1] : nprop);
			 ++j, ++i) {
			std::ostringstream oss;
			oss << (int) (prop[j]);
			(*array)[i] = oss.str();
		    }
		}
		else {
		    for (uint32_t j = 0; j<index.nIndices(); ++j, ++i) {
			if (idx0[j] < nprop) {
			    std::ostringstream oss;
			    oss << (int) (prop[idx0[j]]);
			    (*array)[i] = oss.str();
			}
			else
			    break;
		    }
		}
		++ index;
	    }
	}

	if (i != tot) {
	    array->resize(i);
	    logWarning("selectStrings", "expects to retrieve %lu elements "
		       "but only got %lu", static_cast<long unsigned>(tot),
		       static_cast<long unsigned>(i));
	}
	else if (ibis::gVerbose > 5) {
	    timer.stop();
	    long unsigned cnt = mask.cnt();
	    logMessage("selectStrings", "retrieving %lu 1-byte integer%s "
		       "took %g sec(CPU), %g sec(elapsed)",
		       static_cast<long unsigned>(cnt), (cnt > 1 ? "s" : ""),
		       timer.CPUTime(), timer.realTime());
	}
	break;}
    case ibis::FLOAT: {
	const array_t<float> &prop =
	    * static_cast<const array_t<float>*>(buffer);

	uint32_t i = 0;
	array->resize(tot);
	const uint32_t nprop = prop.size();
	ibis::bitvector::indexSet index = mask.firstIndexSet();
	if (nprop >= mask.size()) {
	    while (index.nIndices() > 0) {
		const ibis::bitvector::word_t *idx0 = index.indices();
		if (index.isRange()) {
		    for (uint32_t j = *idx0; j<idx0[1]; ++j, ++i) {
			std::ostringstream oss;
			oss << (prop[j]);
			(*array)[i] = oss.str();
		    }
		}
		else {
		    for (uint32_t j = 0; j<index.nIndices(); ++j, ++i) {
			std::ostringstream oss;
			oss << (prop[idx0[j]]);
			(*array)[i] = oss.str();
		    }
		}
		++ index;
	    }
	}
	else {
	    while (index.nIndices() > 0) {
		const ibis::bitvector::word_t *idx0 = index.indices();
		if (*idx0 >= nprop) break;
		if (index.isRange()) {
		    for (uint32_t j = *idx0;
			 j<(idx0[1]<=nprop ? idx0[1] : nprop);
			 ++j, ++i) {
			std::ostringstream oss;
			oss << (prop[j]);
			(*array)[i] = oss.str();
		    }
		}
		else {
		    for (uint32_t j = 0; j<index.nIndices(); ++j, ++i) {
			if (idx0[j] < nprop) {
			    std::ostringstream oss;
			    oss << (prop[idx0[j]]);
			    (*array)[i] = oss.str();
			}
			else
			    break;
		    }
		}
		++ index;
	    }
	}

	if (i != tot) {
	    array->resize(i);
	    logWarning("selectStrings", "expects to retrieve %lu elements "
		       "but only got %lu", static_cast<long unsigned>(tot),
		       static_cast<long unsigned>(i));
	}
	else if (ibis::gVerbose > 5) {
	    timer.stop();
	    long unsigned cnt = mask.cnt();
	    logMessage("selectStrings", "retrieving %lu float value%s "
		       "took %g sec(CPU), %g sec(elapsed)",
		       static_cast<long unsigned>(cnt), (cnt > 1 ? "s" : ""),
		       timer.CPUTime(), timer.realTime());
	}
	break;}
    case ibis::DOUBLE: {
	const array_t<double> &prop =
	    * static_cast<const array_t<double>*>(buffer);

	uint32_t i = 0;
	array->resize(tot);
	const uint32_t nprop = prop.size();
	ibis::bitvector::indexSet index = mask.firstIndexSet();
	if (nprop >= mask.size()) {
	    while (index.nIndices() > 0) {
		const ibis::bitvector::word_t *idx0 = index.indices();
		if (index.isRange()) {
		    for (uint32_t j = *idx0; j<idx0[1]; ++j, ++i) {
			std::ostringstream oss;
			oss << (prop[j]);
			(*array)[i] = oss.str();
		    }
		}
		else {
		    for (uint32_t j = 0; j<index.nIndices(); ++j, ++i) {
			std::ostringstream oss;
			oss << (prop[idx0[j]]);
			(*array)[i] = oss.str();
		    }
		}
		++ index;
	    }
	}
	else {
	    while (index.nIndices() > 0) {
		const ibis::bitvector::word_t *idx0 = index.indices();
		if (*idx0 >= nprop) break;
		if (index.isRange()) {
		    for (uint32_t j = *idx0;
			 j<(idx0[1]<=nprop ? idx0[1] : nprop);
			 ++j, ++i) {
			std::ostringstream oss;
			oss << (prop[j]);
			(*array)[i] = oss.str();
		    }
		}
		else {
		    for (uint32_t j = 0; j<index.nIndices(); ++j, ++i) {
			if (idx0[j] < nprop) {
			    std::ostringstream oss;
			    oss << (prop[idx0[j]]);
			    (*array)[i] = oss.str();
			}
			else
			    break;
		    }
		}
		++ index;
	    }
	}

	if (i != tot) {
	    array->resize(i);
	    logWarning("selectStrings", "expects to retrieve %lu elements "
		       "but only got %lu", static_cast<long unsigned>(tot),
		       static_cast<long unsigned>(i));
	}
	else if (ibis::gVerbose > 5) {
	    timer.stop();
	    long unsigned cnt = mask.cnt();
	    logMessage("selectStrings", "retrieving %lu double value%s "
		       "took %g sec(CPU), %g sec(elapsed)",
		       static_cast<long unsigned>(cnt), (cnt > 1 ? "s" : ""),
		       timer.CPUTime(), timer.realTime());
	}
	break;}
    case ibis::OID: {
	const array_t<ibis::rid_t> &prop =
	    * static_cast<const array_t<ibis::rid_t>*>(buffer);

	uint32_t i = 0;
	array->resize(tot);
	const uint32_t nprop = prop.size();
	ibis::bitvector::indexSet index = mask.firstIndexSet();
	if (nprop >= mask.size()) {
	    while (index.nIndices() > 0) {
		const ibis::bitvector::word_t *idx0 = index.indices();
		if (index.isRange()) {
		    for (uint32_t j = *idx0; j<idx0[1]; ++j, ++i) {
			std::ostringstream oss;
			oss << (prop[j]);
			(*array)[i] = oss.str();
		    }
		}
		else {
		    for (uint32_t j = 0; j<index.nIndices(); ++j, ++i) {
			std::ostringstream oss;
			oss << (prop[idx0[j]]);
			(*array)[i] = oss.str();
		    }
		}
		++ index;
	    }
	}
	else {
	    while (index.nIndices() > 0) {
		const ibis::bitvector::word_t *idx0 = index.indices();
		if (*idx0 >= nprop) break;
		if (index.isRange()) {
		    for (uint32_t j = *idx0;
			 j<(idx0[1]<=nprop ? idx0[1] : nprop);
			 ++j, ++i) {
			std::ostringstream oss;
			oss << (prop[j]);
			(*array)[i] = oss.str();
		    }
		}
		else {
		    for (uint32_t j = 0; j<index.nIndices(); ++j, ++i) {
			if (idx0[j] < nprop) {
			    std::ostringstream oss;
			    oss << (prop[idx0[j]]);
			    (*array)[i] = oss.str();
			}
			else
			    break;
		    }
		}
		++ index;
	    }
	}

	if (i != tot) {
	    array->resize(i);
	    logWarning("selectStrings", "expects to retrieve %lu elements "
		       "but only got %lu", static_cast<long unsigned>(tot),
		       static_cast<long unsigned>(i));
	}
	else if (ibis::gVerbose > 5) {
	    timer.stop();
	    long unsigned cnt = mask.cnt();
	    logMessage("selectStrings", "retrieving %lu ibis::rid_t value%s "
		       "took %g sec(CPU), %g sec(elapsed)",
		       static_cast<long unsigned>(cnt), (cnt > 1 ? "s" : ""),
		       timer.CPUTime(), timer.realTime());
	}
	break;}
    case ibis::TEXT:
    case ibis::CATEGORY: {
	const std::vector<std::string> &prop =
	    * static_cast<const std::vector<std::string>*>(buffer);

	uint32_t i = 0;
	array->resize(tot);
	const uint32_t nprop = prop.size();
	ibis::bitvector::indexSet index = mask.firstIndexSet();
	if (nprop >= mask.size()) {
	    while (index.nIndices() > 0) {
		const ibis::bitvector::word_t *idx0 = index.indices();
		if (index.isRange()) {
		    for (uint32_t j = *idx0; j<idx0[1]; ++j, ++i) {
			(*array)[i] = (prop[j]);
		    }
		}
		else {
		    for (uint32_t j = 0; j<index.nIndices(); ++j, ++i) {
			(*array)[i] = (prop[idx0[j]]);
		    }
		}
		++ index;
	    }
	}
	else {
	    while (index.nIndices() > 0) {
		const ibis::bitvector::word_t *idx0 = index.indices();
		if (*idx0 >= nprop) break;
		if (index.isRange()) {
		    for (uint32_t j = *idx0;
			 j<(idx0[1]<=nprop ? idx0[1] : nprop);
			 ++j, ++i) {
			(*array)[i] = (prop[j]);
		    }
		}
		else {
		    for (uint32_t j = 0; j<index.nIndices(); ++j, ++i) {
			if (idx0[j] < nprop) {
			    (*array)[i] = (prop[idx0[j]]);
			}
			else
			    break;
		    }
		}
		++ index;
	    }
	}

	if (i != tot) {
	    array->resize(i);
	    logWarning("selectStrings", "expects to retrieve %lu elements "
		       "but only got %lu", static_cast<long unsigned>(tot),
		       static_cast<long unsigned>(i));
	}
	else if (ibis::gVerbose > 5) {
	    timer.stop();
	    long unsigned cnt = mask.cnt();
	    logMessage("selectStrings", "retrieving %lu string value%s "
		       "took %g sec(CPU), %g sec(elapsed)",
		       static_cast<long unsigned>(cnt), (cnt > 1 ? "s" : ""),
		       timer.CPUTime(), timer.realTime());
	}
	break;}
    default: {
	logWarning("selectStrings", "incompatible data type");
	break;}
    }
    return array;
} // ibis::bord::column::selectStrings

void ibis::bord::column::getString(uint32_t i, std::string &val) const {
    val.erase();
    if (m_type == ibis::TEXT || m_type == ibis::CATEGORY) {
	std::vector<std::string> *str_column = 
	    static_cast<std::vector<std::string> *>(buffer);
	if ( i < str_column->size())
	    val = str_column->at(i);
    }
} // ibis::bord::column::getString

/// Makes a copy of the in-memory data.  Use shallow copy for ibis::array_t
/// objects.
int ibis::bord::column::getValuesArray(void* vals) const {
    if (vals == 0 || buffer == 0) return -1;
    switch (m_type) {
    default: {
	LOGGER(ibis::gVerbose > 0)
	    << "Warning -- bord[" << (thePart?thePart->name():"")
	    << "]::column[" << m_name << "]::getValuesArray does not yet "
	    "support column type " << ibis::TYPESTRING[(int)m_type];
	break;}
    case ibis::BYTE: {
	static_cast<array_t<signed char>*>(vals)->
	    copy(* static_cast<const array_t<signed char>*>(buffer));
	break;}
    case ibis::UBYTE: {
	static_cast<array_t<unsigned char>*>(vals)->
	    copy(* static_cast<const array_t<unsigned char>*>(buffer));
	break;}
    case ibis::SHORT: {
	static_cast<array_t<int16_t>*>(vals)->
	    copy(* static_cast<const array_t<int16_t>*>(buffer));
	break;}
    case ibis::USHORT: {
	static_cast<array_t<uint16_t>*>(vals)->
	    copy(* static_cast<const array_t<uint16_t>*>(buffer));
	break;}
    case ibis::INT: {
	static_cast<array_t<int32_t>*>(vals)->
	    copy(* static_cast<const array_t<int32_t>*>(buffer));
	break;}
    case ibis::UINT: {
	static_cast<array_t<uint32_t>*>(vals)->
	    copy(* static_cast<const array_t<uint32_t>*>(buffer));
	break;}
    case ibis::LONG: {
	static_cast<array_t<int64_t>*>(vals)->
	    copy(* static_cast<const array_t<int64_t>*>(buffer));
	break;}
    case ibis::ULONG: {
	static_cast<array_t<uint64_t>*>(vals)->
	    copy(* static_cast<const array_t<uint64_t>*>(buffer));
	break;}
    case ibis::FLOAT: {
	static_cast<array_t<float>*>(vals)->
	    copy(* static_cast<const array_t<float>*>(buffer));
	break;}
    case ibis::DOUBLE: {
	static_cast<array_t<double>*>(vals)->
	    copy(* static_cast<const array_t<double>*>(buffer));
	break;}
    case ibis::TEXT:
    case ibis::CATEGORY: {
	std::vector<std::string>
	    tmp(* static_cast<const std::vector<std::string>*>(buffer));
	static_cast<std::vector<std::string>*>(vals)->swap(tmp);
	break;}
}
return 0;
} // ibis::bord::column::getValuesArray

void ibis::bord::column::reverseRows() {
    switch(m_type) {
    case ibis::ULONG: {
	array_t<uint64_t> &prop =
	    * static_cast<array_t<uint64_t>*>(buffer);
	std::reverse(prop.begin(), prop.end());
	break;}
    case ibis::LONG: {
	array_t<int64_t> &prop =
	    * static_cast<array_t<int64_t>*>(buffer);
	std::reverse(prop.begin(), prop.end());
	break;}
    case ibis::UINT: {
	array_t<uint32_t> &prop =
	    * static_cast<array_t<uint32_t>*>(buffer);
	std::reverse(prop.begin(), prop.end());
	break;}
    case ibis::INT: {
	array_t<int32_t> &prop =
	    * static_cast<array_t<int32_t>*>(buffer);
	std::reverse(prop.begin(), prop.end());
	break;}
    case ibis::USHORT: {
	array_t<uint16_t> &prop =
	    * static_cast<array_t<uint16_t>*>(buffer);
	std::reverse(prop.begin(), prop.end());
	break;}
    case ibis::SHORT: {
	array_t<int16_t> &prop =
	    * static_cast<array_t<int16_t>*>(buffer);
	std::reverse(prop.begin(), prop.end());
	break;}
    case ibis::UBYTE: {
	array_t<unsigned char> &prop =
	    * static_cast<array_t<unsigned char>*>(buffer);
	std::reverse(prop.begin(), prop.end());
	break;}
    case ibis::BYTE: {
	array_t<signed char> &prop =
	    * static_cast<array_t<signed char>*>(buffer);
	std::reverse(prop.begin(), prop.end());
	break;}
    case ibis::FLOAT: {
	array_t<float> &prop =
	    * static_cast<array_t<float>*>(buffer);
	std::reverse(prop.begin(), prop.end());
	break;}
    case ibis::DOUBLE: {
	array_t<double> &prop =
	    * static_cast<array_t<double>*>(buffer);
	std::reverse(prop.begin(), prop.end());
	break;}
    case ibis::CATEGORY:
    case ibis::TEXT: {
	std::vector<std::string> &prop =
	    * static_cast<std::vector<std::string>*>(buffer);
	std::reverse(prop.begin(), prop.end());
	break;}
    default: {
	logWarning("reverseRows", "incompatible data type");
	break;}
    }
} // ibis::bord::column::reverseRows

int ibis::bord::column::limit(uint32_t nr) {
    int ierr = 0;
    switch(m_type) {
    case ibis::ULONG: {
	array_t<uint64_t> &prop =
	    * static_cast<array_t<uint64_t>*>(buffer);
	if (nr < prop.size())
	    prop.resize(nr);
	break;}
    case ibis::LONG: {
	array_t<int64_t> &prop =
	    * static_cast<array_t<int64_t>*>(buffer);
	if (nr < prop.size())
	    prop.resize(nr);
	break;}
    case ibis::UINT: {
	array_t<uint32_t> &prop =
	    * static_cast<array_t<uint32_t>*>(buffer);
	if (nr < prop.size())
	    prop.resize(nr);
	break;}
    case ibis::INT: {
	array_t<int32_t> &prop =
	    * static_cast<array_t<int32_t>*>(buffer);
	if (nr < prop.size())
	    prop.resize(nr);
	break;}
    case ibis::USHORT: {
	array_t<uint16_t> &prop =
	    * static_cast<array_t<uint16_t>*>(buffer);
	if (nr < prop.size())
	    prop.resize(nr);
	break;}
    case ibis::SHORT: {
	array_t<int16_t> &prop =
	    * static_cast<array_t<int16_t>*>(buffer);
	if (nr < prop.size())
	    prop.resize(nr);
	break;}
    case ibis::UBYTE: {
	array_t<unsigned char> &prop =
	    * static_cast<array_t<unsigned char>*>(buffer);
	if (nr < prop.size())
	    prop.resize(nr);
	break;}
    case ibis::BYTE: {
	array_t<signed char> &prop =
	    * static_cast<array_t<signed char>*>(buffer);
	if (nr < prop.size())
	    prop.resize(nr);
	break;}
    case ibis::FLOAT: {
	array_t<float> &prop =
	    * static_cast<array_t<float>*>(buffer);
	if (nr < prop.size())
	    prop.resize(nr);
	break;}
    case ibis::DOUBLE: {
	array_t<double> &prop =
	    * static_cast<array_t<double>*>(buffer);
	if (nr < prop.size())
	    prop.resize(nr);
	break;}
    case ibis::CATEGORY:
    case ibis::TEXT: {
	std::vector<std::string> &prop =
	    * static_cast<std::vector<std::string>*>(buffer);
	if (nr < prop.size())
	    prop.resize(nr);
	break;}
    default: {
	logWarning("reverseRows", "incompatible data type");
	ierr = -1;
	break;}
    }
    return ierr;
} // ibis::bord::column::limit

/// Convert the integer representation to string representation.  The
/// existing data type must be ibis::UINT and the column with the same in
/// in the given ibis::part prt must be of type ibis::CATEGORY.
int ibis::bord::column::restoreCategoriesAsStrings(const ibis::part& prt) {
    if (m_type != ibis::UINT) // must be uint32_t
	return -2;
    const ibis::column* ctmp = prt.getColumn(m_name.c_str());
    if (ctmp == 0)
	return -3;
    if (ctmp->type() != ibis::CATEGORY)
	return -4;

    ibis::array_t<uint32_t> *arrint =
	static_cast<ibis::array_t<uint32_t>*>(buffer);
    const int nr = (thePart->nRows() <= arrint->size() ?
		    thePart->nRows() : arrint->size());
    std::vector<std::string> *arrstr = new std::vector<std::string>(nr);
    for (int j = 0; j < nr; ++ j)
	ctmp->getString((*arrint)[j], (*arrstr)[j]);
    delete arrint; // free the storage for the integers.
    m_type = ibis::CATEGORY;
    buffer = arrstr;
    return nr;
} // ibis::bord::column::restoreCategoriesAsStrings

/// Constructor.  It retrieves the columns from the table object using that
/// function ibis::part::getColumn(uint32_t), which preserves the order
/// specified in the original table construction, but may leave the columns
/// in an arbitrary order.
ibis::bord::cursor::cursor(const ibis::bord &t)
    : buffer(t.nColumns()), tab(t), curRow(-1) {
    if (buffer.empty()) return;
    for (uint32_t j = 0; j < t.nColumns(); ++ j) {
	const ibis::bord::column *col =
	    dynamic_cast<const ibis::bord::column*>(t.mypart.getColumn(j));
	if (col != 0) {
	    buffer[j].cname = col->name();
	    buffer[j].ctype = col->type();
	    buffer[j].cval = col->getArray();
	    bufmap[col->name()] = j;
	}
    }
} // ibis::bord::cursor::cursor

/// Print the content of the current row.
int ibis::bord::cursor::dump(std::ostream& out, const char* del) const {
    if (curRow < 0 || curRow >= (int64_t) tab.nRows())
	return -1;
    if (! out) return -4;

    const uint32_t cr = static_cast<uint32_t>(curRow);
    int ierr = dumpIJ(out, cr, 0U);
    if (ierr < 0) return ierr;
    if (del == 0) del = ", ";
    for (uint32_t j = 1; j < buffer.size(); ++ j) {
	out << del;
	ierr = dumpIJ(out, cr, j);
	if (ierr < 0) return ierr;
    }
    out << "\n";
    return (out ? ierr : -4);
} // ibis::bord::cursor::dump

void ibis::bord::cursor::fillRow(ibis::table::row& res) const {
    res.clear();
    for (uint32_t j = 0; j < buffer.size(); ++ j) {
	switch (buffer[j].ctype) {
	case ibis::BYTE: {
	    res.bytesnames.push_back(buffer[j].cname);
	    if (buffer[j].cval) {
		res.bytesvalues.push_back
		    ((* static_cast<const array_t<const char>*>
		      (buffer[j].cval))[curRow]);
	    }
	    else {
		res.bytesvalues.push_back(0x7F);
	    }
	    break;}
	case ibis::UBYTE: {
	    res.bytesnames.push_back(buffer[j].cname);
	    if (buffer[j].cval) {
		res.ubytesvalues.push_back
		    ((* static_cast<const array_t<const unsigned char>*>
		      (buffer[j].cval))[curRow]);
	    }
	    else {
		res.ubytesvalues.push_back(0xFF);
	    }
	    break;}
	case ibis::SHORT: {
	    res.shortsnames.push_back(buffer[j].cname);
	    if (buffer[j].cval) {
		res.shortsvalues.push_back
		    ((* static_cast<const array_t<int16_t>*>
		      (buffer[j].cval))[curRow]);
	    }
	    else {
		res.shortsvalues.push_back(0x7FFF);
	    }
	    break;}
	case ibis::USHORT: {
	    res.shortsnames.push_back(buffer[j].cname);
	    if (buffer[j].cval) {
		res.ushortsvalues.push_back
		    ((* static_cast<const array_t<const uint16_t>*>
		      (buffer[j].cval))[curRow]);
	    }
	    else {
		res.ushortsvalues.push_back(0xFFFF);
	    }
	    break;}
	case ibis::INT: {
	    res.intsnames.push_back(buffer[j].cname);
	    if (buffer[j].cval) {
		res.intsvalues.push_back
		    ((* static_cast<const array_t<int32_t>*>
		      (buffer[j].cval))[curRow]);
	    }
	    else {
		res.intsvalues.push_back(0x7FFFFFFF);
	    }
	    break;}
	case ibis::UINT: {
	    res.intsnames.push_back(buffer[j].cname);
	    if (buffer[j].cval) {
		res.uintsvalues.push_back
		    ((* static_cast<const array_t<const uint32_t>*>
		      (buffer[j].cval))[curRow]);
	    }
	    else {
		res.uintsvalues.push_back(0xFFFFFFFF);
	    }
	    break;}
	case ibis::LONG: {
	    res.longsnames.push_back(buffer[j].cname);
	    if (buffer[j].cval) {
		res.longsvalues.push_back
		    ((* static_cast<const array_t<int64_t>*>
		      (buffer[j].cval))[curRow]);
	    }
	    else {
		res.longsvalues.push_back(0x7FFFFFFFFFFFFFFFLL);
	    }
	    break;}
	case ibis::ULONG: {
	    res.longsnames.push_back(buffer[j].cname);
	    if (buffer[j].cval) {
		res.ulongsvalues.push_back
		    ((* static_cast<const array_t<const uint64_t>*>
		      (buffer[j].cval))[curRow]);
	    }
	    else {
		res.ulongsvalues.push_back(0xFFFFFFFFFFFFFFFFULL);
	    }
	    break;}
	case ibis::FLOAT: {
	    res.floatsnames.push_back(buffer[j].cname);
	    if (buffer[j].cval) {
		res.floatsvalues.push_back
		    ((* static_cast<const array_t<float>*>
		      (buffer[j].cval))[curRow]);
	    }
	    else {
		res.floatsvalues.push_back
		    (std::numeric_limits<float>::quiet_NaN());
	    }
	    break;}
	case ibis::DOUBLE: {
	    res.doublesnames.push_back(buffer[j].cname);
	    if (buffer[j].cval) {
		res.doublesvalues.push_back
		    ((* static_cast<const array_t<const double>*>
		      (buffer[j].cval))[curRow]);
	    }
	    else {
		res.doublesvalues.push_back
		    (std::numeric_limits<double>::quiet_NaN());
	    }
	    break;}
	case ibis::CATEGORY: {
	    res.catsnames.push_back(buffer[j].cname);
	    if (buffer[j].cval) {
		res.catsvalues.push_back
		    ((* static_cast<const std::vector<std::string>*>
		      (buffer[j].cval))[curRow]);
	    }
	    else {
		res.catsvalues.push_back("");
	    }
	    break;}
	case ibis::TEXT: {
	    res.textsnames.push_back(buffer[j].cname);
	    if (buffer[j].cval) {
		res.textsvalues.push_back
		    ((* static_cast<const std::vector<std::string>*>
		      (buffer[j].cval))[curRow]);
	    }
	    else {
		res.textsvalues.push_back("");
	    }
	    break;}
	default: { // unexpected
	    if (ibis::gVerbose > 1)
		ibis::util::logMessage
		    ("Warning", "bord::cursor::fillRow is not expected "
		     "to encounter data type %s (column name %s)",
		     ibis::TYPESTRING[(int)buffer[j].ctype], buffer[j].cname);
	    break;}
	}
    }
} // ibis::bord::cursor::fillRow

int ibis::bord::cursor::getColumnAsByte(uint32_t j, char& val) const {
    if (curRow < 0 || curRow >= (int64_t) tab.nRows())
	return -1;
    if (buffer[j].cval == 0)
	return -2;

    int ierr;
    switch (buffer[j].ctype) {
    case ibis::BYTE:
    case ibis::UBYTE: {
	val = (* static_cast<const array_t<signed char>*>(buffer[j].cval))[curRow];
	ierr = 0;
	break;}
    default: {
	ierr = -1;
	break;}
    }
    return ierr;
} // ibis::bord::cursor::getColumnAsByte

int ibis::bord::cursor::getColumnAsUByte(uint32_t j, unsigned char& val) const {
    if (curRow < 0 || curRow >= (int64_t) tab.nRows())
	return -1;
    if (buffer[j].cval == 0)
	return -2;

    int ierr;
    switch (buffer[j].ctype) {
    case ibis::BYTE:
    case ibis::UBYTE: {
	val = (* static_cast<const array_t<unsigned char>*>(buffer[j].cval))
	    [curRow];
	ierr = 0;
	break;}
    default: {
	ierr = -1;
	break;}
    }
    return ierr;
} // ibis::bord::cursor::getColumnAsUByte

int ibis::bord::cursor::getColumnAsShort(uint32_t j, int16_t& val) const {
    if (curRow < 0 || curRow >= (int64_t) tab.nRows())
	return -1;
    if (buffer[j].cval == 0)
	return -2;

    int ierr;
    switch (buffer[j].ctype) {
    case ibis::BYTE: {
	val = (* static_cast<const array_t<signed char>*>(buffer[j].cval))[curRow];
	ierr = 0;
	break;}
    case ibis::UBYTE: {
	val = (* static_cast<const array_t<unsigned char>*>
	       (buffer[j].cval))[curRow];
	ierr = 0;
	break;}
    case ibis::SHORT:
    case ibis::USHORT: {
	val = (* static_cast<const array_t<int16_t>*>(buffer[j].cval))
	    [curRow];
	ierr = 0;
	break;}
    default: {
	ierr = -1;
	break;}
    }
    return ierr;
} // ibis::bord::cursor::getColumnAsShort

int ibis::bord::cursor::getColumnAsUShort(uint32_t j, uint16_t& val) const {
    if (curRow < 0 || curRow >= (int64_t) tab.nRows())
	return -1;
    if (buffer[j].cval == 0)
	return -2;

    int ierr;
    switch (buffer[j].ctype) {
    case ibis::BYTE:
    case ibis::UBYTE: {
	val = (* static_cast<const array_t<unsigned char>*>(buffer[j].cval))
	    [curRow];
	ierr = 0;
	break;}
    case ibis::SHORT:
    case ibis::USHORT: {
	val = (* static_cast<const array_t<uint16_t>*>(buffer[j].cval))[curRow];
	ierr = 0;
	break;}
    default: {
	ierr = -1;
	break;}
    }
    return ierr;
} // ibis::bord::cursor::getColumnAsUShort

int ibis::bord::cursor::getColumnAsInt(uint32_t j, int32_t& val) const {
    if (curRow < 0 || curRow >= (int64_t) tab.nRows())
	return -1;
    if (buffer[j].cval == 0)
	return -2;

    int ierr;
    switch (buffer[j].ctype) {
    case ibis::BYTE: {
	val = (* static_cast<const array_t<signed char>*>(buffer[j].cval))[curRow];
	ierr = 0;
	break;}
    case ibis::UBYTE: {
	val = (* static_cast<const array_t<unsigned char>*>
	       (buffer[j].cval))[curRow];
	ierr = 0;
	break;}
    case ibis::SHORT: {
	val = (* static_cast<const array_t<int16_t>*>
	       (buffer[j].cval))[curRow];
	ierr = 0;
	break;}
    case ibis::USHORT: {
	val = (* static_cast<const array_t<uint16_t>*>
	       (buffer[j].cval))[curRow];
	ierr = 0;
	break;}
    case ibis::INT:
    case ibis::UINT: {
	val = (* static_cast<const array_t<int32_t>*>
	       (buffer[j].cval))[curRow];
	ierr = 0;
	break;}
    default: {
	ierr = -1;
	break;}
    }
    return ierr;
} // ibis::bord::cursor::getColumnAsInt

int ibis::bord::cursor::getColumnAsUInt(uint32_t j, uint32_t& val) const {
    if (curRow < 0 || curRow >= (int64_t) tab.nRows())
	return -1;
    if (buffer[j].cval == 0)
	return -2;

    int ierr;
    switch (buffer[j].ctype) {
    case ibis::BYTE:
    case ibis::UBYTE: {
	val = (* static_cast<const array_t<unsigned char>*>
	       (buffer[j].cval))[curRow];
	ierr = 0;
	break;}
    case ibis::SHORT:
    case ibis::USHORT: {
	val = (* static_cast<const array_t<uint16_t>*>
	       (buffer[j].cval))[curRow];
	ierr = 0;
	break;}
    case ibis::INT:
    case ibis::UINT: {
	val = (* static_cast<const array_t<uint32_t>*>
	       (buffer[j].cval))[curRow];
	ierr = 0;
	break;}
    default: {
	ierr = -1;
	break;}
    }
    return ierr;
} // ibis::bord::cursor::getColumnAsUInt

int ibis::bord::cursor::getColumnAsLong(uint32_t j, int64_t& val) const {
    if (curRow < 0 || curRow >= (int64_t) tab.nRows())
	return -1;
    if (buffer[j].cval == 0)
	return -2;

    int ierr;
    switch (buffer[j].ctype) {
    case ibis::BYTE: {
	val = (* static_cast<const array_t<signed char>*>(buffer[j].cval))[curRow];
	ierr = 0;
	break;}
    case ibis::UBYTE: {
	val = (* static_cast<const array_t<unsigned char>*>
	       (buffer[j].cval))[curRow];
	ierr = 0;
	break;}
    case ibis::SHORT: {
	val = (* static_cast<const array_t<int16_t>*>
	       (buffer[j].cval))[curRow];
	ierr = 0;
	break;}
    case ibis::USHORT: {
	val = (* static_cast<const array_t<uint16_t>*>
	       (buffer[j].cval))[curRow];
	ierr = 0;
	break;}
    case ibis::INT: {
	val = (* static_cast<const array_t<int32_t>*>
	       (buffer[j].cval))[curRow];
	ierr = 0;
	break;}
    case ibis::UINT: {
	val = (* static_cast<const array_t<uint32_t>*>
	       (buffer[j].cval))[curRow];
	ierr = 0;
	break;}
    case ibis::LONG:
    case ibis::ULONG: {
	val = (* static_cast<const array_t<int64_t>*>
	       (buffer[j].cval))[curRow];
	ierr = 0;
	break;}
    default: {
	ierr = -1;
	break;}
    }
    return ierr;
} // ibis::bord::cursor::getColumnAsLong

int ibis::bord::cursor::getColumnAsULong(uint32_t j, uint64_t& val) const {
    if (curRow < 0 || curRow >= (int64_t) tab.nRows())
	return -1;
    if (buffer[j].cval == 0)
	return -2;

    int ierr;
    switch (buffer[j].ctype) {
    case ibis::BYTE:
    case ibis::UBYTE: {
	val = (* static_cast<const array_t<unsigned char>*>
	       (buffer[j].cval))[curRow];
	ierr = 0;
	break;}
    case ibis::SHORT:
    case ibis::USHORT: {
	val = (* static_cast<const array_t<uint16_t>*>
	       (buffer[j].cval))[curRow];
	ierr = 0;
	break;}
    case ibis::INT:
    case ibis::UINT: {
	val = (* static_cast<const array_t<uint32_t>*>
	       (buffer[j].cval))[curRow];
	ierr = 0;
	break;}
    case ibis::LONG:
    case ibis::ULONG: {
	val = (* static_cast<const array_t<uint64_t>*>
	       (buffer[j].cval))[curRow];
	ierr = 0;
	break;}
    default: {
	ierr = -1;
	break;}
    }
    return ierr;
} // ibis::bord::cursor::getColumnAsULong

int ibis::bord::cursor::getColumnAsFloat(uint32_t j, float& val) const {
    if (curRow < 0 || curRow >= (int64_t) tab.nRows())
	return -1;
    if (buffer[j].cval == 0)
	return -2;

    int ierr;
    switch (buffer[j].ctype) {
    case ibis::BYTE: {
	val = (* static_cast<const array_t<signed char>*>(buffer[j].cval))[curRow];
	ierr = 0;
	break;}
    case ibis::UBYTE: {
	val = (* static_cast<const array_t<unsigned char>*>
	       (buffer[j].cval))[curRow];
	ierr = 0;
	break;}
    case ibis::SHORT: {
	val = (* static_cast<const array_t<int16_t>*>
	       (buffer[j].cval))[curRow];
	ierr = 0;
	break;}
    case ibis::USHORT: {
	val = (* static_cast<const array_t<uint16_t>*>
	       (buffer[j].cval))[curRow];
	ierr = 0;
	break;}
    case ibis::FLOAT: {
	val = (* static_cast<const array_t<float>*>
	       (buffer[j].cval))[curRow];
	ierr = 0;
	break;}
    default: {
	ierr = -1;
	break;}
    }
    return ierr;
} // ibis::bord::cursor::getColumnAsFloat

int ibis::bord::cursor::getColumnAsDouble(uint32_t j, double& val) const {
    if (curRow < 0 || curRow >= (int64_t) tab.nRows())
	return -1;
    if (buffer[j].cval == 0)
	return -2;

    int ierr;
    switch (buffer[j].ctype) {
    case ibis::BYTE: {
	val = (* static_cast<const array_t<signed char>*>(buffer[j].cval))[curRow];
	ierr = 0;
	break;}
    case ibis::UBYTE: {
	val = (* static_cast<const array_t<unsigned char>*>
	       (buffer[j].cval))[curRow];
	ierr = 0;
	break;}
    case ibis::SHORT: {
	val = (* static_cast<const array_t<int16_t>*>
	       (buffer[j].cval))[curRow];
	ierr = 0;
	break;}
    case ibis::USHORT: {
	val = (* static_cast<const array_t<uint16_t>*>
	       (buffer[j].cval))[curRow];
	ierr = 0;
	break;}
    case ibis::INT: {
	val = (* static_cast<const array_t<int32_t>*>
	       (buffer[j].cval))[curRow];
	ierr = 0;
	break;}
    case ibis::UINT: {
	val = (* static_cast<const array_t<uint32_t>*>
	       (buffer[j].cval))[curRow];
	ierr = 0;
	break;}
    case ibis::DOUBLE: {
	val = (* static_cast<const array_t<double>*>(buffer[j].cval))[curRow];
	ierr = 0;
	break;}
    default: {
	ierr = -1;
	break;}
    }
    return ierr;
} // ibis::bord::cursor::getColumnAsDouble

int ibis::bord::cursor::getColumnAsString(uint32_t j, std::string& val) const {
    if (curRow < 0 || curRow >= (int64_t) tab.nRows())
	return -1;
    if (buffer[j].cval == 0)
	return -2;

    int ierr;
    std::ostringstream oss;
    switch (buffer[j].ctype) {
    case ibis::BYTE: {
	oss << static_cast<int>((* static_cast<const array_t<signed char>*>
				 (buffer[j].cval))[curRow]);
	val = oss.str();
	ierr = 0;
	break;}
    case ibis::UBYTE: {
	oss << static_cast<unsigned>
	    ((* static_cast<const array_t<unsigned char>*>
	      (buffer[j].cval))[curRow]);
	val = oss.str();
	ierr = 0;
	break;}
    case ibis::SHORT: {
	oss << (* static_cast<const array_t<int16_t>*>
		(buffer[j].cval))[curRow];
	val = oss.str();
	ierr = 0;
	break;}
    case ibis::USHORT: {
	oss << (* static_cast<const array_t<uint16_t>*>
		(buffer[j].cval))[curRow];
	val = oss.str();
	ierr = 0;
	break;}
    case ibis::INT: {
	oss << (* static_cast<const array_t<int32_t>*>
		(buffer[j].cval))[curRow];
	val = oss.str();
	ierr = 0;
	break;}
    case ibis::UINT: {
	oss << (* static_cast<const array_t<uint32_t>*>
		(buffer[j].cval))[curRow];
	val = oss.str();
	ierr = 0;
	break;}
    case ibis::LONG: {
	oss << (* static_cast<const array_t<int64_t>*>
		(buffer[j].cval))[curRow];
	val = oss.str();
	ierr = 0;
	break;}
    case ibis::ULONG: {
	oss << (* static_cast<const array_t<uint64_t>*>
		(buffer[j].cval))[curRow];
	val = oss.str();
	ierr = 0;
	break;}
    case ibis::FLOAT: {
	oss << (* static_cast<const array_t<float>*>
		(buffer[j].cval))[curRow];
	val = oss.str();
	ierr = 0;
	break;}
    case ibis::DOUBLE: {
	oss << (* static_cast<const array_t<double>*>
		(buffer[j].cval))[curRow];
	val = oss.str();
	ierr = 0;
	break;}
    case ibis::CATEGORY:
    case ibis::TEXT: {
	const ibis::column* col = tab.mypart.getColumn(buffer[j].cname);
	if (col != 0) {
	    col->getString(static_cast<uint32_t>(curRow), val);
	    ierr = 0;
	}
	else {
	    ierr = -1;
	}
	break;}
    default: {
	ierr = -1;
	break;}
    }
    return ierr;
} // ibis::bord::cursor::getColumnAsLong
