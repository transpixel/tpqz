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

#ifndef tri_NodeCache_INCL_
#define tri_NodeCache_INCL_

/*! \file
\brief Declarations for tri::NodeCache
*/


#include "libtri/IsoTille.h"
#include "libtri/tri.h"

#include <string>
#include <sstream>


namespace tri
{
	constexpr float sValue{}; // TODO

/*! \brief Data store compatible with tri::IsoTille use.

\par Example
\dontinclude testtri/uNodeCache.cpp
\skip ExampleStart
\until ExampleEnd
*/

class NodeCache
{

public: // methods

	//! default null constructor
	NodeCache
		() = default;

	//! Allocate enough space for all notes in active domain.
	explicit
	NodeCache
		( tri::IsoTille const & // trinet
		)
	{
	}

	// copy constructor -- compiler provided
	// assignment operator -- compiler provided
	// destructor -- compiler provided

	//! True if instance is valid
	bool
	isValid
		() const
	{
		return false;
	}

	size_t
	size
		() const
	{
		return {}; // TODO
	}

using ValueType = float;

	void
	setValue
		( NodeKey const & // keyIJ
		, ValueType const & // value
		)
	{
	}

	ValueType const &
	valueAt
		( NodeKey const & // keyIJ
		)
	{
		return sValue;
	}

	//! Descriptive information about this instance.
	std::string
	infoString
		( std::string const & title = std::string()
//		, std::string const & fmt = std::string("%12.6f")
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

}; // NodeCache

} // tri

// Inline definitions
// #include "libtri/NodeCache.inl"

#endif // tri_NodeCache_INCL_

