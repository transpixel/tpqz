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
\brief Inline definitions for img::raw10
*/



namespace img
{

namespace raw10
{

//! Functions for decoding data byte/bit patterns
namespace bitwork
{

	using WorkType = unsigned int; // probably more 'natural' for compiler
	// using WorkType = uint16_t;

	//! Decode hiByte as largest component of full value
	inline
	WorkType
	hiValue
		( uint8_t const & hiByte
		)
	{
		// multiply to leave room for low 2 bits
		return
			{ static_cast<WorkType>
				(static_cast<WorkType>(hiByte) << WorkType(2u))
			};
	}

	//! Active low bits associated with mask
	inline
	WorkType
	onLoBits
		( uint8_t const & loBits
		, uint8_t const & mask
		)
	{
		return
			{ static_cast<WorkType>
				( static_cast<WorkType>(loBits)
				& static_cast<WorkType>(mask)
				)
			};
	} 

	//! Value associated with (masked and shifted) loBit pattern
	inline
	WorkType
	loValue
		( uint8_t const & loBits
		, uint8_t const & mask
		, WorkType const & shift
		)
	{
		return { static_cast<WorkType>(onLoBits(loBits, mask) >> shift) };
	}

	//! Value associated with (masked and UNshifted) loBit pattern
	inline
	WorkType
	loValue
		( uint8_t const & loBits
		, uint8_t const & mask
		)
	{
		return { onLoBits(loBits, mask) };
	}

	//! Decode member bit patterns into full values in working units
	inline
	std::array<WorkType, 4u>
	workValues
		( FourPix const & quad
		)
	{
		std::array<uint8_t, 4u> const & hiBytes = quad.theHiBytes;
		uint8_t const & loBits = quad.theLoBits;
		return std::array<WorkType, 4u>
			{ static_cast<WorkType>
				( hiValue(hiBytes[0]) + loValue(loBits, 0xC0, 6u)
				)
			, static_cast<WorkType>
				( hiValue(hiBytes[1]) + loValue(loBits, 0x30, 4u)
				)
			, static_cast<WorkType>
				( hiValue(hiBytes[2]) + loValue(loBits, 0x0C, 2u)
				)
			, static_cast<WorkType>
				( hiValue(hiBytes[3]) + loValue(loBits, 0x0C)
				)
			};
	}

	//! Full values converted to image pixel type
	template <typename PixType>
	inline
	std::array<PixType, 4u>
	pixelValues
		( FourPix const & quad
		)
	{
		std::array<WorkType, 4u> const workVals{ workValues(quad) };
		return std::array<PixType, 4u>
			{ static_cast<PixType>(workVals[0])
			, static_cast<PixType>(workVals[1])
			, static_cast<PixType>(workVals[2])
			, static_cast<PixType>(workVals[3])
			};
	}

	//! Specialization for uint8_t pixel recovery (hi-bits only)
	template <>
	inline
	std::array<uint8_t, 4u>
	pixelValues<uint8_t>
		( FourPix const & quad
		)
	{
		return { quad.theHiBytes };
	}

} // bitwork

template <typename PixType>
std::array<PixType, 4u>
pixelValues
	( FourPix const & quad
	)
{
	return { bitwork::pixelValues<PixType>(quad) };
}

} // raw10

} // img

