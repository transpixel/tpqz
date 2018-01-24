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

#ifndef cam_Loader_INCL_
#define cam_Loader_INCL_

/*! \file
\brief Declarations for cam::Loader
*/


#include "libcam/cam.h"
#include "libcam/XRefSpots.h"

#include <string>
#include <set>
#include <map>


namespace cam
{

/*! \brief Loader object for reading image measurements

\par Example
\dontinclude testcam/uLoader.cpp
\skip ExampleStart
\until ExampleEnd
*/

class Loader
{

private:

	std::set<PntName> thePntNames;
	std::set<AcqName> theAcqNames;
	std::vector<MeaGroupOneAcq> theMeaGroups;

public: // static methods

	//! Measurement data in table form from all measurement files
	static
	XRefSpots
	spotTableFor
		( std::vector<std::string> const & meapaths
		);

public: // methods

	//! default null constructor
	Loader
		() = default;

	//! Attached to spot table
	explicit
	Loader
		( std::vector<std::string> const & meapaths
		);

	//! Name to index lookup
	std::map<PntName, PntNdx>
	pntNameNdxMap
		() const;

	//! Name to index lookup
	std::map<AcqName, AcqNdx>
	acqNameNdxMap
		() const;

	//! Names of points (name = result[pntNdx])
	std::vector<PntName>
	pntNames // by pntNdx
		() const;

	//! Names of acquisitions (name = result[acqNdx])
	std::vector<AcqName>
	acqNames // by acqNdx
		() const;

	//! Measurement data in table format
	XRefSpots
	spotTable
		( std::map<PntName, PntNdx> const & pntNameNdxMap
		) const;

	//! Measurement data in table format
	XRefSpots
	spotTable
		() const;

	//! Descriptive information about this instance.
	std::string
	infoStringDetail
		( std::string const & title = std::string()
		) const;

}; // Loader

} // cam

// Inline definitions
// #include "libcam/Loader.inl"

#endif // cam_Loader_INCL_

