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


/*! \file
\brief Inline definitions for prob::io
*/


#include "libio/sprintf.h"

#include <fstream>


template <size_t Dim>
inline
void
prob::io :: saveToFile
	( std::array<size_t, Dim> const & hist
	, std::string const & fpath
	)
{
	std::ofstream ofs(fpath);

	// compute total count
	size_t const sumAll(std::accumulate(hist.begin(), hist.end(), size_t(0u)));
	double const total(std::max(1., (double)sumAll)); // max: trick 4 zero case

	ofs << "# BinValue Count Fraction Cummulative" << std::endl;
	size_t const numBins(hist.size());
	double cdf(0.);
	for (size_t nn(0u) ; nn < numBins ; ++nn)
	{
		size_t const & count = hist[nn];
		double const frac((double)count / total);
		cdf += frac;
		ofs
			<< "  " << ::io::sprintf("%3d", nn)
			<< "  " << ::io::sprintf("%12d", count)
			<< "  " << ::io::sprintf("%9.6f", frac)
			<< "  " << ::io::sprintf("%9.6f", cdf)
			<< std::endl;
	}
	ofs << std::endl;
	ofs << std::endl;
	ofs << std::endl;

	return;
}

