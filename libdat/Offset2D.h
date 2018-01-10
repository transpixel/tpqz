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

#ifndef dat_Offset2D_INCL_
#define dat_Offset2D_INCL_

/*! \file
\brief Declarations for dat::Offset2D
*/


#include "libdat/info.h"
#include "libdat/validity.h"

#include <array>
#include <string>
#include <sstream>

#include "libio/stream.h" // TODO

namespace dat
{

/*! \brief Translation remapper for 2-dimensional entities.

Functor for producing signed entites from unsigned ones relative
to an unsigned offset.

\par Example
\dontinclude testdat/uOffset2D.cpp
\skip ExampleStart
\until ExampleEnd
*/

template <typename UType, typename SType>
class USOffset
{
	SType theZeroVal{ dat::nullValue<SType>() };

public: // methods

	//! default null constructor
	USOffset
		() = default;

	//! Configure to map zeroPoint to (0,0) output value
	explicit
	USOffset
		( SType const & zeroPoint
		)
		: theZeroVal{ zeroPoint }
	{ }

	//! True if instance is valid
	bool
	isValid
		() const
	{
		return dat::isValid(theZeroVal);
	}

	//! Value after removal of offset
	UType
	operator()
		( SType const & inVal
		) const
	{
		UType posVal{ dat::nullValue<UType>() };
		SType const delta{ static_cast<SType>(inVal - theZeroVal) };
		constexpr SType zero{ 0 };
		if (! (delta < zero))
		{
			posVal = static_cast<UType>(delta);
		}
		return posVal;
	}

	//! Descriptive information about this instance.
	std::string
	infoString
		( std::string const & title = std::string()
		) const
	{
		std::ostringstream oss;
		if (! title.empty())
		{
			oss << title << std::endl;
		}
		if (isValid())
		{
			oss << dat::infoString(theZeroVal, "theZeroVal");
		}
		else
		{
			oss << " <null>";
		}
		return oss.str();
	}

}; // Offset2D


template <typename UType, typename SType>
class Offset2D
{
	std::array<USOffset<UType, SType>, 2u> theUSO;

public: // methods

	//! default null constructor
	Offset2D
		() = default;

	//! Configure to map zeroPoint to (0,0) output value
	explicit
	Offset2D
		( std::array<SType, 2u> const & zeroPoint
		)
		: Offset2D(zeroPoint[0], zeroPoint[1])
	{ }

	//! Convenience version of ctor
	explicit
	Offset2D
		( SType const & zeroPoint0
		, SType const & zeroPoint1
		)
		: theUSO
			{{ USOffset<UType, SType>(zeroPoint0)
			 , USOffset<UType, SType>(zeroPoint1)
			}}
	{ }

	//! True if instance is valid
	bool
	isValid
		() const
	{
		return
			(  theUSO[0].isValid()
			&& theUSO[1].isValid()
			);
	}

	//! Value after removal of offset
	std::array<UType, 2u>
	operator()
		( std::array<SType, 2u> const & inArray
		) const
	{
		return
			{{ theUSO[0](inArray[0])
			 , theUSO[1](inArray[1])
			}};
	}

	//! Descriptive information about this instance.
	std::string
	infoString
		( std::string const & title = std::string()
		) const
	{
		std::ostringstream oss;
		if (! title.empty())
		{
			oss << title << std::endl;
		}
		if (isValid())
		{
			oss
				<< "theUSO[0,1]:"
				<< " " << dat::infoString(theUSO[0])
				<< " " << dat::infoString(theUSO[1])
				;
		}
		else
		{
			oss << " <null>";
		}
		return oss.str();
	}

}; // Offset2D

} // dat

// Inline definitions
// #include "libdat/Offset2D.inl"

#endif // dat_Offset2D_INCL_

