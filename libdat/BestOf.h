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

#include <set>
#include <string>


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
	std::multiset<Type, Compare > theItems;
	Compare theCompFunc;

public: // methods

	//! default null constructor
	BestOf
		() = default;

	//! Configure to select numBest objects
	inline
	explicit
	BestOf
		( size_t const & numBest
		);

	//! True if instance is valid
	inline
	bool
	isValid
		() const;

	//! The number of objects currently being tracked
	inline
	size_t
	capacity
		() const;

	//! The number of objects currently being tracked
	inline
	size_t
	size
		() const;

	/*! Add object to tracking collection if "better" - true if added
	 *
	 * \arg "better" means (item < allTrackedItems)
	 *
	 */
	inline
	bool
	addSample
		( Type const & item
		);

	//! Process collection of objects with addSample()
	template <typename FwdIter>
	inline
	bool
	addSamples
		( FwdIter const & itBeg //!< *iter == Type
		, FwdIter const & itEnd
		);

	//! Best items in order CompFunc(item[nn], item[nn+1])
	inline
	std::vector<Type>
	bestItems
		() const;

	//! Descriptive information about this instance.
	inline
	std::string
	infoString
		( std::string const & title = std::string()
		) const;

	//! Descriptive information about this instance.
	inline
	std::string
	infoStringContents
		( std::string const & title = std::string()
		) const;

}; // BestOf

} // dat

// Inline definitions
#include "libdat/BestOf.inl"

#endif // dat_BestOf_INCL_

