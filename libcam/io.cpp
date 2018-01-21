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
\brief Definitions for cam::io
*/


#include "libcam/io.h"

#include "libdat/info.h"
#include "libio/sprintf.h"
#include "libio/stream.h"
#include "libio/string.h"

#include <cassert>
#include <fstream>
#include <sstream>


namespace cam
{
namespace io
{

PntName
pntNameForNdx
	( cam::PntNdx const & pntNdx
	, std::string const & ndxFmt
	)
{
	std::string const prefix{ "pnt_" };
	std::string const name{ prefix + ::io::sprintf(ndxFmt, pntNdx) };
	return name;
}

MeaGroupOneAcq
loadFromAsciiTrifecta
	( std::istream & istrm
	, std::set<PntName> * const ptNames
	)
{
	MeaGroupOneAcq meaGroups;

	std::string line;
	std::string record;
	while (istrm && (! istrm.eof()))
	{
		std::getline(istrm, line);
		record = ::io::string::before(line, "#");
		if (! record.empty())
		{
			std::istringstream iss(record);
			PntName name{};
			dat::Spot spot{{}};
			iss >> name >> spot[0] >> spot[1];
			if ((! name.empty()) && (dat::isValid(spot)))
			{
				meaGroups.emplace_back(MeaForOnePnt{ name, spot });
				if (ptNames)
				{
					ptNames->insert(name);
				}
			}
		}
	}

	return meaGroups;
}

bool
insertIntoTable
	( cam::XRefSpots * const & ptSpotTab
	, MeaGroupOneAcq const & meaGroups
	, AcqNdx const & acqNdx
	, std::map<PntName, PntNdx> const & pntNameNdxMap
	)
{
	bool okay{ false };
	if (ptSpotTab && (! meaGroups.empty()))
	{
		if (acqNdx < ptSpotTab->acqCapacity())
		{
			bool hitErr{ false };
			for (MeaForOnePnt const & meaGroup : meaGroups)
			{
				PntName const & pntName = meaGroup.thePntName;
				std::map<PntName, PntNdx>::const_iterator const itFind
					{ pntNameNdxMap.find(pntName) };
				if (pntNameNdxMap.end() != itFind)
				{
					size_t const & pntNdx = itFind->second;
					if (pntNdx < ptSpotTab->pntCapacity())
					{
						(*ptSpotTab)(pntNdx, acqNdx) = meaGroup.theSpot;
					}
				}
				else
				{
					hitErr = true;
				}
			}
			okay = (! hitErr);
		}
	}
	return okay;
}

bool
saveToAsciiTrifecta
	( cam::XRefSpots const & spotTab
	, std::ostream & ostrm
	, cam::AcqNdx const & acqNdx
	)
{
	bool okay{ false };
	if (spotTab.isValid())
	{
		std::vector<cam::PntNdx> const pntNdxs{ spotTab.pntIndicesFor(acqNdx) };
		if(! pntNdxs.empty())
		{
			for (cam::PntNdx const & pntNdx : pntNdxs)
			{
				dat::Spot const & spot = spotTab(pntNdx, acqNdx);
				if (dat::isValid(spot))
				{
					std::string const pntName{ pntNameForNdx(pntNdx) };
					ostrm
						<< dat::infoString(pntName)
						<< " " << dat::infoString(spot)
						<< " " << " 1. 0. 0. 1." // fake covar
						<< '\n';
				}
			}
			okay = (! ostrm.fail());
		}
	}
	return okay;
}

bool
saveToAsciiTrifecta
	( cam::XRefSpots const & spotTab
	, std::vector<std::string> const & acqnames
	)
{
	bool okay{ false };
	assert(acqnames.size() == spotTab.acqCapacity());
	size_t const numAcqs{ spotTab.acqCapacity() };
	for (AcqNdx acqNdx{0u} ; acqNdx < numAcqs ; ++acqNdx)
	{
		std::string const & acqname = acqnames[acqNdx];
		std::string const acqpath{ acqname + ".meapoint" };

		std::ofstream ofsAcq(acqpath);
		okay = saveToAsciiTrifecta(spotTab, ofsAcq, acqNdx);
		if (! okay)
		{
			break;
		}
	}
	return okay;
}

bool
saveToAsciiGnuplot
	( XRefRays const & rayTab
	, XRefDists const & distTab
	, std::string const & fpath
	)
{
	bool okay{ false };

	assert(rayTab.sameCapacityAs(distTab));

	std::ofstream ofs(fpath);
	ofs << "# \n";
	ofs << "# <sx sy sz  dx dy dz  pnt acq>\n";
	ofs << "# gnuplot> splot ... u 1:2:3:4:5:6 w vec\n";
	ofs << "# \n";
	for (XRefRays::const_iterator itRay{rayTab.begin()} ; itRay ; ++itRay)
	{
		cam::RayItem const pavRay(itRay.paItem());
		cam::PntNdx const & pntNdx = pavRay.thePntNdx;
		cam::AcqNdx const & acqNdx = pavRay.theAcqNdx;

		geo::Ray const & ray = pavRay.theItem;
		double const & dist = distTab(pntNdx, acqNdx);
		assert(dat::isValid(ray));
		if (dat::isValid(dist))
		{
			ofs
				<< dat::infoString(ray.theStart)
				<< dat::infoString(dist * ray.theDir)
				<< dat::infoString(pntNdx)
				<< dat::infoString(acqNdx)
				<< '\n';
		}
	}
	ofs << std::endl;
	okay = (! ofs.fail());

	return okay;
}


} // io
} // cam

