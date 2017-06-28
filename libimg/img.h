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

#ifndef img_img_INCL_
#define img_img_INCL_

/*! \file
\brief Declarations for namespace img
*/


#include "libdat/dat.h"
#include "libdat/grid.h"
#include "libdat/MinMax.h"
#include "libdat/Region.h"
#include "libdat/template.h"

#include "exthalf/half.hpp"

#include <limits>
#include <utility>
#include <vector>


/*! \brief Declarations and Definitions for libimg.

\par General Concept:

Operations for simple digital images - emphasis on quantitative processing.

\par Special Notes:

+ Templatized but most support for "float" pixel types.
+ Controlled radiometric operations (e.g. under/overflow checking)


*/
namespace img
{
	//! floating point pixel type
	typedef float fpix_t;

	//
	// Floating point pixel convention
	//   -inf == pix     : underflow
	//   -inf < pix < 0. : underflow
	//   0. < pix < +inf : active (i.e. okay)
	//   pix == +inf     : overflow
	//   pix == nan      : invalid
	//

	//! missing pixel value
	constexpr fpix_t fpixBad(dat::nullValue<fpix_t>());

	//! unresponsive pixel value
	constexpr fpix_t fpixUnder
		(- std::numeric_limits<fpix_t>::infinity());

	//! saturated and unknown pixel value
	constexpr fpix_t fpixOver
		(  std::numeric_limits<fpix_t>::infinity());

	//! minimum 'active' value for unsigned 8-bit
	constexpr uint8_t u8pixMinValid(  1u);
	//! maximum 'active' value for unsigned 8-bit
	constexpr uint8_t u8pixMaxValid(250u);

	//! pixel value is not null: includes (Under, Active, Over)
	template <typename PixType>
	inline
	bool
	isValid
		( PixType const & pix
		);

	//! pixel value negative
	template <typename PixType>
	inline
	bool
	isUnder
		( PixType const & pix
		);

	//! pixel value positive and finite
	template
		<typename PixType, dat::EnableIf< std::is_floating_point<PixType> >...>
	inline
	bool
	isActive
		( PixType const & pix
		);

	//! should be caught by is_floating_point ? ?
	inline
	bool
	isActive
		( half_float::half const & pix
		);

	template <typename PixType, dat::EnableIf< std::is_integral<PixType> >...>
	inline
	bool
	isActive
		( PixType const & pix
		);

	template <typename PixType>
	inline
	bool
	isActive
		( std::array<PixType, 3u> const & deepPix
		);

	//! pixel value (+)infinite
	template <typename PixType>
	inline
	bool
	isOver
		( PixType const & pix
		);

	//! Clamp value between min/max inclusive
	inline
	unsigned long
	uClamp
		( unsigned long const & minValue
		, unsigned long const & value
		, unsigned long const & maxValue
		);
	
	//! Clamp value between min/max inclusive
	inline
	double
	dClamp
		( double const & minValue
		, double const & value
		, double const & maxValue
		);
	
	//! Array scalar multiply
	template <size_t Dim>
	inline
	std::array<fpix_t, Dim>
	operator*
		( double const & lmult
		, std::array<fpix_t, Dim> const & arg
		);

}

// Inline definitions
#include "libimg/img.inl"

#endif // img_img_INCL_

