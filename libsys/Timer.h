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

#ifndef sys_Timer_INCL_
#define sys_Timer_INCL_

/*! \file
\brief Declarations for sys::Timer
*/


#include "libsys/time.h"

#include <string>
#include <utility>
#include <vector>


namespace sys
{

/*! \brief Simple timing support.

\par Example
\dontinclude testsys/uTimer.cpp
\skip ExampleStart
\until ExampleEnd
*/

class Timer
{
	//! Information about one timing interval
	struct Interval
	{
		double theStart;
		double theStop;
		std::string theInfo;

		//! Value ctor
		inline
		explicit
		Interval
			( std::string const & info
			);

		//! True if this instance is complete (has been stop'ed)
		bool
		isDone
			() const;

		//! Set end time
		inline
		void
		stop
			();

		//! Elapsed time
		inline
		double
		duration
			() const;

		//! Descriptive information about this instance
		std::string
		infoString
			( std::string const & title = std::string()
			) const;
	};

	std::vector<Interval> theIntervals;
	bool theTimerIsActive;

public: // methods

	//! Create a timer - which is stopped.
	inline
	Timer
		();

	// copy constructor -- compiler provided
	// assignment operator -- compiler provided
	// destructor -- compiler provided

	//! Start timing an interval - ends previous
	inline
	void
	start
		( std::string const & iName = std::string()
		);

	//! End timing an interval
	inline
	void
	stop
		();

	//! Sum of all intervals
	double
	total
		() const;

	//! Descriptive information about this instance.
	std::string
	infoString
		( std::string const & title = std::string()
		) const;
};

}

// Inline definitions
#include "libsys/Timer.inl"

#endif // sys_Timer_INCL_

