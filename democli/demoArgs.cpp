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
\brief  This file contains main application program demoArgs
*/


#include "build/version.h"

#include "extcli/Parser.h"
#include "libio/stream.h"

#include <cassert>
#include <exception>
#include <sstream>
#include <iostream>


namespace
{
	struct AppConfig
	{
		bool theIsValid{false};
		std::string theStr1{}; // required from command line
		std::string theStr2{"DeFaulT"};
		double theDub1{1234.5};

		cli::Parser theParser;

		//! Override default values with available arguments
		explicit
		AppConfig
			( int const argc
			, char const * const * const argv
			)
			: theParser(argc, argv)
		{

			// configure arguments
			std::string const key_Str1("r");
			theParser.set_required<std::string>
				(key_Str1, "reqStr", "some string parm");

			std::string const key_Str2("o");
			theParser.set_optional<std::string>
				(key_Str2, "optStr", theStr2, "optional string");

			std::string const key_Dub1("d");
			theParser.set_optional<double>
				(key_Dub1, "dubVal", theDub1, "some numeric value");

			theIsValid = theParser.run();
			if (theIsValid)
			{
				theStr1 = theParser.get<std::string>(key_Str1);
				theStr2 = theParser.get<std::string>(key_Str2);
				theDub1 = theParser.get<double>(key_Dub1);
			}
		}

		//! True if this instance contains valid values
		bool
		isValid
			() const
		{
			return theIsValid;
		}

		//! Descriptive information
		std::string
		infoString
			( std::string const & title = std::string()
			) const
		{
			std::ostringstream oss;
			if (! title.empty())
			{
				oss << title << std::endl;
			}

			oss << "theStr1: '" << theStr1 << "'";

			oss << std::endl;
			oss << "theStr2: '" << theStr2 << "'";

			oss << std::endl;
			oss << "theDub1: " << theDub1;

			return oss.str();
		}
	};
}

//! Main program to demonstrate use of 'cli' command line parser
int
main
	( int const argc
	, char const * const * const argv
	)
{
	// present program version
	io::out() << build::version::buildInfo(argv[0]) << std::endl;

	// configure program options in recognition of command line args
	AppConfig const appConfig(argc, argv);

	if (appConfig.isValid())
	{
		// show results
		io::out() << std::endl;
		io::out() << "--- application specific code" << std::endl;
		io::out() << appConfig.infoString("appConfig") << std::endl;
	}
	else
	{
		io::err() << "bad usage" << std::endl;
	}

	return 0;
}
