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

#ifndef sys_JobBase_INCL_
#define sys_JobBase_INCL_

/*! \file
\brief Declarations for sys::JobBase
*/


#include <deque>
#include <memory>
#include <sstream>
#include <string>
#include <thread>
#include <vector>


namespace sys
{

//! \brief Simple baseclass for functor-style processing jobs.
class JobBase
{
public: // data

	std::string const theJobName;

public: // methods

	//! Construct a job with given name
	explicit
	JobBase
		( std::string const & jobName = std::string()
		)
		: theJobName(jobName)
	{
	}

	//! Invoke implementation class's run() method
	void
	operator()
		() const
	{
		// io::out() << "===>:running job: " << theJobName << std::endl;
		run();
		done();
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
			oss << title << " ";
		}
		oss << theJobName;
		return oss.str();
	}

protected:

	//! OVERRIDE this to perform job processing work
	virtual
	void
	run
		() const = 0;

	//! OVERRIDE this to catch completion - default implementation is noop
	virtual
	void
	done
		() const
	{
		// io::out() << "===>:wrap up job: " << theJobName << std::endl;
	}

};

}

// Inline definitions
// #include "libsys/JobBase.inl"

#endif // sys_JobBase_INCL_

