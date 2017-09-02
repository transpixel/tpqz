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

#ifndef tri_NodeIterator_INCL_
#define tri_NodeIterator_INCL_

/*! \file
\brief Declarations for tri::NodeIterator
*/


#include "libdat/Area.h"
#include "libdat/validity.h"
#include "libtri/IsoGeo.h"

#include <string>
#include <sstream>


namespace tri
{

/*! \brief Iterator over tritille nodes within valid domain.

\par Example
\dontinclude testtri/uNodeIterator.cpp
\skip ExampleStart
\until ExampleEnd
*/

class NodeIterator
{

	IsoGeo theGeo{};
	Domain theDomain;

	std::pair<long, long> theBegEndI
		{ dat::nullValue<long>(), dat::nullValue<long>() };
	std::pair<long, long> theBegEndJ
		{ dat::nullValue<long>(), dat::nullValue<long>() };

	std::pair<long, long> theAtIJ
		{ dat::nullValue<long>(), dat::nullValue<long>() };
	bool theIsActive{ false };

public: // methods

	//! default null constructor
	NodeIterator
		() = default;

	//! Construct iterator to cover domain
	explicit
	NodeIterator
		( IsoGeo const & trigeo
		, Domain const & xyDomain
		)
		: NodeIterator()
	{
		dat::Area<double> const mnRange{ trigeo.tileAreaForRefArea(xyDomain) };
		if (mnRange.isValid())
		{
			// get bounding (mu,nu) limits of tile space
			dat::Spot const tileSpotBeg{{ mnRange[0].min(), mnRange[1].min() }};
			dat::Spot const tileSpotEnd{{ mnRange[0].max(), mnRange[1].max() }};

			// transform to index+resid form
			IsoGeo::QuantPair const fracPairBeg
				(trigeo.fracPairForTileSpot(tileSpotBeg));
			IsoGeo::QuantPair const fracPairEnd
				(trigeo.fracPairForTileSpot(tileSpotEnd));

			// identify node indices with STL style iterators
			long const & iBeg = fracPairBeg.first.theFloor;
			long const iEnd{ fracPairEnd.first.theFloor + 1 };
			long const & jBeg = fracPairBeg.second.theFloor;
			long const jEnd{ fracPairEnd.second.theFloor + 1 };
			assert (iBeg < iEnd);
			assert (jBeg < jEnd);

			// set member vars
			theGeo = trigeo;
			theDomain = xyDomain;
			theBegEndI = { iBeg, iEnd };
			theBegEndJ = { jBeg, jEnd };
			theAtIJ = { theBegEndI.first, theBegEndJ.first };

			// ensure first node is over valid domain location (or end)
			theIsActive = true; // assume unless contradicted
			if (! atNodeIsValid())
			{
				advanceToNextValid();
			}
		}
	}

	//! True if iterator is currently active (not at end)
	inline
	explicit
	operator bool
		() const
	{
		return theIsActive;
	}

	//! Index pair associated with current (valid) iterator
	inline
	IsoGeo::QuantPair
	fracPair
		() const
	{
		return fracPairAt();
	}

	inline
	std::pair<long, long>
	indexPair
		() const
	{
		return { theAtIJ.first, theAtIJ.second };
	}

	//! Advance to next valid node (node in the domain)
	inline
	NodeIterator &
	operator++
		()
	{
		advanceToNextValid();
		return *this;
	}

	//! Descriptive information about this instance.
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

		// oss << theGeo.infoString("theGeo");
		// oss << std::endl;

		oss << "theBegEndI: " << dat::infoString(theBegEndI);

		oss << std::endl;
		oss << "theBegEndJ: " << dat::infoString(theBegEndJ);

		oss << std::endl;
		if (dat::isValid(theAtIJ))
		{
			oss << "theAtIJ: " << dat::infoString(theAtIJ);
		}
		else
		{
			oss << "theAtIJ: " << "<null>";
		}

		oss << std::endl;
		oss << "active: " << dat::infoString(theIsActive);

		return oss.str();
	}

private:

	//! QuantPair for current position
	inline
	IsoGeo::QuantPair
	fracPairAt
		() const
	{
		return theGeo.fracPairForIndices(theAtIJ.first, theAtIJ.second);
	}

	//! True if xyLoc is within domain area
	inline
	bool
	atNodeIsValid
		() const
	{
		dat::Spot const xyLoc(theGeo.refSpotForFracPair(fracPairAt()));
		return theDomain.contains(xyLoc);
	}

	//! Increment theAtIJ to next node (whether valid or not)
	inline
	void
	advanceToNextAny
		()
	{
		long & ndxAtI = theAtIJ.first;
		long & ndxAtJ = theAtIJ.second;
		++ndxAtJ;
		if (theBegEndJ.second < ndxAtJ)
		{
			ndxAtJ = theBegEndJ.first;
			++ndxAtI;
			if (theBegEndI.second < ndxAtI)
			{
				theIsActive = false;
				ndxAtI = dat::nullValue<long>();
				ndxAtJ = dat::nullValue<long>();
			}
		}
	}

	//! Increment iterator until pointing to next valid instance (or end)
	inline
	void
	advanceToNextValid
		()
	{
		do
		{
			advanceToNextAny();
		}
		while (theIsActive && (! atNodeIsValid()));
	}

}; // NodeIterator

} // tri

// Inline definitions
// #include "libtri/NodeIterator.inl"

#endif // tri_NodeIterator_INCL_

