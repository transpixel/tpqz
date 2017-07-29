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

#ifndef blk_OriPair_INCL_
#define blk_OriPair_INCL_

/*! \file
\brief Declarations for blk::OriPair
*/


#include "libblk/blk.h"
#include "libdat/validity.h"
#include "libga/Rigid.h"

#include <string>
#include <sstream>
#include <utility>


namespace blk
{

/*! \brief Wrapper around indexed pairwise rigid orientation.

\par Example
\dontinclude testro/uBlock.cpp
\skip ExampleStart
\until ExampleEnd
*/

class OriPair
{

private: // data

	std::pair<NodeNdx, NodeNdx> theNdxIJ
		{ dat::nullValue<NodeNdx>(), dat::nullValue<NodeNdx>() };
	ga::Rigid theOriJwI;

public: // methods

	//! default null constructor
	OriPair
		() = default;

	//! Value ctor...
	explicit
	OriPair
		( NodeNdx const & ndxI
		, NodeNdx const & ndxJ
		, ga::Rigid const oriJwrtI
		)
		: theNdxIJ{ ndxI, ndxJ }
		, theOriJwI{ oriJwrtI }
	{
	}

	// copy constructor -- compiler provided
	// assignment operator -- compiler provided
	// destructor -- compiler provided

	//! True if instance is valid
	bool
	isValid
		() const
	{
		return
			(  theOriJwI.isValid()
			&& dat::isValid(theNdxIJ)
			);
	}

	//! Domain (from) index
	NodeNdx const &
	ndxI
		() const
	{
		return theNdxIJ.first;
	}

	//! Range (into) index
	NodeNdx const &
	ndxJ
		() const
	{
		return theNdxIJ.second;
	}

	//! Attitude of range frame w.r.t. domain frame
	ga::Pose const &
	poseJwI
		() const
	{
		return theOriJwI.pose();
	}

	//! Offset of range frame w.r.t. domain frame
	ga::Vector const &
	baseJwI
		() const
	{
		return theOriJwI.location();
	}

	//! Pair with indices order enforced (ndxI < ndxJ)
	OriPair
	principalPair
		() const
	{
		OriPair ijPair;
		if (isValid())
		{
			if (ndxI() < ndxJ())
			{
				return *this;
			}
			else
			{
				return OriPair(ndxJ(), ndxI(), theOriJwI.inverse());
			}
		}
		return ijPair;
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
		if (isValid())
		{
			oss
				<< dat::infoString(theNdxIJ, "indices")
				<< '\n'
				<< dat::infoString(theOriJwI)
				;
		}
		else
		{
			oss << " <null>";
		}
		return oss.str();
	}

}; // OriPair

} // blk

// Inline definitions
// #include "libblk/OriPair.inl"

#endif // blk_OriPair_INCL_

