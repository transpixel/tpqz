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

#ifndef la_eigen_INCL_
#define la_eigen_INCL_

/*! \file
\brief Declarations for la::eigen
*/


#include "libdat/grid.h"

// for whatever reason, clang(++-7) complains about the GCC pragmas
#if defined(__clang__)
#pragma clang diagnostic ignored "-Wunknown-warning-option"
#endif

#if defined(__GNUC__)
// Enum as int seems to be a error in eigen3 at least through v3.3.4
// Apparently has been addressed in eigen development (at 3.3.90) but
// to use older packages, easier to just disable warning here.
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wint-in-bool-context"
#include "Eigen/Eigen"
#pragma GCC diagnostic pop
#endif


namespace la
{

/*! \brief Eigen::Map access into other data struct types.

\par Example
\dontinclude testla/ueigen.cpp
\skip ExampleStart
\until ExampleEnd
*/

namespace eigen
{
	//! Dynamically allocated eigen matrix
	template <typename ElemType>
		using Matrix_t = typename Eigen::Matrix
			<ElemType, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>;

	//! Behaves like Eigen::Matrix with readonly data content
	template <typename ElemType>
		using ConstMap = Eigen::Map<Matrix_t<ElemType> const>;

	//! Behaves like Eigen::Matrix with writeable data content
	template <typename ElemType>
		using WriteMap = Eigen::Map<Matrix_t<ElemType> >;


	/*! Eigen::Matrix compatible readonly access to grid
	 *
	 * Treat return type as an Eigen::Matrix (is actually Eigen::Map)
	 */
	template <typename ElemType>
	inline
	ConstMap<ElemType>
	withGrid
		( dat::grid<ElemType> const & grid
		);

	/*! Eigen::Matrix compatible write/read access to grid
	 *
	 * Treat return type as an Eigen::Matrix (is actually Eigen::Map)
	 */
	template <typename ElemType>
	inline
	WriteMap<ElemType>
	withGrid
		( dat::grid<ElemType> * const & ptGrid
			//!< NOTE! must have preallocated storage space!
		);

} // eigen

} // la

// Inline definitions
#include "libla/eigen.inl"

#endif // la_eigen_INCL_

