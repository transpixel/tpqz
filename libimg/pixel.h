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

#ifndef img_pixel_INCL_
#define img_pixel_INCL_

/*! \file
\brief Declarations for img::pixel
*/


#include "libimg/img.h"


namespace img
{

//! Pixel type definitions

class UPix8
{
	std::array<uint8_t, 3> theChans;

public: // static methods

	//! largest valid value
	inline
	static
	uint8_t
	maxValid
		();

public: // methods

	//! Construct a default pixel (use masks for null values and such)
	inline
	explicit
	UPix8
		();

	//! Value constructor
	inline
	explicit
	UPix8
		( uint8_t const & red
		, uint8_t const & grn
		, uint8_t const & blu
		);

	//! Size of pixel contents
	inline
	size_t
	sizeOfPixel
		() const;

	//! Read only access to pixel channel
	inline
	const uint8_t &
	operator[]
		( size_t const & channel
		) const;

	//! Write access to pixel channel
	inline
	uint8_t &
	operator[]
		( size_t const & channel
		);

	//! Descriptive information about this instance
	inline
	std::string
	infoString
		( std::string const & title = std::string()
		) const;
};


class FPix
{
	std::array<fpix_t, 3> theChans;

public: // methods

	//! Construct with bad value
	inline
	explicit
	FPix
		();

	//! Value constructor
	inline
	explicit
	FPix
		( float const & chan0
		, float const & chan1
		, float const & chan2
		);

	//! True if this instance is valid
	inline
	bool
	isValid
		() const;

	//! Size of pixel contents
	inline
	size_t
	sizeOfPixel
		() const;

	//! Read only access to pixel channel
	inline
	const fpix_t &
	operator[]
		( size_t const & channel
		) const;

	//! Write access to pixel channel
	inline
	fpix_t &
	operator[]
		( size_t const & channel
		);

	//! Descriptive information about this instance
	inline
	std::string
	infoString
		( std::string const & title = std::string()
		, size_t const & precision = 4u
		) const;

};


}

// Inline definitions
#include "libimg/pixel.inl"

#endif // img_pixel_INCL_

