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
\brief Inline definitions for io::sprintf
*/

#include "utf8.h"

#include <algorithm>
#include <cctype>
#include <iterator>
#include <sstream>
#include <vector>

//
// sprintf
//
template < class Type >
inline
std::string
io :: sprintf
	( std::string const & fmt
	, Type const & data
	)
{
	// output stream
	std::ostringstream oss;

	// wrap format in input stream
	std::istringstream iss(fmt);

	// Eat up characters until a format 'tag' is hit
	char cval;
	while (iss.get(cval))
	{
		if (cval == '%')
		{
			// double %% converts to '%'
			if (iss.peek() == '%')
			{
				iss.get(cval);
			}
			else // beginning of format characters
			{
				break;
			}
		}

		unsigned seqlen(io::sequenceLength8(cval));
		oss.put(cval);
		while (--seqlen && iss.get(cval))
		{
			oss.put(cval);
		}
	}

	// Now check for formatting characters
	bool more = true;
	bool alternate = false;

	while (more && iss.get(cval))
	{
		switch (cval)
		{
			case '+':
				oss << std::showpos;
			break;
			case '-':
				oss << std::left;
			break;
			case '0':
				oss << std::internal;
				oss.fill('0');
			break;
			case '#':
				alternate = true;
			break;
			case ' ':
				oss.fill(' '); // default
			break;
			default: // put this character back on the stream
				more = false;
				iss.putback(cval);
			break;
		}
	}

	// check for field width specification
	if (iss.good() && std::isdigit(iss.peek()))
	{
		// chew up width values
		std::streamsize wid;
		iss >> wid;
		oss.width(wid);
	}

	// check for precision
	if (iss.good() && iss.peek() == '.')
	{
		// eat the '.'
		iss.get(cval);

		// chew up precision values
		std::streamsize prec;
		iss >> prec;
		oss.precision(prec);
	}

	// check for format character
	if (iss.get(cval))
	{
		switch (cval)
		{
			case 'd':
				oss << std::dec;
			break;
			case 'x':
				oss << std::hex;
				if (alternate)
				{
					oss << std::showbase;
				}
			break;
			case 'X':
				oss << std::hex << std::uppercase;
				if (alternate)
				{
					oss << std::showbase;
				}
			break;
			case 'o':
				oss << std::oct;
				if (alternate)
				{
					oss << std::showbase;
				}
			break;
			case 'f':
				oss << std::fixed;
				if (alternate)
				{
					oss << std::showpoint;
				}
			break;
			case 'e':
				oss << std::scientific;
				if (alternate)
				{
					oss << std::showpoint;
				}
			break;
			case 'E':
				oss << std::scientific << std::uppercase;
				if (alternate)
				{
					oss << std::showpoint;
				}
			break;
			case 'g':
				if (alternate)
				{
					oss << std::showpoint;
				}
			break;
			case 'G':
				oss << std::uppercase;
				if (alternate)
				{
					oss << std::showpoint;
				}
			break;
			default:
			break;
		}
	}

	// output data value
	oss << data;

	// eat the rest of the characters
	while (iss.get(cval))
	{
		oss.put(cval);
	}

	return oss.str();
}

//
// repeat
//
inline
std::string
io :: repeat
	( std::string const & val
	, unsigned const & times
	)
{
	std::string res;
	res.reserve(val.size() * times);
	for (unsigned index(0) ; index < times ; ++index)
	{
		res += val;
	}
	return res;
}

//
// repeat
//
inline
std::string
io :: repeat
	( char const & val
	, unsigned const & times
	)
{
	return std::string(times, val);
}

//
// formatThousands
//
template < typename Type >
inline
std::string
io :: formatThousands
	( Type const & val
	, std::string const & sep
	)
{
	std::ostringstream oss;
	std::vector<int> vals;

	Type aval(std::abs(val));
	bool const neg(aval != val);

	do
	{
		vals.push_back(static_cast<int>(aval % 1000));
		aval /= 1000;
	} while (aval != 0);

	if (neg)
	{
		vals.back() = -vals.back();
	}

	std::vector<int>::const_reverse_iterator itv(vals.rbegin());
	std::vector<int>::const_reverse_iterator itend(vals.rend());
	oss << *itv;
	while (++itv != itend)
	{
		oss << sep << io::sprintf("%03d", *itv);
	}

	return oss.str();
}

//
// quote
//
inline
std::string
io :: quote
	( std::string const & value
	, std::string const & str
	)
{
	std::string res(value.size() + 2 * str.size(), '\0');

	std::copy(str.begin(), str.end(), res.begin());
	std::copy(str.begin(), str.end(), res.rbegin());
	std::copy(value.begin(), value.end(), res.begin() + str.size());

	return res;
}
