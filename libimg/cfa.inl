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
\brief Inline definitions for img::cfa
*/


namespace img
{
namespace cfa
{

template <typename Type>
inline
std::array<dat::grid<Type>, 4u>
channelsFromRGGB
	( dat::grid<Type> const & rawGrid
	)
{
	std::array<dat::grid<Type>, 4u> chans;

	// check pre-conditions
	assert(0u == (rawGrid.high() % 2u));
	assert(0u == (rawGrid.wide() % 2u));
	size_t const outHigh(rawGrid.high() / 2u);
	size_t const outWide(rawGrid.wide() / 2u);
	size_t const inWide(rawGrid.wide());

	// allocate space
	chans[0] = dat::grid<Type>(outHigh, outWide);
	chans[1] = dat::grid<Type>(outHigh, outWide);
	chans[2] = dat::grid<Type>(outHigh, outWide);
	chans[3] = dat::grid<Type>(outHigh, outWide);
	typename dat::grid<Type>::iterator itOut00(chans[0].begin());
	typename dat::grid<Type>::iterator itOut01(chans[1].begin());
	typename dat::grid<Type>::iterator itOut10(chans[2].begin());
	typename dat::grid<Type>::iterator itOut11(chans[3].begin());

	// demultiplex channels
	typename dat::grid<Type>::const_iterator itIn00(rawGrid.begin());
	typename dat::grid<Type>::const_iterator itIn01(itIn00 + 1u);
	typename dat::grid<Type>::const_iterator itIn10(itIn00 + inWide);
	typename dat::grid<Type>::const_iterator itIn11(itIn10 + 1u);

	for (size_t outRow(0u) ; outRow < outHigh ; ++outRow)
	{
		for (size_t outCol(0u) ; outCol < outWide ; ++outCol)
		{
			// grab channel data
			*itOut00++ = *itIn00++;
			*itOut01++ = *itIn01++;
			*itOut10++ = *itIn10++;
			*itOut11++ = *itIn11++;
			// advance to next CFA cell
			++itIn00;
			++itIn01;
			++itIn10;
			++itIn11;
		}
		// advance to next row of cells
		itIn00 += inWide;
		itIn01 += inWide;
		itIn10 += inWide;
		itIn11 += inWide;
	}

	return chans;
}

}
}

