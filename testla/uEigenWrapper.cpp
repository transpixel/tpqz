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
\brief  This file contains unit test for la::Eigen
*/


#include "libla/EigenWrapper.h"

#include "libio/stream.h"

#include <algorithm>
#include <sstream>
#include <string>


namespace
{

//! Check for common functions
std::string
la_Eigen_test0
	()
{
	std::ostringstream oss;
	/*
	la::Eigen const aNull;
	if (aNull.isValid())
	{
		oss << "Failure of null value test" << std::endl;
		oss << "infoString: " << aNull.infoString("aNull") << std::endl;
	}
	*/
	return oss.str();
}


//! Demonstrate basic compile/link capabilities for Eigen template library
std::string
la_Eigen_test1
	()
{
	std::ostringstream oss;

	// wrappers providing methods consistent with overall code base conventions
	using EVec3 = la::EigenWrapper<3, 1>;

	// try a few constructions
	EVec3 const vecA{ 3., 4., 5. };
	EVec3 vecB;
	std::copy(vecA.begin(), vecA.end(), vecB.begin());

	// check compatibility with a few constructions
	if (! vecB.isValid())
	{
		oss << "Failure of construct and copy test" << std::endl;
	}
	if (! vecB.nearlyEquals(vecA))
	{
		oss << "Failure of compare test" << std::endl;
		oss << vecA.infoStringContents("vecA") << std::endl;
		oss << vecB.infoStringContents("vecB") << std::endl;
	}

	// check use of Eigen methods
	la::EigenWrapper<3, 3> const expA
		{ {  1., .07, .06 }
		, {-.08,  1., .05 }
		, { .03, .02,  1. }
		};

	using namespace Eigen;
	JacobiSVD<MatrixXd> const svd(expA.theMat, ComputeFullU|ComputeFullV);
    MatrixXd const matU(svd.matrixU());
    MatrixXd const matS(svd.singularValues().asDiagonal());
    MatrixXd const matV(svd.matrixV());
    MatrixXd const matVt(matV.transpose());
	la::EigenWrapper<3, 3> const gotA(matU * matS * matVt);

	if (! gotA.nearlyEquals(expA))
	{
		oss << "Failure of simple svd test" << std::endl;
		oss << expA.infoStringContents("expA") << std::endl;
		oss << gotA.infoStringContents("gotA") << std::endl;
	}

	// check element access
	la::EigenWrapper<3, 3> foo{ expA };
	if (! foo.hwSize().nearlyEquals(expA.hwSize()))
	{
		oss << "Failure of copy size test" << std::endl;
	}
	if (! dat::nearlyEquals(expA(1,2), foo(1,2)))
	{
		oss << "Failure of element read test" << std::endl;
	}
	foo(0, 0) = 7.;
	if (! dat::nearlyEquals(7., foo(0,0)))
	{
		oss << "Failure of element set test" << std::endl;
	}


	return oss.str();
}


}

//! Unit test for la::Eigen
int
main
	( int const /*argc*/
	, char const * const * /*argv*/
	)
{
	std::ostringstream oss;

	// run tests
	oss << la_Eigen_test0();
	oss << la_Eigen_test1();

	// check/report results
	std::string const errMessages(oss.str());
	if (! errMessages.empty())
	{
		io::err() << errMessages << std::endl;
		return 1;
	}
	return 0;
}
