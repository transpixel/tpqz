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

#ifndef build_version_INCL_
#define build_version_INCL_

/*! \file
\brief Declarations for build::version
*/

#include <string>
#include <sstream>

namespace build
{

//! \brief functions for s/w version management.
namespace version
{
	//! Version Brand String (build date)
	inline
	std::string
	buildInfo
		( std::string const & argv0
		, std::string const & vid = std::string(SCM_VERSION_ID)
		, std::string const & bdate = __DATE__
		, std::string const & btime = __TIME__
		)
	{
		std::ostringstream oss;

		oss << argv0 << std::endl;
		if (! vid.empty())
		{
			oss
				<< "  " <<  "... Version:"
				<< " " << vid
				;
		}
		else
		{
			oss
				<< "  " <<  "... Build Date/Time:"
				<< " " << bdate
				<< " " << btime
				;
		}

		return oss.str();
	}

}

}

// Inline definitions
// #include "libbuild/version.inl"

#endif // build_version_INCL_

