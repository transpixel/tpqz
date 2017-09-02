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
		);

	//! True if iterator is currently active (not at end)
	inline
	explicit
	operator bool
		() const;

	//! Node location (index+resid) associated with current (valid) iterator
	inline
	IsoGeo::QuantPair
	fracPair
		() const;

	//! Index pair associated with current (valid) iterator
	inline
	std::pair<long, long>
	indexPair
		() const;

	//! Advance to next valid node (node in the domain)
	inline
	NodeIterator &
	operator++
		();

	//! Descriptive information about this instance.
	std::string
	infoString
		( std::string const & title = std::string()
		) const;

private:

	//! QuantPair for current position
	inline
	IsoGeo::QuantPair
	fracPairAt
		() const;

	//! True if xyLoc is within domain area
	inline
	bool
	atNodeIsValid
		() const;

	//! Increment theAtIJ to next node (whether valid or not)
	inline
	void
	advanceToNextAny
		();

	//! Increment iterator until pointing to next valid instance (or end)
	inline
	void
	advanceToNextValid
		();

}; // NodeIterator

} // tri

// Inline definitions
#include "libtri/NodeIterator.inl"

#endif // tri_NodeIterator_INCL_

