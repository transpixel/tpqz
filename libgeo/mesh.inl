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
\brief Inline definitions for geo::mesh
*/


namespace geo
{
namespace mesh
{

template <typename FwdIter>
inline
std::string
stringFor
	( FwdIter const & vectorBeg
	, FwdIter const & vectorEnd
	, std::string const & fmt
	, char const & sep
	)
{
	std::ostringstream oss;
	for (FwdIter iter{vectorBeg} ; vectorEnd != iter ; ++iter)
	{
		if (vectorBeg != iter)
		{
			oss << sep;
		}
		ga::Vector const & pnt = *iter;
		oss
			<< ::io::sprintf(fmt, pnt[0])
			<< " " 
			<< ::io::sprintf(fmt, pnt[1])
			<< " " 
			<< ::io::sprintf(fmt, pnt[2])
			;
	}
	return oss.str();
}

}
}

