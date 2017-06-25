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
\brief Inline Definitions for sys::jobFactory
*/


namespace sys
{
namespace job
{

//
// Runner
//

inline
// explicit
Runner :: Runner
	( std::shared_ptr<JobBase> const & aJob
	, Capacity * const & capacity
	)
	: theJob(aJob)
	, theCapacity{ capacity }
{}

inline
void
Runner :: operator()
	()
{
	// execute job
	(*theJob)();

	// restore resource availability
	theCapacity->release(); // consumed in factory before job thread creation
}


//
// Factory
//

inline
// explicit
Factory :: Factory
	( std::vector<std::shared_ptr<JobBase> > const & jobs
	, size_t const & maxConcurrent
	)
	: theCapacity(maxConcurrent)
	, theJobs(jobs)
	, theJobNdx{ 0u }
	, theThreads{}
{
	theThreads.reserve(theJobs.size());
	assert(0u < maxConcurrent);
}

inline
void
Factory :: processAll
	()
{
	// process all jobs
	while (haveNewJobs())
	{
		// keep resources at full capacity
		while (theCapacity.hasVacancy() && haveNewJobs())
		{
			// denote use of resources
			theCapacity.consume(); // released in Runner() after job end

			// create new job to run within newly consumed capacity
			startNextJob();
		}
		// block until additional capacity becomes available
		theCapacity.waitForVacancy();
	}

	// wait for job completion burndown
	while (theCapacity.isInUse())
	{
		theCapacity.waitForVacancy();
	}

	// ensure all threads are completed
	// - should be no waiting here, since capacity is no longer in use
	for (std::thread & thread : theThreads)
	{
		if (thread.joinable())
		{
			thread.join();
		}
	}
}

inline
void
Factory :: startNextJob
	()
{
	// get the next job
	assert(theJobNdx < theJobs.size());
	std::shared_ptr<JobBase> const & nextJob = theJobs[theJobNdx];
	++theJobNdx;

	assert(nextJob);

	// run job (in a thread)
	Runner run(nextJob, &theCapacity);
	theThreads.emplace_back(std::thread(run));
}

inline
bool
Factory :: haveNewJobs
	() const
{
	return (theJobNdx < theJobs.size());
}

}
}

