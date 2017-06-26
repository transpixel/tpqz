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
\brief  This file contains unit test for file::path
*/


#include "libfile/path.h"

#include "libdat/info.h"
#include "libdat/validity.h"
#include "libio/stream.h"

#include <iostream>
#include <sstream>
#include <string>


namespace
{

//! Check for common functions
std::string
file_path_test0
	()
{
	std::ostringstream oss;
	/*
	file::path const aNull(dat::nullValue<file::path>());
	if (dat::isValid(aNull))
	{
		oss << "Failure of null value test" << std::endl;
		oss << "infoString: " << dat::infoString(aNull) << std::endl;
	}
	*/
	return oss.str();
}

	void
	checkNames
		( std::ostream & oss
		, std::string const & expName
		, std::string const & gotName
		, std::string const & testname
		)
	{
		if (! (gotName == expName))
		{
			oss << "\nFailure of " << testname << " test" << std::endl;
			oss << "expName: '" << expName << "'\n";
			oss << "gotName: '" << gotName << "'\n";
		}
	}

//! Check basic posix file path parsing
std::string
file_path_test1
	()
{
	std::ostringstream oss;

	std::string const expNameDir("../somedir");
	std::string const expNameBase("filebase");
	std::string const expNameExt("ext");
	std::string const expNameFile(expNameBase + "." + expNameExt);
	std::string const expFullPath(expNameDir + "/" + expNameFile);

	// check parsing operations
	std::string const gotNameDir(file::path::dirname(expFullPath));
	std::string const gotNameBase(file::path::basename(expFullPath));
	std::string const gotNameFile(file::path::filename(expFullPath));
	std::string const gotNameExt(file::path::extension(expFullPath));

	checkNames(oss, expNameDir, gotNameDir, "gotNameDir");
	checkNames(oss, expNameBase, gotNameBase, "gotNameBase");
	checkNames(oss, expNameFile, gotNameFile, "gotNameFile");
	checkNames(oss, expNameExt, gotNameExt, "gotNameExt");

	// check synthesis operations
	std::string const gotFullPathDF
		(file::path::fullpathDirFile(gotNameDir, gotNameFile));
	std::string const gotFullPathDBE
		(file::path::fullpathDirBaseExt(gotNameDir, gotNameBase, gotNameExt));

	checkNames(oss, expFullPath, gotFullPathDF, "gotFullPathDF");
	checkNames(oss, expFullPath, gotFullPathDBE, "gotFullPathDBE");


	return oss.str();
}


}

//! Unit test for file::path
int
main
	( int const /*argc*/
	, char const * const * /*argv*/
	)
{
	std::ostringstream oss;

	// run tests
	oss << file_path_test0();
	oss << file_path_test1();

	// check/report results
	std::string const errMessages(oss.str());
	if (! errMessages.empty())
	{
		io::err() << errMessages << std::endl;
		return 1;
	}
	return 0;
}
