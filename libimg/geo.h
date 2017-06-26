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

#ifndef img_geo_INCL_
#define img_geo_INCL_

/*! \file
\brief Declarations for img::geo
*/


#include "libdat/Extents.h"
#include "libdat/grid.h"
#include "libdat/SubExtents.h"
#include "libdat/validity.h"


namespace img
{

//! Functions for operating on image geometry
namespace geo
{
	//! grid from SubExtents of source (OutType static_cast of InType)
	template <typename OutType, typename InType>
	inline
	dat::grid<OutType>
	croppedGrid
		( dat::grid<InType> const & srcGrid
		, dat::SubExtents const & cropSub
		);

	//! Cropped data from center of srcGrid - (OutType static_cast of InType)
	template <typename OutType, typename InType>
	inline
	dat::grid<OutType>
	centerCroppedGrid
		( dat::grid<InType> const & srcGrid
		, dat::Extents const & outSize
		);

	//! Fill SubExtents of outGrid with contents of srcGrid 
	template <typename OutType, typename InType>
	inline
	void
	insertSubGrid
		( dat::grid<OutType> * const & ptOutGrid
		, dat::SubExtents const & fillSub
		, dat::grid<InType> const & srcGrid
		);

	//! Put collection of item grids into one larger grid
	template <typename PixType, typename FwdIter>
	inline
	dat::grid<PixType>
	montageOf
		( FwdIter const & beg //!< *iter: grid<PixType>
		, FwdIter const & end
		, size_t const & numCols = 10u
		, PixType const & backValue = dat::nullValue<PixType>()
		, size_t const & pad = 0u
		, PixType const & foreValue = dat::nullValue<PixType>()
		);
}

}

// Inline definitions
#include "libimg/geo.inl"

#endif // img_geo_INCL_

