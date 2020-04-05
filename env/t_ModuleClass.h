//
//
// MIT License
//
// Copyright (c) 2020 Stellacore Corporation.
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

#ifndef NM_CN_INCL_
#define NM_CN_INCL_

/*! \file
\brief Declarations for NM::CN
*/

#include <string>

namespace NM
{

/*! \brief XXX

\par Example
\dontinclude testNM/uCN.cpp
\skip ExampleStart
\until ExampleEnd
*/

class CN
{

private: // disable

	//! Disable implicit copy and assignment
	CN(CN const &) = delete;
	CN & operator=(CN const &) = delete;

public: // methods

	//! default null constructor
	CN
		() = default;

	//! XXX...
	explicit
	CN
		(
		);

	// copy constructor -- compiler provided
	// assignment operator -- compiler provided
	// destructor -- compiler provided

	//! True if instance is valid
	bool
	isValid
		() const;

	//! Descriptive information about this instance.
	std::string
	infoString
		( std::string const & title = {}
//		, std::string const & fmt = { std::string("%12.6f") }
		) const
	{
		std::ostringstream oss;
		if (! title.empty())
		{
			oss << title << std::endl;
		}
		if (isValid())
		{
		}
		else
		{
			oss << " <null>";
		}
		return oss.str();
	}

}; // CN

} // NM

// Inline definitions
// #include "libNM/CN.inl"

#endif // NM_CN_INCL_

