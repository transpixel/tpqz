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

#ifndef sys_Utilization_INCL_
#define sys_Utilization_INCL_

/*! \file
\brief Declarations for sys::Utilization
*/


#include <cassert>
#include <mutex>


namespace sys
{

// A thread-safe finite resource counter
class Utilization
{
	std::mutex theMutex{};
	size_t const theMaxCount{};
	size_t theCount{};
	bool theIsValid{ false };

private: // disable

	//! Disable implicit copy and assignment
	Utilization(Utilization const &) = delete;
	Utilization & operator=(Utilization const &) = delete;

public: // methods

	//! Configure to count relative to max value
	inline
	explicit
	Utilization
		( size_t const & maxCount
		);

	//! Check if instance is valid
	bool
	isValid
		() const;

	//! Increase current count (within a lock)
	inline
	void
	increase
		();

	//! Decrease current count (within a lock)
	inline
	void
	decrease
		();

	//! True if nothing is in use (zero count)
	inline
	bool
	isZero
		();

	//! True if current use is less than max allowed use
	inline
	bool
	isIncomplete
		();

	//! Descriptive information about this instance.
	std::string
	infoString
		( std::string const & title = std::string()
		) const;

};


}

// Inline definitions
#include "libsys/Utilization.inl"

#endif // sys_Utilization_INCL_

