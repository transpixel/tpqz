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
\brief Definitions for sig::io
*/


#include "libsig/io.h"

#include "libdat/cast.h"
#include "libdat/info.h"
#include "libdat/SpotX.h"

#include <fstream>
#include <sstream>


namespace
{
	//! Text record for point
	std::string
	recordsFor
		( dat::Spot const & spot
		)
	{
		std::ostringstream oss;
		oss
			<< dat::infoString(spot, "spot") << '\n'
			;
		return oss.str();
	}

	//! Text records for a rectangle
	std::string
	recordsFor
		( dat::Spot const & ul
		, dat::Spot const & br
		)
	{
		std::ostringstream oss;
		dat::Spot const bl{{ br[0], ul[1] }};
		dat::Spot const ur{{ ul[0], br[1] }};
		oss
			<< dat::infoString(ul, "i.ul") << '\n'
			<< dat::infoString(bl, "i.bl") << '\n'
			<< dat::infoString(br, "i.br") << '\n'
			<< dat::infoString(ur, "i.ul") << '\n'
			<< dat::infoString(ul, "i.xx") << '\n'
			;
		return oss.str();
	}

	//! Text records for a rectangle
	std::string
	recordsFor
		( dat::RowCol const & ul
		, dat::RowCol const & br
		)
	{
		return recordsFor(dat::cast::Spot(ul), dat::cast::Spot(br));
	}

	//! Text records for a rectangle
	std::string
	recordsFor
		( dat::SubExtents const & crop
		)
	{
		dat::RowCol const & ul = crop.theUL;
		dat::RowCol const br(crop.insideCornerBR());
		return recordsFor(ul, br);
	}

	//! Custom: Text records for a rectangle after transformation
	std::string
	recordsFor
		( sig::MatchConfig const & matchBwA
		, dat::SubExtents const & cropInTgt
		)
	{
		dat::RowCol const ulInTgt = cropInTgt.theUL;
		dat::RowCol const brInTgt(cropInTgt.insideCornerBR());
		dat::Spot const ulInRef(matchBwA.inReference(ulInTgt));
		dat::Spot const brInRef(matchBwA.inReference(brInTgt));
		return recordsFor(ulInRef, brInRef);
	}
}


namespace sig
{
namespace io
{

void
saveMatchConfig
	( sig::MatchConfig const & matchBwA
	)
{

	std::ofstream ofsCropFullA("cropFullA.dat");
	std::ofstream ofsCropOverA("cropOverA.dat");
	std::ofstream ofsCropWorkA("cropWorkA.dat");
	//
	std::ofstream ofsCropFullB("cropFullB.dat");
	std::ofstream ofsCropOverB("cropOverB.dat");
	std::ofstream ofsCropWorkB("cropWorkB.dat");

	// sample the overlap with this number of samples high and wide
	dat::SubExtents const cropOverInA(matchBwA.overCropInReference());
	dat::SubExtents const cropWorkInA(matchBwA.workCropInReference());
	dat::Extents const & workSize = cropWorkInA.theSize;

	// if no overlap, something is very bad
	assert(cropOverInA.isValid());
	assert(cropWorkInA.isValid());
	assert(workSize.isValid());

	// reference grid info
	dat::Extents const & sizeA = matchBwA.theSizeA;
	dat::SubExtents const cropFullA(dat::RowCol{{ 0u, 0u }}, sizeA);

	ofsCropFullA
		<< recordsFor(cropFullA)
		<< "\n\n";
	ofsCropOverA
		<< recordsFor(cropOverInA)
		<< "\n\n";
	ofsCropWorkA
		<< recordsFor(cropWorkInA)
		<< "\n\n"
		;

	// target grid info
	dat::Extents const & sizeB = matchBwA.theSizeB;
	dat::SpotX const & xSrcBWrtSrcA = matchBwA.theTgtWrtRef;
	dat::SubExtents const cropFullB
		(dat::RowCol{{ 0u, 0u }}, sizeB);
	dat::SubExtents const cropOverInB
		(sig::MatchConfig::xformed(xSrcBWrtSrcA, cropOverInA));
	dat::SubExtents const cropWorkInB
		(sig::MatchConfig::xformed(xSrcBWrtSrcA, cropWorkInA));

	ofsCropFullB
		<< recordsFor(matchBwA, cropFullB)
		<< "\n\n";
	ofsCropOverB
		<< recordsFor(matchBwA, cropOverInB)
		<< "\n\n";
	ofsCropWorkB
		<< recordsFor(matchBwA, cropWorkInB)
		<< "\n\n"
		;

}

void
saveFilterContexts
	( std::vector<sig::FilterContext> const & fconSamps
	, sig::MatchConfig const & matchBwA
	)
{
	std::ofstream ofsCropCentA("cropCentA.dat");
	std::ofstream ofsCropCentB("cropCentB.dat");
	std::ofstream ofsCropSampA("cropSampA.dat");
	std::ofstream ofsCropSampB("cropSampB.dat");

	for (sig::FilterContext const & fconSamp : fconSamps)
	{
		// define sample crop area
		dat::SubExtents const cropSampA(fconSamp.referenceCrop());
		dat::SubExtents const cropSampB(fconSamp.targetCrop());
		ofsCropSampA << recordsFor(cropSampA) << "\n\n";
		ofsCropCentA << recordsFor(cropSampA.center()) << "\n\n";
		bool const fitsB(cropSampB.fitsWithin(matchBwA.theSizeB));
		assert(fitsB);
		if (fitsB)
		{
			ofsCropSampB
				<< recordsFor(matchBwA.inReference(cropSampB))
				<< "\n\n";
			ofsCropCentB
				<< recordsFor(matchBwA.inReference(cropSampB.center()))
				<< "\n\n";
		}
	}
}

bool
saveParallax
	( std::vector<std::pair<dat::Spot, dat::Spot> > const & spotPairs
	, std::string const & fpath
	)
{
	std::ofstream ofs(fpath);
	for (std::pair<dat::Spot, dat::Spot> const & spotPairAB : spotPairs)
	{
		dat::Spot const & spotInA = spotPairAB.first;
		dat::Spot const & spotInB = spotPairAB.second;
		using dat::operator-;
		dat::Spot const parallax(spotInB - spotInA);
		double const pmag{ dat::magnitude(parallax) };

		if (dat::isValid(parallax))
		{
			ofs
				<< " " << dat::infoString(spotInA)
				<< " " << dat::infoString(spotInB)
				<< " " << dat::infoString(parallax)
				<< " " << dat::infoString(pmag)
				<< std::endl;
		}
	}
	return (! ofs.bad());
}

}
}

