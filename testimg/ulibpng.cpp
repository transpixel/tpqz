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
\brief  This file contains unit test for img::libpng
*/


#include "libdat/info.h"
#include "libdat/validity.h"
#include "libio/stream.h"

#include "libdat/Extents.h"

//#include "extpng/png++-0.2.9/png.hpp"

#include <cassert>
#include <fstream>
#include <iostream>
#include <sstream>
#include <sstream>
#include <string>

#include <png.h>
//#include <zlib.h>
#include <stdio.h>


namespace
{
	constexpr size_t const sPngSigSize{ 8u };

	bool
	isPngFile
		( std::istream & istm
		)
	{
		bool isPng{ false };
		constexpr size_t sigSize{ 8u };
		png_byte signature[sigSize];

		istm.read((char*)signature, sigSize);
		bool const okay(istm.good());
		if (okay)
		{
			isPng = (0 == png_sig_cmp(signature, 0, sigSize));
		}
		return isPng;
	}

	struct PingContext
	{
		png_structp theState;
		png_infop theInfo;

		inline
		png_structp const &
		state
			()
		{
			return theState;
		}

		//! Allocate memory/structs for bookkeeping
		PingContext
			()
			: theState
				{ png_create_read_struct
					(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr)
				}
			, theInfo
				{ png_create_info_struct(theState) }
		{ }

		//! Free bookkeeping memory
		~PingContext
			()
		{
			png_destroy_read_struct
				(&theState, (png_infopp)nullptr, (png_infopp)nullptr);
		}

		//! True if this instance (data loading context) is okay
		bool
		isValid
			() const
		{
			return
				{  (bool)theState
				&& (bool)theInfo
				};
		}

		//! Image data high/wide
		dat::Extents
		hwSize
			() const
		{
			dat::Extents hw;
			if (isValid())
			{
				png_uint_32 const high
					{ png_get_image_height(theState, theInfo) };
				png_uint_32 const wide
					{ png_get_image_width(theState, theInfo) };
				hw = dat::Extents{ (size_t)high, (size_t)wide };
			}
			return hw;
		}

		std::string
		infoString
			( std::string const & title = {}
			) const
		{
			std::ostringstream oss;
			if (! title.empty())
			{
				oss << title << std::endl;
			}
			if (isValid())
			{
				oss << dat::infoString(hwSize(), "hwSize");
			}
			else
			{
				oss << " <null>";
			}
			return oss.str();
		}
	};

	struct PingInfo
	{
		dat::Extents theSize{};
		size_t theNumBits{ 0u };
		size_t theNumChans{ 0u };

		static
		PingInfo
		from
			( PingContext const & context
			)
		{
			PingInfo pInfo{};
			if (context.isValid())
			{
				png_structp const & state = context.theState;
				png_infop const & info = context.theInfo;
				png_uint_32 const high
					{ png_get_image_height(state, info) };
				png_uint_32 const wide
					{ png_get_image_width(state, info) };
				png_byte const deep
					{ png_get_bit_depth(state, info) };
				png_byte const color_type
					{ png_get_color_type(state, info) };

				pInfo.theSize = dat::Extents{ (size_t)high, (size_t)wide };
				pInfo.theNumBits = (size_t)deep;
				pInfo.theNumChans = 0u;
			}
			return pInfo;
		}
	};

	struct PingReader
	{
		PingContext theContext{};
		std::istream * theStream{ nullptr };

		static
		void
		readDataFunc
			( png_structp const & pngPtr // context.theState
			, png_bytep const & data // store data here, for
			, png_size_t const & length // this many elements
			)
		{
			png_voidp ioPtr{ png_get_io_ptr(pngPtr) };
			std::istream * const ptrStream
				{ reinterpret_cast<std::istream *>(ioPtr) };
			ptrStream->read((char*)data, length);
		}

		//! Allocate memory/structs for bookkeeping
		PingReader
			()
		{
			/*
			png_structp read_ptr = theContext.theState;
			png_voidp read_io_ptr = (png_voidp)theStream;
			png_rw_ptr read_data_fn = (png_rw_ptr)readDataFunc;
			*/

			// initialize reader and get header info
			png_set_read_fn
				( theContext.theState
				, (png_voidp)theStream
				, (png_rw_ptr)readDataFunc
				);

			// read header
			png_set_sig_bytes(theContext.theState, sPngSigSize);
			png_read_info(theContext.theState, theContext.theInfo);
		}
	};

	void
	loadPng
		( std::string const & fname
		)
	{
		{
			std::ifstream ifs(fname);
			bool const isPng{ isPngFile(ifs) };
			io::out() << dat::infoString(isPng, "isPng") << std::endl;
			io::out() << std::endl;
		}

		PingContext const context;

		PingReader const reader;

io::out() << dat::infoString(context, "context") << std::endl;

	}

