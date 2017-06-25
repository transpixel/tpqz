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

#ifndef dat_KeyedValues_INCL_
#define dat_KeyedValues_INCL_

/*! \file
\brief Declarations for dat::KeyedValues
*/


#include <map>
#include <string>


namespace dat
{

// Shorthand types

	typedef std::string Key;
	typedef std::string Value;

/*! \brief Container/management of Key/Value pairs.
*/

/*
\par Example
\dontinclude testdat/uKeyedValues.cpp
\skip ExampleStart
\until ExampleEnd
*/

class KeyedValues
{

	std::map<Key, Value> theKVs;

public: // static methods

	//! Populate from text file (space between Key and Value)
	static
	KeyedValues
	fromFile
		( std::string const & fpath
		);

public: // methods

	//! construct empty instance
	KeyedValues
		();

	// copy constructor -- compiler provided
	// assignment operator -- compiler provided
	// destructor -- compiler provided

	//! Check if instance is valid (empty considered null)
	bool
	isValid
		() const;

	//! Value for key
	std::string
	value
		( Key const & key
		) const;

	//! Value for key
	double
	dValue
		( Key const & key
		) const;

	//! Descriptive information about this instance.
	std::string
	infoString
		( std::string const & title = std::string()
		) const;

	//! Contents of this instance
	std::string
	infoStringContents
		( std::string const & title = std::string()
		) const;
};

}

// Inline definitions
// #include "libdat/KeyedValues.inl"

#endif // dat_KeyedValues_INCL_

