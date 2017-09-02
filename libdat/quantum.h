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

#ifndef dat_quantum_INCL_
#define dat_quantum_INCL_

/*! \file
\brief Declarations for dat::quantum
*/


#include "libdat/template.h"

#include <cmath>
#include <utility>


namespace dat
{

/*! \brief Functions related to quantization operations.

\par Example
\dontinclude testdat/uquantum.cpp
\skip ExampleStart
\until ExampleEnd
*/

namespace quantum
{
	/*! Functor to split values into floor and fraction parts.
	 *
	 * NOTE: FracType must support fractional parts of a quanta.
	 *       E.g. a floating point type for integral BaseType.
	 *
	 */
	template
		< typename BaseType, typename FracType
		>
	struct Splitter
	{
		FracType theDelta; // { dat::nullValue<FracType>() };

		//! Split value into module and fraction parts
		inline
		std::pair<BaseType, FracType>
		operator()
			( FracType const & value
			) const;

		inline
		FracType
		value
			( std::pair<BaseType, FracType> const & qfParts
			) const
		{
			FracType const valueInQuanta
				{ static_cast<FracType>(qfParts.first) + qfParts.second };
			return (valueInQuanta * theDelta);
		}
	};


} // quantum

} // dat

// Inline definitions
#include "libdat/quantum.inl"

#endif // dat_quantum_INCL_

