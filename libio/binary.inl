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
\brief Definitions for namespace io::binary
*/


namespace io
{

namespace binary
{

template <typename Type>
inline
bool
save
	( std::vector<Type> const & items
	, std::string const & fpath
	)
{
	std::ofstream ofs(fpath, std::ios::binary);
	ofs.write
		( reinterpret_cast<char const * const>( items.data() )
		, static_cast<std::streamsize>( items.size() * sizeof(Type) )
		);
	return ofs.good();
}

template <typename Type>
inline
std::vector<Type>
load
	( std::string const & fpath
	)
{
	std::vector<Type> items;
	std::streamsize const bytesPerRec{ sizeof(Type) };
	std::streamsize numRecs{};
	std::ifstream ifs;
	bool const okay{ prepareStream(ifs, fpath, bytesPerRec, &numRecs) };
	if (okay)
	{
		// allocate space and read data
		items.resize(numRecs);
		std::streamsize const expBytes{ numRecs * bytesPerRec };
		ifs.read
			( reinterpret_cast<char * const>( items.data() )
			, expBytes
			);
		std::streamsize const gotBytes{ ifs.gcount() };
		if (! (gotBytes == expBytes))
		{
			items.clear();
		}
	}
	return items;
}

} // binary

} // io

