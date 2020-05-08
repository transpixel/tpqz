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
\brief Inline definitions for sys::Timer
*/


#include <cassert>


namespace sys
{
//======================================================================

inline
// explicit
Timer::Interval :: Interval
	( std::string const & info
	)
	: theStart(sys::time::relativeNow())
	, theStop{ -1. }
	, theInfo(info)
{ }

inline
void
Timer::Interval :: stop
	()
{
	theStop = sys::time::relativeNow();
}

inline
double
Timer::Interval :: duration
	() const
{
	return (theStop - theStart);
}

inline
bool
Timer::Interval :: isDone
	() const
{
	return (0. < theStop);
}

//
//
//

inline
Timer :: Timer
	()
	: theIntervals()
	, theTimerIsActive(false)
{
}

// copy constructor -- compiler provided
// assignment operator -- compiler provided
// destructor -- compiler provided

inline
void
Timer :: start
	( std::string const & iName
	)
{
	if (theTimerIsActive)
	{
		// force end of previous interval
		stop();
	}

	// start a new active interval
	Interval const ival(iName);
	theIntervals.push_back(ival);
	theTimerIsActive = true;
}

inline
void
Timer :: stop
	()
{
	if (theTimerIsActive)
	{
		// complete currently active interval
		assert(! theIntervals.empty());
		theIntervals.back().stop();
	}

	// return to quiesence
	theTimerIsActive = false;
}

//======================================================================
}

