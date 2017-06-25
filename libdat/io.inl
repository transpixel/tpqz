//
//
// MIT License
//
// Copyright (c) 2017 Stellacore Corporation.
//
// Permission is hereby granted, free of charge, to any person obtaining
// a copy of this software and associated documentation files (the
// "Software"), to deal in the Software without restriction, including
// without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to
// permit persons to whom the Software is furnished to do so, subject
// to the following conditions:
//
// The above copyright notice and this permission notice shall be
// included in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY
// KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
// WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
// BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
// AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR
// IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//
//


/*! \file
\brief Inline definitions for dat::io
*/


#include "libio/stream.h"

#include <iterator>


namespace dat
{
namespace io
{

namespace priv
{
	//! Number of bytes to expect for binary I/O
	template <typename FwdIter>
	inline
	std::streamsize
	numBytesFor
		( FwdIter const & beg
		, FwdIter const & end
		)
	{
		size_t const numElem
			{ (size_t)std::distance(beg, end) };
		size_t const gotElemSize
			{ sizeof(typename std::iterator_traits<FwdIter>::value_type) };
		std::streamsize const numBytes
			{ (std::streamsize)(gotElemSize * numElem) };
		return numBytes;
	}
}

template <typename FwdIter>
inline
bool
saveBinary
	( std::ostream & ostrm
	, FwdIter const & beg
	, FwdIter const & end
	)
{
	bool okay{ false };
	if (ostrm.good())
	{
		std::streamsize const numBytes{ priv::numBytesFor<FwdIter>(beg, end) };
		ostrm.write((char const *)beg, numBytes);
		okay = (! ostrm.fail());
	}
	else
	{
		::io::err()
			<< "Failure: saveIndicesU32: can't open stream"
			<< std::endl;
	}

	return okay;
}

template <typename FwdIter>
inline
bool
loadBinary
	( std::istream & istrm
	, FwdIter const & beg
	, FwdIter const & end
	, std::streamsize * const & ptGotBytes
	)
{
	bool okay{ false };

	if (istrm.good())
	{
		std::streamsize const expBytes{ priv::numBytesFor<FwdIter>(beg, end) };
		istrm.read((char*)beg, expBytes);
		std::streamsize const gotBytes{ istrm.gcount() };
		if (gotBytes == expBytes)
		{
			okay = (istrm.good());
		}
		if (ptGotBytes)
		{
			*ptGotBytes = gotBytes;
		}
	}
	return okay;
}


}
}

