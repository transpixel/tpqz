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

#include <cassert>
#include <fstream>


namespace cam
{
namespace io
{


bool
saveToAsciiTrifecta
	( cam::XRefSpots const & spotTab
	, std::vector<std::string> const & acqnames
	)
{
	bool okay{ false };
	assert(acqnames.size() == spotTab.acqCapacity());
	size_t const numAcqs{ spotTab.acqCapacity() };
	for (size_t acqNdx{0u} ; acqNdx < numAcqs ; ++acqNdx)
	{
		std::string const & acqname = acqnames[acqNdx];
		std::string const acqpath{ acqname + ".meapoint" };

		std::vector<cam::PntNdx> const pntNdxs{ spotTab.pntIndicesFor(acqNdx) };
		if(! pntNdxs.empty())
		{
			std::ofstream ofsAcq(acqpath);
			for (cam::PntNdx const & pntNdx : pntNdxs)
			{
				dat::Spot const & spot = spotTab(pntNdx, acqNdx);
				if (dat::isValid(spot))
				{
					std::string const pntKey
						("pnt_" + ::io::sprintf("%03d",pntNdx));
					ofsAcq
						<< dat::infoString(pntKey)
						<< " " << dat::infoString(spot)
						<< " " << " 1. 0. 0. 1." // fake covar
						<< '\n';
				}
			}
			okay = (! ofsAcq.fail());
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

