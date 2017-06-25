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
\brief Definitions for sig::filter
*/


#include "libsig/filter.h"


namespace sig
{
namespace filter
{

size_t
hunkSizeFor
	( size_t const & moveSize
	, size_t const & fullSize
	)
{
	size_t size{ dat::nullValue<size_t>() };
	if ( dat::isValid(moveSize) && (0u < moveSize)
	  && dat::isValid(fullSize) && (0u < fullSize)
	  && (moveSize <= fullSize)
	   )
	{
		size = (fullSize - moveSize + 1u);
	}
	return size;
}

size_t
moveSizeFor
	( size_t const & fullSize
	, size_t const & hunkSize
	)
{
	size_t size{ dat::nullValue<size_t>() };
	if ( dat::isValid(fullSize) && (0u < fullSize)
	  && dat::isValid(hunkSize) && (0u < hunkSize)
	  && (hunkSize <= fullSize)
	   )
	{
		//  M = F - H + 1
		size = (fullSize - hunkSize + 1u);
	}
	return size;
}

size_t
fullSizeFor
	( size_t const & hunkSize
	, size_t const & moveSize
	)
{
	size_t size{ dat::nullValue<size_t>() };
	if ( dat::isValid(hunkSize) && (0u < hunkSize)
	  && dat::isValid(moveSize) && (0u < moveSize)
	   )
	{
		//  F = M + H - 1
		size = (moveSize + hunkSize - 1u);
	}
	return size;
}

dat::Extents
hunkExtentsFor
	( dat::Extents const & moveExtents
	, dat::Extents const & fullExtents
	)
{
	return dat::Extents
		( hunkSizeFor(moveExtents.high(), fullExtents.high())
		, hunkSizeFor(moveExtents.wide(), fullExtents.wide())
		);
}

dat::Extents
moveExtentsFor
	( dat::Extents const & fullExtents
	, dat::Extents const & hunkExtents
	)
{
	return dat::Extents
		( moveSizeFor(fullExtents.high(), hunkExtents.high())
		, moveSizeFor(fullExtents.wide(), hunkExtents.wide())
		);
}

dat::Extents
fullExtentsFor
	( dat::Extents const & hunkExtents
	, dat::Extents const & moveExtents
	)
{
	return dat::Extents
		( fullSizeFor(hunkExtents.high(), moveExtents.high())
		, fullSizeFor(hunkExtents.wide(), moveExtents.wide())
		);
}

} // filter
} // sig

