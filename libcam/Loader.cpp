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
\brief Definitions for cam::Loader
*/


#include "libcam/Loader.h"

#include "libcam/io.h"
#include "libdat/validity.h"
#include "libfile/path.h"

#include <fstream>
#include <sstream>


namespace
{
	std::vector<cam::MeaGroupOneAcq>
	acqGroupsFor
		( std::vector<std::string> const & meapaths
		, std::set<cam::PntName> * const & ptPntNames
		, std::set<cam::AcqName> * const & ptAcqNames
		)
	{
		std::vector<cam::MeaGroupOneAcq> meaGroups;
		meaGroups.reserve(meapaths.size());
		for (std::string const & meapath : meapaths)
		{
			std::ifstream ifs(meapath);
			meaGroups.emplace_back
				(cam::io::loadFromAsciiTrifecta(ifs, ptPntNames));
			if (ptAcqNames)
			{
				cam::AcqName const acqName{ file::path::basename(meapath) };
				ptAcqNames->insert(acqName);
			}
		}
		return meaGroups;
	}
}

namespace cam
{

// static
XRefSpots
Loader :: spotTableFor
	( std::vector<std::string> const & meapaths
	)
{
	return Loader(meapaths).spotTable();
}

// explicit
Loader :: Loader
	( std::vector<std::string> const & meapaths
	)
	: thePntNames{}
	, theAcqNames{}
	, theMeaGroups{ acqGroupsFor(meapaths, &thePntNames, &theAcqNames) }
{ }

namespace
{
	std::map<std::string, size_t>
	aNameNdxMap
		( std::set<std::string> const & names
		)
	{
		std::map<std::string, size_t> nameNdxMap;
		size_t ndx{ 0u };
		for (std::string const & name : names)
		{
			nameNdxMap[name] = ndx;
			++ndx;
		}
		return nameNdxMap;
	}
}

std::map<PntName, PntNdx>
Loader :: pntNameNdxMap
	() const
{
	return aNameNdxMap(thePntNames);
}

std::map<AcqName, AcqNdx>
Loader :: acqNameNdxMap
	() const
{
	return aNameNdxMap(theAcqNames);
}

XRefSpots
Loader :: spotTable
	( std::map<PntName, PntNdx> const & pntNameNdxMap
	) const
{
	XRefSpots spotTab(thePntNames.size(), theMeaGroups.size());

	size_t const numAcqs{ theMeaGroups.size() };
	for (AcqNdx acqNdx{0u} ; acqNdx < numAcqs ; ++acqNdx)
	{
		MeaGroupOneAcq const & meaGroup = theMeaGroups[acqNdx];
		for (MeaForOnePnt const & meaPnt : meaGroup)
		{
			std::string const & pntName = meaPnt.thePntName;
			dat::Spot const & spot = meaPnt.theSpot;

			std::map<PntName, PntNdx>::const_iterator const iter
				{ pntNameNdxMap.find(pntName) };
			assert(pntNameNdxMap.end() != iter);
			PntNdx const & pntNdx = iter->second;
			spotTab(pntNdx, acqNdx) = spot;
		}
	}

	return spotTab;
}

XRefSpots
Loader :: spotTable
	() const
{
	return spotTable(pntNameNdxMap());
}

std::string
Loader :: infoStringDetail
	( std::string const & title
	) const
{
	std::ostringstream oss;
	if (! title.empty())
	{
		oss << title << std::endl;
	}

	// point names
	std::map<PntName, PntNdx> const pntMap{ pntNameNdxMap() };
	oss << "Point count: " << pntMap.size();
	for (std::pair<cam::PntName, cam::PntNdx> const & nameNdx : pntMap)
	{
		oss << std::endl;
		oss << "... " << dat::infoString(nameNdx);
	}

	// acquisition names
	std::map<AcqName, AcqNdx> const acqMap{ acqNameNdxMap() };
	oss << std::endl;
	oss << "Acquisition count: " << acqMap.size();
	for (std::pair<cam::AcqName, cam::AcqNdx> const & nameNdx : acqMap)
	{
		oss << std::endl;
		oss << "... " << dat::infoString(nameNdx);
	}

	return oss.str();
}

} // cam

