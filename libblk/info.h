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

#ifndef blk_info_INCL_
#define blk_info_INCL_

/*! \file
\brief Declarations for blk::info
*/


#include "libblk/blk.h"
#include "libga/Rigid.h"

#include <string>
#include <map>
#include <vector>


namespace blk
{
	//! Condensed orientation infromation for collection of orientations
	template <typename TypeKey>
	std::string
	infoString
		( std::map<TypeKey, ga::Rigid> oriMap
		, std::string const & title = std::string{}
		);

	//! Condensed orientation infromation for collection of orientations
	std::string
	infoString
		( std::vector<ga::Rigid> const & oris
		, std::string const & title = std::string{}
		);

	//! Condesed summary of all RelOris
	std::string
	infoString
		( std::vector<EdgeOri> const & eROs
		, std::string const & title = std::string{}
		);

} // blk

// Inline definitions
#include "libblk/info.inl"

#endif // blk_info_INCL_

