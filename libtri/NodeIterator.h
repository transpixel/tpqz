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

	std::pair<long, long> theMinMaxMu
		{ dat::nullValue<long>(), dat::nullValue<long>() };
	std::pair<long, long> theMinMaxNu
		{ dat::nullValue<long>(), dat::nullValue<long>() };

	std::pair<long, long> theAtMuNu
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
		dat::Area<double> const mnRange{ trigeo.mnAreaForXY(xyDomain) };
		if (mnRange.isValid())
		{
			long const muMin{ trigeo.indexForMu(mnRange[0].min()) };
			long const muMax{ trigeo.indexForMu(mnRange[0].max()) };
			long const nuMin{ trigeo.indexForNu(mnRange[1].min()) };
			long const nuMax{ trigeo.indexForNu(mnRange[1].max()) };
			assert (muMin <= muMax);
			assert (nuMin <= nuMax);

			theGeo = trigeo;
			theDomain = xyDomain;
			theMinMaxMu = { muMin, muMax };
			theMinMaxNu = { nuMin, nuMax };
			theAtMuNu = { theMinMaxMu.first, theMinMaxNu.first };
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
	std::pair<long, long>
	operator*
		() const
	{
		return theAtMuNu;
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

		oss << "muMinMax: " << dat::infoString(theMinMaxMu);

		oss << std::endl;
		oss << "nuMinMax: " << dat::infoString(theMinMaxNu);

		oss << std::endl;
		if (dat::isValid(theAtMuNu))
		{
			oss << "atMuNu: " << dat::infoString(theAtMuNu);
		}
		else
		{
			oss << "theAtMuNu: " << "<null>";
		}

		oss << std::endl;
		oss << "active: " << dat::infoString(theIsActive);

		return oss.str();
	}

private:

	//! True if xyLoc is within domain area
	inline
	bool
	atNodeIsValid
		() const
	{
		double const mu{ theGeo.muFromIndex(theAtMuNu.first) };
		double const nu{ theGeo.nuFromIndex(theAtMuNu.second) };
		dat::Spot const mnLoc{{ mu, nu }};
		return theDomain.contains(theGeo.xyLocForMuNu(mnLoc));
	}

	//! Increment theAtMuNu to next node (whether valid or not)
	inline
	void
	advanceToNextAny
		()
	{
		long & ndxAtMu = theAtMuNu.first;
		long & ndxAtNu = theAtMuNu.second;
		++ndxAtNu;
		if (theMinMaxNu.second < ndxAtNu)
		{
			ndxAtNu = theMinMaxNu.first;
			++ndxAtMu;
			if (theMinMaxMu.second < ndxAtMu)
			{
				theIsActive = false;
				ndxAtMu = dat::nullValue<long>();
				ndxAtNu = dat::nullValue<long>();
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

