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


/*! \file
\brief Inline definitions for io::out, io::err, io::log
*/

//! The io/stream module provides lock wrappers for std::ostreams.
namespace io
{

// 
// === err
//

template < typename Type >
inline
err &
err :: operator<<
	( Type const & val
	)
{
	theOss << val;
	return *this;
}

inline
err &
err :: operator<<
	( std::ostream & (*fptr)(std::ostream &)
	)
{
	theOss << fptr;
	return *this;
}

// 
// === out
//

template < typename Type >
inline
out &
out :: operator<<
	( Type const & val
	)
{
	theOss << val;
	return *this;
}

inline
out &
out :: operator<<
	( std::ostream & (*fptr)(std::ostream &)
	)
{
	theOss << fptr;
	return *this;
}

// 
// === log
//

template < typename Type >
inline
log &
log :: operator<<
	( Type const & val
	)
{
	theOss << val;
	return *this;
}

inline
log &
log :: operator<<
	( std::ostream & (*fptr)(std::ostream &)
	)
{
	theOss << fptr;
	return *this;
}

}