	//! Read png file into buffer - from libpng doc examples
	void
	read_png_file
		( std::string const & fname
		)
	{
		// open file
		FILE * const fp{ fopen(fname.c_str(), "rb") };
		assert(fp);

		// check header/magic number
		constexpr size_t const numMagic{ 8u };
		char header[numMagic]; // magic check is up to length 8
		fread(header, 1, numMagic, fp);
		bool const isPng
			{ (0u == png_sig_cmp((png_byte*)header, 0, numMagic)) };
io::out() << dat::infoString(fname, "fname") << std::endl;
io::out() << dat::infoString(isPng, "isPng") << std::endl;

		// allocate space
		png_structp const png_ptr
			{ png_create_read_struct
				(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr)
			};
		assert(png_ptr);
		png_infop const info_ptr
			{ png_create_info_struct(png_ptr) };
		assert(info_ptr);
		if (setjmp(png_jmpbuf(png_ptr)))
		{
			// catch initialization errors
			io::err() << "ERROR in init" << std::endl;
		//	png_destroy_read_struct
		//		(&png_ptr, (png_infopp)nullptr, (png_infopp)nullptr);
			assert(false);
		}

		// initialize reader and get header info
		png_init_io(png_ptr, fp);
		png_set_sig_bytes(png_ptr, numMagic);
		png_read_info(png_ptr, info_ptr);

		// image characteristics
		png_uint_32 const high
			{ png_get_image_height(png_ptr, info_ptr) };
		png_uint_32 const wide
			{ png_get_image_width(png_ptr, info_ptr) };
		dat::Extents const hwSize{ (size_t)high, (size_t)wide };
		png_byte const deep
			{ png_get_bit_depth(png_ptr, info_ptr) };
		png_byte const color_type
			{ png_get_color_type(png_ptr, info_ptr) };

io::out() << dat::infoString(hwSize, "hwSize") << std::endl;
io::out() << "deep: " << +deep << std::endl;
io::out() << "color_type: " << +color_type << std::endl;
io::out() << "...Gray: " << PNG_COLOR_TYPE_GRAY << '\n';
io::out() << "...Gray_Alpha: " << PNG_COLOR_TYPE_GRAY_ALPHA << '\n';
io::out() << "...Palette: " << PNG_COLOR_TYPE_PALETTE << '\n';
io::out() << "...RGB: " << PNG_COLOR_TYPE_RGB << '\n';
io::out() << "...RGB_Alpha: " << PNG_COLOR_TYPE_RGB_ALPHA << '\n';

		// request libpng to handle interlace processing
		int const numPasses
			{ png_set_interlace_handling(png_ptr) };
		png_read_update_info(png_ptr, info_ptr);
io::out() << dat::infoString(numPasses, "numPasses") << std::endl;

		// catch errors during read
		if (setjmp(png_jmpbuf(png_ptr)))
		{
			io::err() << "ERROR during read" << std::endl;
		//	png_destroy_read_struct
		//		(&png_ptr, (png_infopp)nullptr, (png_infopp)nullptr);
			assert(false);
		}

		png_bytep * const row_pointers
			{ (png_bytep*)malloc(sizeof(png_bytep) * high) };
		for (size_t row{0u} ; row < hwSize.high() ; ++row)
		{
			png_bytep const row_pointer
				{ (png_byte*)malloc(png_get_rowbytes(png_ptr, info_ptr)) };
			row_pointers[row] = row_pointer;
		}
size_t const s_png_byte{ sizeof(png_byte) };
io::out() << dat::infoString(s_png_byte, "s_png_byte") << std::endl;

		// read image
		png_read_image(png_ptr, row_pointers);

		// close source file
		fclose(fp);
	}

//! Check basic API use
std::string
img_libpng_test0
	()
{
	std::ostringstream oss;

	return oss.str();
}

/*
//! Check TODO....
std::string
img_libpng_test1
	()
{
	std::ostringstream oss;
	oss << "Failure: implement this test!" << std::endl;
	return oss.str();
}
*/


}

//! Unit test for img::libpng
int
main
//	( int const /*argc*/
//	, char const * const * /*argv*/
//	)
( int const argc
, char const * const * argv
)
{
	std::ostringstream oss;

	assert(2u == argc);
	std::string const fname( argv[1] );
	io::out() << "arg:fname: " << fname << std::endl;

	// c++ style
	(void)loadPng(fname);
bool const useC{ false };
if (useC)
{
	read_png_file(fname);
}

	// run tests
	oss << img_libpng_test0();
//	oss << img_libpng_test1();

	// check/report results
	std::string const errMessages(oss.str());
	if (! errMessages.empty())
	{
		io::err() << errMessages << std::endl;
		return 1;
	}
	return 0;
}
