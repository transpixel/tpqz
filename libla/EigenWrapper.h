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

#ifndef la_EigenWrapper_INCL_
#define la_EigenWrapper_INCL_

/*! \file
\brief Declarations for la::EigenWrapper
*/


#include "libdat/compare.h"
#include "libdat/Extents.h"
#include "libdat/info.h"
#include "libio/sprintf.h"

#include "Eigen"

#include <cassert>
#include <cstddef>
#include <sstream>


namespace la
{

/*! \brief Eigen::MatrixXd wrapper providing methods compatible with code base.

\par Example
\dontinclude testla/uEigenWrapper.cpp
\skip ExampleStart
\until ExampleEnd
*/

template <int DimRow, int DimCol>
struct EigenWrapper
{
	//! Eigen structure
	Eigen::Matrix<double, DimRow, DimCol> theMat{};

	//! default null constructor
	EigenWrapper
		() = default;

	//! Value ctor from Eigen::MatrixXd
	inline
	explicit
	EigenWrapper
		( Eigen::Matrix<double, DimRow, DimCol> const & emat
		);

	//! Value construct column (Nx1) vector with initializer-list syntax
	inline
	explicit
	EigenWrapper
		( std::initializer_list<double> const & values
		);

	//! Value construct a matrix with list of lists
	inline
	explicit
	EigenWrapper
		( std::initializer_list<std::initializer_list<double> >
			const & rcValues
		);

	//! Synatx compatible with dat::grid
	inline
	size_t
	high
		() const;

	//! Synatx compatible with dat::grid
	inline
	size_t
	wide
		() const;

	//! Extents of this matrix
	inline
	dat::Extents
	hwSize
		() const;

	//! Element read access
	inline
	double const &
	operator()
		( int const & row
		, int const & col
		) const;

	//! Element write access
	inline
	double &
	operator()
		( int const & row
		, int const & col
		);

	//! Synatx compatible with STL and dat::grid
	inline
	double *
	begin
		();

	//! Synatx compatible with STL and dat::grid
	inline
	double *
	end
		();

	//! Synatx compatible with STL and dat::grid
	inline
	double const *
	begin
		() const;

	//! Synatx compatible with STL and dat::grid
	inline
	double const *
	end
		() const;

	//! Check if instance is valid
	inline
	bool
	isValid
		() const;

	//! True if this instance is same size and all elements same within tol
	inline
	bool
	nearlyEquals
		( EigenWrapper const & other
		, double const & tol = dat::smallValue<double>()
		) const;

	//! Descriptive information about this instance.
	inline
	std::string
	infoString
		( std::string const & title = std::string()
		) const;

	//! Descriptive information about the contents of this instance.
	inline
	std::string
	infoStringContents
		( std::string const & title = std::string()
		, std::string const & fmt = "%9.6f"
		) const;

};

}

// Inline definitions
#include "libla/EigenWrapper.inl"

#endif // la_EigenWrapper_INCL_

