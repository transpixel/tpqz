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

#ifndef mem_query_INCL_
#define mem_query_INCL_

/*! \file
\brief Declarations for mem::query
*/


#include <sstream>
#include <string>
#include <utility>
#include <vector>


namespace mem
{

/*! \brief Query memory information pertaining to current (calling) process.

\par Example
\dontinclude testmem/uquery.cpp
\skip ExampleStart
\until ExampleEnd
*/
namespace query
{
	//! Items to report
	static std::vector<std::string> const sFindKeys
		{ "VmSize:"
		, "VmRSS:"
		, "VmData:"
		, "VmPeak:"
		, "VmHWM:"
		, "VmLib:"
		};

	//! Description of various memory properties (from /proc/self/status)
	std::string
	infoString
		( std::string const & title = std::string()
		, std::vector<std::string> const & findKeys = sFindKeys
		, std::string const & sep = std::string(" ")
		);

	//! Functor for reporting infoString() content
	struct Reporter
	{
		std::ostringstream theOss{};
		std::vector<std::string> const theKeys{};
		std::string const theSep{};

		//! Construct 'noop' instance
		Reporter
			() = default;

		//! Construct to report keys
		explicit
		Reporter
			( std::vector<std::string> const & findKeys = sFindKeys
			, std::string const & sep = std::string(" ")
			);

		//! Log current info associated with ctor keys
		void
		operator()
			( std::string const & title = std::string()
			);

		//! Descriptive information about this instance
		std::string
		infoString
			( std::string const & title = std::string()
			) const;
	};

}

}

// Inline definitions
// #include "libmem/query.inl"

#endif // mem_query_INCL_

