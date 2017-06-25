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

#ifndef sys_jobFactory_INCL_
#define sys_jobFactory_INCL_

/*! \file
\brief Declarations for sys::jobFactory
*/


#include "libsys/JobBase.h"
#include "libsys/jobCapacity.h"
#include "libsys/Utilization.h"

#include <memory>
#include <thread>
#include <vector>


namespace sys
{
namespace job
{

//! Functor wrapper which updates capacity when running a job
class Runner
{
	std::shared_ptr<JobBase> const theJob;
	Capacity * const theCapacity;

public: // methods

	//! Attach to job
	inline
	explicit
	Runner
		( std::shared_ptr<JobBase> const & aJob
		, Capacity * const & capacity
		);

	//! Execute job inside grab/free capacity bookends
	inline
	void
	operator()
		();
};

//! Manage concurrent processing of jobs
class Factory
{
	Capacity theCapacity;
	std::vector<std::shared_ptr<JobBase> > const & theJobs;
	size_t theJobNdx;
	std::vector<std::thread> theThreads;

private: // disable

	//! Disable implicit copy and assignment
	Factory(Factory const &) = delete;
	Factory & operator=(Factory const &) = delete;

public: // methods

	//! Configure to run a collection of jobs
	inline
	explicit
	Factory
		( std::vector<std::shared_ptr<JobBase> > const & jobs
		, size_t const & maxConcurrent = std::thread::hardware_concurrency()
		);

	//! Check if instance is valid
	bool
	isValid
		() const;

	//! Perform concurrent processing of all jobs (in order provided)
	inline
	void
	processAll
		();

	//! Descriptive information about this instance.
	std::string
	infoString
		( std::string const & title = std::string()
		) const;

private:

	//! Setup and launch next job in line
	inline
	void
	startNextJob
		();

	//! True while there are jobs remaining to be launched
	inline
	bool
	haveNewJobs
		() const;

};

}
}

// Inline definitions
#include "libsys/jobFactory.inl"

#endif // sys_jobFactory_INCL_

