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

#ifndef prob_histo_INCL_
#define prob_histo_INCL_

/*! \file
\brief Declarations for prob::histo
*/


#include "libmath/Partition.h"

#include <array>
#include <cstddef>
#include <string>


namespace prob
{

//! \brief Probability functions/operations related to histograms.
namespace histo
{
	//! Histogram from a buffer of uint8_t values
	template <typename FwdIter>
	inline
	std::array<size_t, 256u>
	countsForUint8
		( FwdIter const & begin //!< *iter should be a uint8_t
		, FwdIter const & end
		);

	//! First bin in hist with non-zero element
	inline
	size_t
	minBinNotZero
		( std::array<size_t, 256u> const & hist
		);

	//! Last element in bin with non-zero element
	inline
	size_t
	maxBinNotZero
		( std::array<size_t, 256u> const & hist
		);

	//! Histogram from samples: bin layout defined by partition
	template <typename FwdIter>
	inline
	std::vector<size_t>
	countsFromSamps
		( FwdIter const & sampBeg //!< *iter should be castable to double
		, FwdIter const & sampEnd
		, math::Partition const & part
		);

	//! Shortcut access to above
	template <typename Collection>
	inline
	std::vector<size_t>
	countsFromSamps
		( Collection const & samps //!< *iter should be castable to double
		, math::Partition const & part
		);
}

}

// Inline definitions
#include "libprob/histo.inl"

#endif // prob_histo_INCL_

