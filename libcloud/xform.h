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

#ifndef cloud_xform_INCL_
#define cloud_xform_INCL_

/*! \file
\brief Declarations for cloud::xform
*/


#include "libcloud/FixedPoint.h"
#include "libga/Rigid.h"

#include <vector>


namespace cloud
{

/*! \brief Functions associated with cloud:: data structures.

\par Example
\dontinclude testcloud/uxform.cpp
\skip ExampleStart
\until ExampleEnd
*/

namespace xform
{

	//! Transform 3d point cloud into station frame
	std::vector<cloud::FixedPoint>
	fixedPoints
		( ga::Rigid const & xIntoWrtFrom
		, std::vector<cloud::FixedPoint> const & fpntFroms
		);

}

}

// Inline definitions
// #include "libcloud/xform.inl"

#endif // cloud_xform_INCL_

