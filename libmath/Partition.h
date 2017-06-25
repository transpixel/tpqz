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

#ifndef math_Partition_INCL_
#define math_Partition_INCL_

/*! \file
\brief Declarations for math::Partition
*/


#include "libdat/limits.h"
#include "libdat/Range.h"

#include <string>


namespace math
{

/*! \brief Representation interval divided into equally sized parts.

\par Example
\dontinclude testmath/uPartition.cpp
\skip ExampleStart
\until ExampleEnd
*/

class Partition
{
	size_t theNumParts;
	double theMin;
	double theDelta;

public: // static methods

	//! Construct partition that spans the data (*IN*cluding last value)
	template <typename FwdIter>
	inline
	static
	Partition
	spanning
		( FwdIter const & beg
		, FwdIter const & end
		, size_t const & numParts
		, double const & endEpsilon = dat::smallValue<double>()
			//! Bump range end-point this much past largest data value
		);

public: // methods

	//! Construct an invalid instance
	Partition
		();

	//! value ctor
	explicit
	Partition
		( double const & min
		, double const & delta
		, size_t const & numParts
		);

	//! Convenience ctor - using indirect values
	explicit
	Partition
		( dat::Range<double> const & range
		, size_t const & numParts
		);

	//! Convenience ctor - using indirect values
	explicit
	Partition
		( std::pair<double, double> const & pair
		, size_t const & numParts
		);

	// copy constructor -- compiler provided
	// assignment operator -- compiler provided
	// destructor -- compiler provided

	//! True if not null
	inline
	bool
	isValid
		() const;

	//! Number of intervals in this partition
	inline
	size_t
	size
		() const;

	//! Minimum value IN-cluded
	inline
	double
	min
		() const;

	//! Maximum value EX-cluded
	inline
	double
	max
		() const;

	//! Range of entire partition
	inline
	dat::Range<double>
	range
		() const;

	//! Range of values associated with a particular bin
	inline
	dat::Range<double>
	rangeForBin
		( size_t const & ndx
		) const;

	//! Index into partition associated with value
	inline
	size_t
	binIndexFor
		( double const & value
		) const;

	//! Value into partition associated with (fractional) subIndex
	inline
	double
	interpValueFor
		( double const & subIndex
		) const;

	//! Index into partition associated with value
	inline
	double
	interpIndexFor
		( double const & value
		) const;

	//! Discrete index (static_cast of interpIndexFor())
	template<typename IType>
	inline
	IType
	indexFor
		( double const & value
		) const;

	//! True if partitions have same number of elements and ~= end points
	bool
	nearlyEquals
		( Partition const & other
		, double const & tol = dat::smallValue<double>()
		) const;

	//! Descriptive information about this instance.
	std::string
	infoString
		( std::string const & title = std::string()
		) const;
};

}

// Inline definitions
#include "libmath/Partition.inl"

#endif // math_Partition_INCL_

