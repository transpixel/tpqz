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


/*! \file
\brief Definitions for ga::io
*/


#include "libga/io.h"

#include <fstream>


namespace ga
{
namespace io
{

bool
saveToAscii
	( std::vector<ga::Vector> const & points
	, std::string const & fpath
	)
{
	std::ofstream ofs(fpath);

	ofs << "# \n";
	ofs << "# <x y z>\n";
	ofs << "# \n";

	for (ga::Vector const & point : points)
	{
		if (dat::isValid(point))
		{
			ofs << "  ";
		}
		else
		{
			ofs << "# ";
		}
		ofs << dat::infoString(point) << '\n';
	}

	ofs << std::endl;
	return (! ofs.fail());
}

std::vector<ga::Vector>
fromAscii
	( std::string const & fpath
	)
{
	std::vector<ga::Vector> points;
	std::ifstream ifs(fpath);

	std::string line;
	double xx{}, yy{}, zz{};
	while (ifs && (! ifs.eof()))
	{
		std::getline(ifs, line);
		if (! line.empty())
		{
			std::istringstream iss(line);
			iss >> xx >> yy >> zz;
			bool const okay{ (! iss.fail()) };
			if ( okay
			  && dat::isValid(xx)
			  && dat::isValid(yy)
			  && dat::isValid(zz)
			   )
			{
				points.emplace_back(ga::Vector(xx, yy, zz));
			}
		}
	}

	return points;
}

} // io
} // ga

