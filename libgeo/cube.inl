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
\brief Inline definitions for geo::cube
*/


namespace geo
{
namespace cube
{
//======================================================================

inline
size_t
faceIndex
	( ga::Vector const & dir
	)
{
	size_t ndx{ dat::nullValue<size_t>() };

	double const & xx = dir[0];
	double const & yy = dir[1];
	double const & zz = dir[2];

	// seems like there should be a faster way?
	double const px{ std::abs(xx) };
	double const py{ std::abs(yy) };
	double const pz{ std::abs(zz) };
	bool const is05{ (px < pz) && (py < pz) };

	if (is05) // +-Z
	{
		if (zz < 0.)
		{
			ndx = 0u;
		}
		else
		{
			ndx = 5u;
		}
	}
	else
	{
		bool is14( yy < xx);
		bool is12(-yy < xx);

		if (is14)
		{
			if (is12)
			{
				ndx = 1u;
			}
			else
			{
				ndx = 4u;
			}
		}
		else // 2,3
		{
			if (is12)
			{
				ndx = 2u;
			}
			else
			{
				ndx = 3u;
			}
		}
	}

	return ndx;
}

//======================================================================
}
}

