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
\brief Definitions for sys::jobFactory
*/


#include "libsys/jobFactory.h"

#include <iomanip>
#include <sstream>


namespace sys
{
namespace job
{

bool
Factory :: isValid
	() const
{
	return theCapacity.isValid();
}

std::string
Factory :: infoString
	( std::string const & title
	) const
{
	std::ostringstream oss;
	if (! title.empty())
	{
		oss << title << std::endl;
	}
	if (isValid())
	{
		static std::string pre{ "..." };
		oss << pre
			<< "theCapacity:"
			<< theCapacity.infoString();

		constexpr size_t fw{ 6u };
		oss << std::endl;
		oss << pre
			<< "numJobs:"
			<< " " << std::setw(fw) << theJobs.size();

		oss << std::endl;
		oss << pre
			<< "theJobNdx:"
			<< " " << std::setw(fw) << theJobNdx;

		oss << std::endl;
		oss << pre
			<< "numThreads:"
			<< " " << std::setw(fw) << theThreads.size();
	}
	else
	{
		oss << " <null>";
	}
	return oss.str();
}

}
}

