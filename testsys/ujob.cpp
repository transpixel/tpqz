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
\brief  This file contains unit test for sys::job
*/


#include "libsys/job.h"

#include "libdat/info.h"
#include "libdat/dat.h"
#include "libio/sprintf.h"
#include "libio/stream.h"

#include <iostream>
#include <sstream>
#include <string>


namespace
{

//! Check for common functions
std::string
sys_job_test0
	()
{
	std::ostringstream oss;

	// simple constructions

	std::mutex aMutex{};
	std::condition_variable aCond{};
	sys::job::Notification{};

	sys::job::Capacity aCapacity(17u);

	sys::job::Runner aRunner(nullptr, & aCapacity);

	std::vector<std::shared_ptr<sys::JobBase> > noJobs;
	sys::job::Factory factory(noJobs, 8u);

	return oss.str();
}

	static std::mutex sMutexDumb{};
	static size_t sCountDumb{ 0u };
	struct JobDumb : public sys::JobBase
	{
		JobDumb
			()
			: JobBase{ "JobDumb" }
		{}

		virtual
		void
		run
			() const
		{
			{ std::lock_guard<std::mutex> lock(sMutexDumb);
				++sCountDumb;
			}
		}
	};

//! Check with basic simple jobs
std::string
sys_job_test1
	()
{
	std::ostringstream oss;

	// create a bunch of jobs that compete for access to a counter
	constexpr size_t numJobs{ 1024u };
	std::vector<std::shared_ptr<sys::JobBase> > jobs;
	jobs.reserve(numJobs);
	for (size_t nn(0u) ; nn < numJobs ; ++nn)
	{
		jobs.emplace_back(std::make_shared<JobDumb>());
	}

	// use factory to run all the jobs
	size_t const numBunches{ 10u };
	for (size_t nn(0u) ; nn < numBunches ; ++nn)
	{
	//	size_t const maxThreads{ 0u }; // asserts (intentionally)
		size_t const maxThreads{ nn + 1u }; // include 0 case
		sys::job::Factory factory(jobs, maxThreads);
		factory.processAll();
	}

	// check that jobs updated the counter as many times as expected
	size_t const expCount{ numBunches * numJobs };
	size_t const & gotCount = sCountDumb;
	if (! dat::nearlyEquals(gotCount, expCount))
	{
		oss << "Failure of factory jobDumb count test" << std::endl;
		oss << dat::infoString(expCount, "expCount") << std::endl;
		oss << dat::infoString(gotCount, "gotCount") << std::endl;
	}

	return oss.str();
}

	static std::mutex sMutexSlow{};
	static size_t sCountSlow{ 0u };
	struct JobSlow : public sys::JobBase
	{
		JobSlow
			( size_t const & id
			)
			: JobBase{ io::sprintf("JobSlow-%03d", id) }
		{}

		virtual
		void
		run
			() const
		{
			// block a bit to simulate running
			std::this_thread::sleep_for(std::chrono::microseconds(500));
			{ std::lock_guard<std::mutex> lock(sMutexSlow);
				++sCountSlow;
				// block a bit with mutext locked
				std::this_thread::sleep_for(std::chrono::microseconds(137));
			}
		}
	};

//! Check factory with jobs which take a 'while' to run
std::string
sys_job_test2
	()
{
	std::ostringstream oss;

	constexpr size_t numJobs{ 7u };
	constexpr size_t maxThreads{ 3u };
	std::vector<std::shared_ptr<sys::JobBase> > jobs;
	jobs.reserve(numJobs);
	for (size_t nn{0u} ; nn < numJobs ; ++nn)
	{
		jobs.emplace_back(std::make_shared<JobSlow>(nn));
	}

	// use factory to run all the jobs
	size_t const numBunches{ 2u };
	for (size_t nn(0u) ; nn < numBunches ; ++nn)
	{
		sys::job::Factory factory(jobs, maxThreads);
		factory.processAll();
	}

	// check that jobs updated the counter as many times as expected
	size_t const expCount{ numBunches * numJobs };
	size_t const & gotCount = sCountSlow;
	if (! dat::nearlyEquals(gotCount, expCount))
	{
		oss << "Failure of factory jobSlow count test" << std::endl;
		oss << dat::infoString(expCount, "expCount") << std::endl;
		oss << dat::infoString(gotCount, "gotCount") << std::endl;
	}

	return oss.str();
}


}

//! Unit test for sys::job
int
main
	( int const /*argc*/
	, char const * const * /*argv*/
	)
{
	std::ostringstream oss;

	// run tests
	oss << sys_job_test0();
	oss << sys_job_test1();
	oss << sys_job_test2();

	// check/report results
	std::string const errMessages(oss.str());
	if (! errMessages.empty())
	{
		io::err() << errMessages << std::endl;
		return 1;
	}
	return 0;
}
