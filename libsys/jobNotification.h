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

#ifndef sys_jobNotification_INCL_
#define sys_jobNotification_INCL_

/*! \file
\brief Declarations for sys::jobNotification
*/


#include <condition_variable>
#include <mutex>
#include <thread>


namespace sys
{

//! Structures and functions for concurrent job processing.
namespace job
{

//! Condition variable synchronization
class Notification
{
	std::mutex theMutex{};
	std::condition_variable theCond{};
	bool theIsReal{ false };

private: // disable

	//! Disable implicit copy and assignment
	Notification(Notification const &) = delete;
	Notification & operator=(Notification const &) = delete;

public: // methods

	//! Create a vanilla notifier
	Notification
		() = default;

	//! Suspend calling thread until condition is set (by other thread)
	inline
	void
	waitFor
		();

	//! Set condition and notify other (all) threads of the change
	inline
	void
	issue
		();

};

}
}

// Inline definitions
#include "libsys/jobNotification.inl"

#endif // sys_jobNotification_INCL_

