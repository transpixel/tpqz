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
\brief Definitions for cloud::Panel
*/


#include "libcloud/Panel.h"

#include "libdat/layout.h"

#include "libio/stream.h"
#include "libdat/info.h"

#include <sstream>


namespace cloud
{

	namespace
	{
		//! Transform spots that span area into Pan(el) frame.
		std::vector<ga::Vector>
		pointsFor
			( dat::Area<double> const & area
			, dat::Extents const & hwSize
			, ga::Rigid const & oriPanWrtRef
			)
		{
			std::vector<ga::Vector> pnts;

			// get spots in panel (XY plane) frame
			std::vector<dat::Spot> const spotInPans
				{ dat::layout::expanded::spots(area, hwSize) };

			// tranform into reference frame
			ga::Rigid const oriRefWrtPan{ oriPanWrtRef.inverse() };
			for (dat::Spot const & spotInPan : spotInPans)
			{
				ga::Vector const pntPan(spotInPan[0], spotInPan[1], 0.);
				ga::Vector const pntRef{ oriRefWrtPan(pntPan) };
				pnts.emplace_back(pntRef);
			}
			return pnts;
		}
	}

// explicit
Panel :: Panel
	( dat::Area<double> const & area
	, dat::Extents const & hwSize
	, ga::Rigid const & oriPanWrtRef
	)
	: thePoints{ pointsFor(area, hwSize, oriPanWrtRef) }
{
}

bool
Panel :: isValid
	() const
{
	return (! thePoints.empty());
}

std::string
Panel :: infoString
	( std::string const & title
	) const
{
	std::ostringstream oss;
	if (! title.empty())
	{
		oss << title << std::endl;
	}
	if (isValid())
	{
		oss << "numPnts: " << dat::infoString(thePoints.size());
		for (ga::Vector const & pnt : thePoints)
		{
			oss << std::endl;
			oss << dat::infoString(pnt);
		}
	}
	else
	{
		oss << " <null>";
	}
	return oss.str();
}

} // cloud

