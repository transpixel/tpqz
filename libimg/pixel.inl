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
\brief Inline definitions for img::pixel
*/


#include "libdat/dat.h"

#include <iomanip>
#include <sstream>


//===========================================================================
//===========================================================================
//===========================================================================

inline
// static
uint8_t
img::UPix8 :: maxValid
	()
{
	return 255u;
}

inline
// explicit
img::UPix8 :: UPix8
	()
	: theChans{{ 0u, 0u, 0u }}
{ }

inline
// explicit
img::UPix8 :: UPix8
	( uint8_t const & red
	, uint8_t const & grn
	, uint8_t const & blu
	)
	: theChans{{ red, grn, blu }}
{ }

inline
size_t
img::UPix8 :: sizeOfPixel
	() const
{
	return sizeof(uint8_t);
}

inline
const uint8_t &
img::UPix8 :: operator[]
	( size_t const & channel
	) const
{
	return theChans[channel];
}

inline
uint8_t &
img::UPix8 :: operator[]
	( size_t const & channel
	)
{
	return theChans[channel];
}

inline
std::string
img::UPix8 :: infoString
	( std::string const & title
	) const
{
	std::ostringstream oss;
	if (! title.empty())
	{
		oss << title << "  ";
	}
	using std::setw;
	oss
		<< " " << setw(4) << (int)theChans[0]
		<< " " << setw(4) << (int)theChans[1]
		<< " " << setw(4) << (int)theChans[2]
		;
	return oss.str();
}

//===========================================================================
//===========================================================================
//===========================================================================

inline
// explicit
img::FPix :: FPix
	()
	: theChans{{ fpixBad, fpixBad, fpixBad }}
{ }

inline
// explicit
img::FPix :: FPix
	( float const & chan0
	, float const & chan1
	, float const & chan2
	)
	: theChans{{ (fpix_t)chan0, (fpix_t)chan1, (fpix_t)chan2 }}
{ }

inline
bool
img::FPix :: isValid
	() const
{
	return
		(  dat::isValid(theChans[0])
		&& dat::isValid(theChans[1])
		&& dat::isValid(theChans[2])
		);
}

inline
size_t
img::FPix :: sizeOfPixel
	() const
{
	return sizeof(fpix_t);
}

inline
const img::fpix_t &
img::FPix :: operator[]
	( size_t const & channel
	) const
{
	return theChans[channel];
}

inline
img::fpix_t &
img::FPix :: operator[]
	( size_t const & channel
	)
{
	return theChans[channel];
}

inline
std::string
img::FPix :: infoString
	( std::string const & title
	, size_t const & precision
	) const
{
	std::ostringstream oss;
	if (! title.empty())
	{
		oss << title << "  ";
	}
	size_t const & fpre = precision;
	using std::setw;
	using std::setprecision;
	oss
		<< std::showpoint
		<< " " << setw(fpre+3u) << setprecision(fpre) << (float)theChans[0]
		<< " " << setw(fpre+3u) << setprecision(fpre) << (float)theChans[1]
		<< " " << setw(fpre+3u) << setprecision(fpre) << (float)theChans[2]
		;
	return oss.str();
}

