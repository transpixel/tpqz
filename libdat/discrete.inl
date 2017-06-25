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
\brief Declarations for dat::discrete
*/


namespace dat
{
namespace discrete
{

inline
size_t
quantizedLo
	( size_t const & orig
	, size_t const & delta
	)
{
	size_t quant{ dat::nullValue<size_t>() };
	if (0u < delta)
	{
		size_t const numFull(orig / delta);
		quant =  numFull * delta;
	}
	return quant;
}

inline
size_t
quantizedHi
	( size_t const & orig
	, size_t const & delta
	)
{
	size_t quant{ dat::nullValue<size_t>() };
	if (0u < delta)
	{
		size_t const numLeft{ orig % delta };
		if (0u == numLeft)
		{
			quant = orig;
		}
		else
		{
			size_t const numFull{ orig / delta };
			quant = (numFull + 1u) * delta;
		}
	}
	return quant;
}

inline
dat::RowCol
quantized
	( dat::RowCol const & orig
	, size_t const & quant
	)
{
	return
		{{ quantizedLo(orig[0], quant)
		 , quantizedLo(orig[1], quant)
		}};
}

inline
dat::Extents
quantized
	( dat::Extents const & orig
	, size_t const & quant
	)
{
	return dat::Extents
		( quantizedHi(orig[0], quant)
		, quantizedHi(orig[1], quant)
		);
}

}
}

