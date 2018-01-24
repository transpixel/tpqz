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

#ifndef dat_BestOf_INCL_
#define dat_BestOf_INCL_

/*! \file
\brief Declarations for dat::BestOf
*/


#include "libdat/validity.h"
#include "libdat/info.h" // TODO
#include "libio/stream.h" // TODO

#include <cassert>
#include <iterator>
#include <set>
#include <sstream>
#include <string>
#include <vector>


namespace dat
{

/*! \brief Collection comprised of the N "best" objects encountered

\par Example
\dontinclude testdat/uBestOf.cpp
\skip ExampleStart
\until ExampleEnd
*/

template <typename Type, typename Compare = std::greater<Type> >
class BestOf
{
	size_t theNumBest{ dat::nullValue<size_t>() };
//	std::set<Type, std::greater<double> > theItems;
	std::set<Type, Compare > theItems;

public: // methods

	//! default null constructor
	BestOf
		() = default;

	//! Configure to select numBest objects
	inline
	explicit
	BestOf
		( size_t const & numBest
		)
		: theNumBest{ numBest }
		, theItems{}
	{
	}

	//! True if instance is valid
	inline
	bool
	isValid
		() const
	{
		return dat::isValid(theNumBest);
	}

	//! The number of objects currently being tracked
	inline
	size_t
	capacity
		() const
	{
		return theNumBest;
	}

	//! The number of objects currently being tracked
	inline
	size_t
	size
		() const
	{
		return theItems.size();
	}

	/*! Add object to tracking collection if "better" - true if added
	 *
	 * \arg "better" means (item < allTrackedItems)
	 *
	 */
	inline
	bool
	addSample
		( Type const & item
		)
	{
		bool wasAdded{ false };

std::ostringstream oss;
oss << dat::infoString(item);
oss << "  ";

		bool addItem{ false };
		if (theItems.empty())
		{
			addItem = true;
oss << "zero:";
		}
		else
		{
oss << "init:";
			Type const & head = *(theItems.begin());
			Type const & tail = *(theItems.rbegin());
			if ((head < item) && (! (tail < item)))
			{
oss << "larger:";
				addItem = true;
			}
			else
			{
oss << "ignore:";
			}
		}
		
addItem = true;
		if (addItem)
		{
			theItems.insert(item);
			wasAdded = true;
oss << "added:";

			// enforce set size
			if (theNumBest < size())
			{
				theItems.erase(std::prev(theItems.end()));
oss << "rm:";
			}
else
{
oss << "--:";
}
		}
else
{
oss << "nooop:";
}

oss << "...:";
for (Type const & inHeap : theItems)
{
	oss << " " << dat::infoString(inHeap);
}
io::out() << oss.str() << std::endl;

		return wasAdded;
	}

	//! The ndx-th best object being tracked
	inline
	Type
	itemAt
		( size_t const & ndx //!< 0:best, 1:second-best, etc
		) const
	{
		Type item{};
		assert(ndx < theItems.size());
		{ // HACK - replace function
			std::vector<Type> sorted(theItems.begin(), theItems.end());
			item = sorted[ndx];
		}
		return item;
	}

	//! Descriptive information about this instance.
	inline
	std::string
	infoString
		( std::string const & title = std::string()
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

	//! Descriptive information about this instance.
	inline
	std::string
	infoStringContents
		( std::string const & title = std::string()
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

}; // BestOf

} // dat

// Inline definitions
// #include "libdat/BestOf.inl"

#endif // dat_BestOf_INCL_

