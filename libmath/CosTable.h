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

#ifndef math_CosTable_INCL_
#define math_CosTable_INCL_

/*! \file
\brief Declarations for math::CosTable
*/


#include "libmath/Partition.h"

#include <string>

#include "libdat/info.h"
#include "libio/stream.h"
#include <iterator>
#include <sstream>


namespace math
{

/*! \brief Lookup table based spaced as the cosine of domain value.

\par Example
\dontinclude testmath/uCosTable.cpp
\skip ExampleStart
\until ExampleEnd
*/

template <typename Type>
class CosTable
{
	std::vector<Type> theTable{};
	math::Partition thePart{};
	Type theNull{};

//private: // disable (can be large)
//
//	//! Disable implicit copy and assignment
//	CosTable(CosTable const &) = delete;
//	CosTable & operator=(CosTable const &) = delete;

public: // methods

	//! Domain of validity [0,pi]
	inline
	static
	dat::Range<double>
	closedDomain
		();

	//! default null constructor
	CosTable
		() = default;

	//! Construct directly from collection of values
	template <typename FwdIter>
	inline
	explicit
	CosTable
		( FwdIter const & itBeg
		, FwdIter const & itEnd
		, Type const & nullValue
		);

	//! Construct empty table of given size (populate via insert)
	inline
	explicit
	CosTable
		( size_t const & numElem
		, Type const & nullValue
		);

	// copy constructor -- compiler provided
	// assignment operator -- compiler provided
	// destructor -- compiler provided

	//! Check if instance is valid
	inline
	bool
	isValid
		() const;

	//! Number of elements in LUT
	inline
	size_t
	size
		() const;

	//! Direct readonly access to table element
	inline
	Type const &
	operator[]
		( size_t const & ndx
		) const;

	//! Direct write access to table element
	inline
	Type &
	operator[]
		( size_t const & ndx
		);

	//! index into LUT for table value
	inline
	size_t
	indexForCosine
		( double const & cosValue
		) const;

	//! Cosine value associated with index
	inline
	double
	cosineForIndex
		( size_t const & index
		) const;

	//! Increment of cosine values between table elements
	inline
	double
	deltaCosine
		() const;

	/*
	//! Copy of table element determined from cosine value
	inline
	Type
	operator()
		( double const & cosValue
		) const;
	*/

	//! Put value into table element associated with cosAngle
	inline
	void
	assignValueAt
		( Type const & tabValue
		, double const & cosValue
		);

	//! Descriptive information about this instance.
	inline
	std::string
	infoString
		( std::string const & title = std::string()
		) const;

};

}

// Inline definitions
#include "libmath/CosTable.inl"

#endif // math_CosTable_INCL_

