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
\brief Inline definitions for dat::BestOf
*/


#include "libdat/info.h"
#include "libio/stream.h"

#include <cassert>
#include <vector>
#include <iterator>
#include <sstream>


namespace dat
{


template <typename Type, typename Compare>
inline
// explicit
BestOf<Type, Compare> :: BestOf
	( size_t const & numBest
	)
	: theNumBest{ numBest }
	, theItems{}
{
}

template <typename Type, typename Compare>
inline
bool
BestOf<Type, Compare> :: isValid
	() const
{
	return dat::isValid(theNumBest);
}

template <typename Type, typename Compare>
inline
size_t
BestOf<Type, Compare> :: capacity
	() const
{
	return theNumBest;
}

template <typename Type, typename Compare>
inline
size_t
BestOf<Type, Compare> :: size
	() const
{
	return theItems.size();
}

template <typename Type, typename Compare>
inline
bool
BestOf<Type, Compare> :: addSample
	( Type const & item
	)
{
	bool wasAdded{ false };

	bool addItem{ false };
	if (theItems.size() < theNumBest)
	{
		addItem = true;
	}
	else
	{
		Type const & tail = *(theItems.rbegin());
		bool beatsTail{ theCompFunc(item, tail) };
		if (beatsTail)
		{
			addItem = true;
		}
	}
	
	if (addItem)
	{
		theItems.insert(item);
		wasAdded = true;

		// enforce size limit
		if (theNumBest < size())
		{
			theItems.erase(std::prev(theItems.end()));
		}
	}

	return wasAdded;
}

template <typename Type, typename Compare>
template <typename FwdIter>
inline
bool
BestOf<Type, Compare> :: addSamples
	( FwdIter const & itBeg
	, FwdIter const & itEnd
	)
{
	bool anyAdded{ false };
	for (FwdIter iter{itBeg} ; itEnd != iter ; ++iter)
	{
		anyAdded |= addSample(*iter);
	}
	return anyAdded;
}

template <typename Type, typename Compare>
inline
std::vector<Type>
BestOf<Type, Compare> :: bestItems
	() const
{
	std::vector<Type> const values(theItems.begin(), theItems.end());
	return values;
}

template <typename Type, typename Compare>
inline
std::string
BestOf<Type, Compare> :: infoString
	( std::string const & title
	) const
{
	std::ostringstream oss;
	if (! title.empty())
	{
		oss << title << " ";
	}
	if (isValid())
	{
		oss
			<< " " << dat::infoString(capacity(), "capacity()")
			<< " " << dat::infoString(size(), "size()")
			;
	}
	else
	{
		oss << " <null>";
	}
	return oss.str();
}

template <typename Type, typename Compare>
inline
std::string
BestOf<Type, Compare> :: infoStringContents
	( std::string const & title
	) const
{
	std::ostringstream oss;
	oss << infoString(title);
	if (isValid())
	{
		for (Type const & item : theItems)
		{
			oss << std::endl;
			oss << dat::infoString(item, "item");
		}
	}
	return oss.str();
}


} // dat

