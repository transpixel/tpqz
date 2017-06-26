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

#ifndef cloud_PointIterator_INCL_
#define cloud_PointIterator_INCL_

/*! \file
\brief Declarations for cloud::PointIterator
*/


#include "libcloud/cast.h"
#include "libcloud/cast.h"

//#include <string>


namespace cloud
{

/*! \brief Provide iterator over ga::Vector representation of fixed points

\par Example
\dontinclude testcloud/uPointIterator.cpp
\skip ExampleStart
\until ExampleEnd
*/

class PointIterator
{
	using FwdIter = std::vector<FixedPoint>::const_iterator;

	FwdIter theBeg;
	FwdIter theIter;
	FwdIter theEnd;

public: // methods

	//! default null constructor
	PointIterator
		() = default;

	//! Provide iterator that cast
	explicit
	PointIterator
		( std::vector<FixedPoint> const & fpnts
		)
		: theBeg{ fpnts.begin() }
		, theIter{ theBeg }
		, theEnd{ fpnts.end() }
	{
	}

	// copy constructor -- compiler provided
	// assignment operator -- compiler provided
	// destructor -- compiler provided

	//! Like isActive() but w/o validity check
	inline
	explicit operator bool
		() const
	{
		return (theEnd != theIter);
	}

	//! RowCol expression of current iterator value
	inline
	ga::Vector
	vectorPoint
		() const
	{
		return cast::Vector(*theIter);
	}

	//! Move forward by one element of domain
	inline
	PointIterator &
	operator++
		()
	{
		++theIter;
		return *this;
	}

	/*
	//! Check if instance is valid
	bool
	isValid
		() const
	{
		return (! (theBeg == FwdIter{}));
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
		}
		else
		{
			oss << " <null>";
		}
		return oss.str();
	}
	*/

};

}

// Inline definitions
// #include "libcloud/PointIterator.inl"

#endif // cloud_PointIterator_INCL_

