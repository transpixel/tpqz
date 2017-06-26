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
\brief Inline definitions for sen::funcSenseOp
*/



namespace sen
{

namespace color
{
namespace
{
	//! Color associated with a model component
	inline
	model::atom::Color
	atomFor
		( model::Part const & partInSen
		, geo::Ray const & // rayInSen // not used here, but could be
		)
	{
		return partInSen.theRadTraits.theColor;
	}
}

inline
model::atom::Color
SenseOp :: operator()
	( model::Part const & partInSen
	, geo::Ray const & rayInSen
	) const
{
	return atomFor(partInSen, rayInSen);
}

}

namespace range
{

namespace
{
	//! Range associated with a model component
	inline
	model::atom::Range
	atomFor
		( model::Part const & partInSen
		, geo::Ray const & rayInSen
		)
	{
		ga::Vector const & locPart = partInSen.theLoc;  
		ga::Vector const & locOrig = rayInSen.theStart;
		double const dist{ ga::magnitude(locPart - locOrig) };
		return dist;
	}
}

inline
model::atom::Range
SenseOp :: operator()
	( model::Part const & partInSen
	, geo::Ray const & rayInSen
	) const
{
	return atomFor(partInSen, rayInSen);
}

}

namespace point
{

namespace
{
	//! Range associated with a model component
	inline
	model::atom::Point
	atomFor
		( model::Part const & partInSen
		, geo::Ray const & // rayInSen
		)
	{
		return partInSen.theLoc;
	}
}

inline
model::atom::Point
SenseOp :: operator()
	( model::Part const & partInSen
	, geo::Ray const & rayInSen
	) const
{
	return atomFor(partInSen, rayInSen);
}

}


}

