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

public: // types

	using NodeNdxRange = std::pair<tri::NodeNdxType, tri::NodeNdxType>;

	//! STL style [begin,end) iterator pairs for tessellation geometry.
	struct IndexLimits
	{
		IsoGeo::QuantPair theFracPairBeg{};
		IsoGeo::QuantPair theFracPairEnd{};

		IndexLimits
			() = default;

		//! Construct indices into trigeo which span *entire* mnArea.
		explicit
		IndexLimits
			( tri::IsoGeo const & trigeo
			, dat::Area<double> const & mnArea
			);

		//! Start and (one-past) end iterator for 'mu/u' direction.
		NodeNdxRange
		ndxBegEndI
			() const;

		//! Start and (one-past) end iterator for 'nu/v' direction.
		NodeNdxRange
		ndxBegEndJ
			() const;

		//! Descriptive information about this instance.
		std::string
		infoString
			( std::string const & title
			) const;
	};

public: // data

	//! Tritille geometry (used to compute indices)
	IsoGeo theTileGeo{};

	//! Tritille domain of definition (used to evaluate validity)
	Domain theDomain{};

	//! STL-style iterators into all possible nodes
	IndexLimits theNdxLimits{};

private: // data

	//! Node indices ala STL style iterators (for mu)
	NodeNdxRange theBegEndI{ tri::sNullNdxPair };

	//! Node indices ala STL style iterators (for nu)
	NodeNdxRange theBegEndJ{ tri::sNullNdxPair };

	//! Indices to currently active node (or invalid values)
	NodeKey theAtIJ{ tri::sNullNdxPair };

	//! True if current iterator position is valid (not yet at end)
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
	NodeKey
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

