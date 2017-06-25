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


#ifndef    dat_grid_INCL_
#define    dat_grid_INCL_

/*! \file
\brief Declarations for dat::grid
*/

//#define PROXY

#include "libdat/dat.h"
#include "libdat/Extents.h"

#include <string>


namespace dat
{

/*! \brief Holds typed grid data with row/cols

Template data type.

Data is stored in row major order.

Iterations happen as (0, 0), (0, 1), (0, 2) etc. (along rows)

\par Example
\dontinclude testdat/ugrid.cpp
\skip ExampleStart
\until ExampleEnd
*/
template < typename Type >
class grid
{

public: // typedef

	typedef Type value_type;
	typedef Type * iterator;
	typedef Type const * const_iterator;
	typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
	typedef std::reverse_iterator<iterator> reverse_iterator;

public: // static methods

	//! A null (zero sized) grid
	static
	grid
	null
		()
	{
		return {};
	}

public: // methods

	//! construct empty
	grid
		() = default;

	//! Construct with geometry (NOTE: uninitialized values)
	inline
	explicit
	grid
		( size_t const & high
		, size_t const & wide
		);

	//! Construct with geometry (NOTE: uninitialized values)
	inline
	explicit
	grid
		( Extents const & hwSize
		);

	/*! \brief construct with geometry & default
	 * convenince function, calls "fill" internally
	 * */
	inline
	explicit
	grid
		( size_t const & high
		, size_t const & wide
		, Type const & fillValue
		);

	/*! \brief copy constructor
	 * performs a deep copy
	 * */
	inline
	grid
		( grid<Type> const & orig
		);

	//! deep copy
	inline
	grid<Type> &
	operator=
		( grid<Type> const & rhs
		);

	/*! \brief rvalue copy constructor
	 * performs a deep swap
	 * */
	inline
	grid
		( grid<Type> && orig
		);

	//! rvalue deep copy
	inline
	grid<Type> &
	operator=
		( grid<Type> && rhs
		);

	//! standard destructor
	inline
	~grid
		();

	//! check if is valid
	inline
	bool
	isValid
		() const;

	//! Dimensions of this image
	inline
	Extents
	hwSize
		() const;

	//! Height of buffer
	inline
	size_t const &
	high
		() const;

	//! Width of buffer
	inline
	size_t const &
	wide
		() const;

	//! Size of buffer (wide * high)
	inline
	size_t
	size
		() const;

	//! bytes in buffer
	inline
	size_t
	byteSize
		() const;

	//! Returns reference to element
	inline
	Type const &
	operator()
		( size_t const & row
		, size_t const & col
		) const;

	//! Returns pointer to row (non const)
	inline
	Type &
	operator()
		( size_t const & row
		, size_t const & col
		);

	//! Returns reference to element
	inline
	Type const &
	operator()
		( dat::RowCol const & rowcol
		) const;

	//! Returns pointer to row (non const)
	inline
	Type &
	operator()
		( dat::RowCol const & rowcol
		);

	//! use as const_iterator
	inline
	const_iterator
	begin
		() const;

	//! use as const_iterator
	inline
	const_iterator
	end
		() const;

	//! use as iterator
	inline
	iterator
	begin
		();

	//! use as iterator
	inline
	iterator
	end
		();

	//! use as const_reverse_iterator
	inline
	const_reverse_iterator
	rbegin
		() const;

	//! use as const_reverse_iterator
	inline
	const_reverse_iterator
	rend
		() const;

	//! use as reverse_iterator
	inline
	reverse_iterator
	rbegin
		();

	//! use as reverse_iterator
	inline
	reverse_iterator
	rend
		();

	//! const row access, use as const iterator
	inline
	const_iterator
	beginRow
		( size_t const & row
		) const;

	//! const row access, use as const iterator
	inline
	const_iterator
	endRow
		( size_t const & row
		) const;

	//! row access, use as iterator
	inline
	iterator
	beginRow
		( size_t const & row
		);

	//! row access, use as iterator
	inline
	iterator
	endRow
		( size_t const & row
		);

	//! iterator at given row, ocl
	inline
	const_iterator
	iterAt
		( size_t const & row
		, size_t const & col
		) const;

	//! iterator at given row, ocl
	inline
	iterator
	iterAt
		( size_t const & row
		, size_t const & col
		);

	//! row/col from iterator
	inline
	dat::RowCol
	rowColFor
		( const_iterator const & iter
		) const;

	//! Descriptive information about this instance.
	inline
	std::string
	infoString
		( std::string const & title=std::string()
		) const;

	//! contents of the buffer
	inline
	std::string
	infoStringContents
		( std::string const & title
		, std::string const & fmt
		) const;

	//! exactly equals operator
	inline
	bool
	operator==
		( grid const & rhs
		) const;

	//! Cast to different element type
	template <typename OutType>
	inline
	dat::grid<OutType>
	castGrid
		() const;

	//! Number of valid column entires in specified row
	inline
	size_t
	validCountInRow
		( size_t const & rowNdx
		) const;

	//! Number of valid rows entires in specified column
	inline
	size_t
	validCountInCol
		( size_t const & colNdx
		) const;


private:

	size_t theHigh{ 0u }; //!< Height of buffer
	size_t theWide{ 0u }; //!< Width of buffer
	Type * theData{ nullptr }; //!< data buffer pointer

	//! destructive resize
	inline
	void
	resize
		( size_t const & high
		, size_t const & wide
		);

};


}

// inline includes
#include "libdat/grid.inl"

#endif //  dat_grid_INCL_
