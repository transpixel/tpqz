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

#ifndef app_ProcessLogger_INCL_
#define app_ProcessLogger_INCL_

/*! \file
\brief Declarations for app::ProcessLogger
*/


#include "libmem/query.h"
#include "libapp/Timer.h"

#include <memory>
#include <string>
#include <sstream>


namespace app
{

//! \brief Record various process resource usage information.
class ProcessLogger
{

	app::Timer theTimer;
	std::ostringstream theOssMem;
	mem::query::Reporter theMemReporter;

public: // static methods

	//! Mark message to logger if pter is not null
	static
	void
	mark
		( ProcessLogger * const & ptLog
		, std::string const & msg
		);

public: // methods

	//! Construct to monitor specified items
	explicit
	ProcessLogger
		( std::vector<std::string> const memKeys
			= std::vector<std::string>{ "VmSize", "VmRSS" }
		);

	//! Make note of current memory use
	void
	markMem
		( std::string const & msg
		);

	//! Begin a new timing interval
	void
	markTime
		( std::string const & msg
		);

	//! Make a note of current process state
	void
	mark
		( std::string const & msg
		);

	//! Functor form of mark()
	void
	operator()
		( std::string const & msg
		);

	//! Description of memory log
	std::string
	infoStringMem
		( std::string const & title = std::string()
		) const;

	//! Description of timer log
	std::string
	infoStringTime
		( std::string const & title = std::string()
		) const;

};

}

// Inline definitions
// #include "libapp/ProcessLogger.inl"

#endif // app_ProcessLogger_INCL_

