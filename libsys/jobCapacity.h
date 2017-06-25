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

#ifndef sys_jobCapacity_INCL_
#define sys_jobCapacity_INCL_

/*! \file
\brief Declarations for sys::jobCapacity
*/


#include "libsys/Utilization.h"
#include "libsys/jobNotification.h"


namespace sys
{
namespace job
{

//! Track usage (of whatever) with notification on changes
class Capacity
{
	Utilization theUtilization;
	Notification theNotification;

private: // disable

	//! Disable implicit copy and assignment
	Capacity(Capacity const &) = delete;
	Capacity & operator=(Capacity const &) = delete;

public: // methods

	//! Construct with tracking up to maxCount number
	inline
	explicit
	Capacity
		( size_t const & maxCount
		);

	//! Check if instance is valid
	bool
	isValid
		() const;

	//! True when at least one resource is in use
	inline
	bool
	isInUse
		();

	//! True when utilization is less than capacity
	inline
	bool
	hasVacancy
		();

	//! Increase the usage count
	inline
	void
	consume
		();

	//! Decrement the usage count
	inline
	void
	release
		();

	//! Stop calling thread until utilization state changes
	inline
	void
	waitForVacancy
		();

	//! Descriptive information about this instance.
	std::string
	infoString
		( std::string const & title = std::string()
		) const;

};

}
}

// Inline definitions
#include "libsys/jobCapacity.inl"

#endif // sys_jobCapacity_INCL_

