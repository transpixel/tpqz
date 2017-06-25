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

#ifndef sig_Peak_INCL_
#define sig_Peak_INCL_

/*! \file
\brief Declarations for sig::Peak
*/


#include "libdat/grid.h"
#include "libdat/RowCol.h"
#include "libdat/validity.h"

#include <string>
#include <vector>


namespace sig
{

/*! \brief Representation of a peak in raster data

\par Example
\dontinclude testsig/uPeak.cpp
\skip ExampleStart
\until ExampleEnd
*/

class Peak
{

public: // data

	// Primary peak (if it exists)
	dat::RowCol theBestRowCol;
	double theBestScore;

	// Secondary - isolated - peak
	dat::RowCol theElseRowCol;
	double theElseScore;

	// Neighborhood of primary peak
	double theHoodRadius;
	std::vector<double> theHoodScores; //!< first is 2nd best, etc...

public: // static methods

	//! Extract peak from scoring grid
	static
	Peak
	fromGrid
		( dat::grid<double> const & scoreGrid
		, double const & usePeakRadius = 1.5
		);

	//! Location (subpixel) of peak in score grid
	static
	dat::Spot
	fitSpotFor
		( dat::grid<double> const & scoreGrid
		, Peak const & nomPeak = {}
		, double const & usePeakRadius = 1.5
		);

public: // methods

	//! default null constructor
	Peak
		();

	//! True if all peak is defined (best values are valid)
	bool
	isValid
		() const;

	//! Distinction of the peak on scale of [0,1]
	double
	prominenceRank
		() const;

	//! Dominance ratio [0,1] of bestPeak w.r.t. elsePeak, == (1.-else/best)
	double
	ambiguityRatio
		() const;

	//! Descriptive information about this instance
	std::string
	infoString
		( std::string const & title = std::string()
		) const;

	//! Detailed information about this instance
	std::string
	infoStringDetail
		( std::string const & title
		) const;

private:

	//! Size of neighborhood used for peak issolation
	size_t
	hoodArea
		() const;

};

}

// Inline definitions
// #include "libsig/Peak.inl"

#endif // sig_Peak_INCL_

