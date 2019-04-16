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
\brief Definitions for img::raw10 10-bit raw image format (from OmniVision?)
*/


#include "libimg/raw10.h"
#include "libio/sprintf.h"

#include <algorithm>
#include <sstream>


namespace img
{
namespace raw10
{

bool
HeadBRCM :: isValid
	() const
{
	static std::array<uint8_t, 4> const magi{ magic() };
	bool const okayMagic
		{ std::equal(magi.begin(), magi.end(), theHdr.begin()) };
	return okayMagic;
}

std::string
infoString
	( FourPix const & quad
	, std::string const & title
	)
{
	std::ostringstream oss;
	if (! title.empty())
	{
		oss << title << " ";
	}
	std::array<uint16_t, 4u> const pixVals{ pixelValues<uint16_t>(quad) };
	oss << std::endl;
	oss
		<< "HiBytes:"
		<< " " << ::io::sprintf("0x%02X", quad.theHiBytes[0])
		<< " " << ::io::sprintf("0x%02X", quad.theHiBytes[1])
		<< " " << ::io::sprintf("0x%02X", quad.theHiBytes[2])
		<< " " << ::io::sprintf("0x%02X", quad.theHiBytes[3])
		<< " " << "LoBits:"
		<< " " << ::io::sprintf("0x%01X", (quad.theLoBits & 0xC0) >> 6u)
		<< " " << ::io::sprintf("0x%01X", (quad.theLoBits & 0x30) >> 4u)
		<< " " << ::io::sprintf("0x%01X", (quad.theLoBits & 0x0C) >> 2u)
		<< " " << ::io::sprintf("0x%01X", (quad.theLoBits & 0x03) >> 0u)
		<< " " << "LoBits:"
		<< " " << ::io::sprintf("%4d", pixVals[0])
		<< " " << ::io::sprintf("%4d", pixVals[1])
		<< " " << ::io::sprintf("%4d", pixVals[2])
		<< " " << ::io::sprintf("%4d", pixVals[3])
		;
	std::array<uint8_t, 4u> const hiBytes{ pixelHiBytes(quad) };
	std::array<uint8_t, 4u> const loBytes{ pixelLoBytes(quad) };
	oss << std::endl;
	oss
		<< "HiBytes:"
		<< " " << ::io::sprintf("0x%02X", hiBytes[0])
		<< " " << ::io::sprintf("0x%02X", hiBytes[1])
		<< " " << ::io::sprintf("0x%02X", hiBytes[2])
		<< " " << ::io::sprintf("0x%02X", hiBytes[3])
		<< " " << "LoBits:"
		<< " " << ::io::sprintf("0x%01X", loBytes[0])
		<< " " << ::io::sprintf("0x%01X", loBytes[1])
		<< " " << ::io::sprintf("0x%01X", loBytes[2])
		<< " " << ::io::sprintf("0x%01X", loBytes[3])
		<< " " << "LoBits:"
		<< " " << ::io::sprintf("%4d", pixVals[0])
		<< " " << ::io::sprintf("%4d", pixVals[1])
		<< " " << ::io::sprintf("%4d", pixVals[2])
		<< " " << ::io::sprintf("%4d", pixVals[3])
		;
	return oss.str();
}

} // raw10

} // img

