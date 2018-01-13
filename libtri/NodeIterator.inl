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
\brief Inline definitions for tri::NodeIterator
*/



namespace tri
{

inline
// static
NodeKey
NodeIterator :: nextNodeMu
	( NodeKey const & keyIJ
	)
{
	return { keyIJ.first + 1L, keyIJ.second };
}

inline
// static
NodeKey
NodeIterator :: nextNodeNu
	( NodeKey const & keyIJ
	)
{
	return { keyIJ.first, keyIJ.second + 1L };
}

inline
// static
NodeKey
NodeIterator :: nextNodeDi
	( NodeKey const & keyIJ
	)
{
	return { keyIJ.first + 1L, keyIJ.second + 1L };
}

inline
// static
NodeKey
NodeIterator :: prevNodeMu
	( NodeKey const & keyIJ
	)
{
	return { keyIJ.first - 1L, keyIJ.second };
}

inline
// static
NodeKey
NodeIterator :: prevNodeNu
	( NodeKey const & keyIJ
	)
{
	return { keyIJ.first, keyIJ.second - 1L };
}

// static
NodeKey
NodeIterator :: prevNodeDi
	( NodeKey const & keyIJ
	)
{
	return { keyIJ.first - 1L, keyIJ.second - 1L };
}

inline
// explicit
NodeIterator :: operator bool
	() const
{
	return theIsActive;
}

inline
IsoGeo::QuantPair
NodeIterator :: fracPair
	() const
{
	return fracPairAt();
}

inline
NodeKey
NodeIterator :: nodeKey
	() const
{
	return { theAtIJ.first, theAtIJ.second };
}

inline
NodeKey
NodeIterator :: nextNodeMu
	() const
{
	return nextNodeMu(theAtIJ);
}

inline
NodeKey
NodeIterator :: nextNodeNu
	() const
{
	return nextNodeNu(theAtIJ);
}

inline
NodeKey
NodeIterator :: nextNodeDi
	() const
{
	return nextNodeDi(theAtIJ);
}

inline
NodeKey
NodeIterator :: prevNodeMu
	() const
{
	return prevNodeMu(theAtIJ);
}

inline
NodeKey
NodeIterator :: prevNodeNu
	() const
{
	return prevNodeNu(theAtIJ);
}

inline
NodeKey
NodeIterator :: prevNodeDi
	() const
{
	return prevNodeDi(theAtIJ);
}

inline
NodeIterator &
NodeIterator :: operator++
	()
{
	advanceToNextValid();
	return *this;
}

inline
IsoGeo::QuantPair
NodeIterator :: fracPairAt
	() const
{
	return theTileGeo.fracPairForNodeKey(theAtIJ);
}

inline
bool
NodeIterator :: atNodeIsValid
	() const
{
	dat::Spot const xyLoc(theTileGeo.refSpotForFracPair(fracPairAt()));
	return theDomain.contains(xyLoc);
}

inline
void
NodeIterator :: advanceToNextAny
	()
{
	NodeNdxType & ndxAtI = theAtIJ.first;
	NodeNdxType & ndxAtJ = theAtIJ.second;
	++ndxAtJ;
	if (theBegEndJ.second < ndxAtJ)
	{
		ndxAtJ = theBegEndJ.first;
		++ndxAtI;
		if (theBegEndI.second < ndxAtI)
		{
			theIsActive = false;
			ndxAtI = tri::sNullNdx;
			ndxAtJ = tri::sNullNdx;
		}
	}
}

inline
void
NodeIterator :: advanceToNextValid
	()
{
	do
	{
		advanceToNextAny();
	}
	while (theIsActive && (! atNodeIsValid()));
}

} // tri

