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

#ifndef io_stream_INCL_
#define io_stream_INCL_

/*! \file
\brief Declarations for io::stream
*/


#include <sstream>


/*! \file
\brief Declarations for io::out, io::err
*/

//! The io/stream module provides lock wrappers for std::ostreams.
namespace io
{

//! lock wrapper for std::cerr
struct err
{
	//! add a sink
	static
	void
	addSink
		( std::ostream * const str
		);

	//! remove a sink
	static
	bool
	removeSink
		( std::ostream const * const str
		);

	//! default constructor
	err
		();

	//! destructor...write & flush to std::err
	~err
		();

	//! how stream gets filled
	template < typename Type >
	inline
	err &
	operator<<
		( Type const & val
		);

	//! specializer for manipulators
	inline
	err &
	operator<<
		( std::ostream & (*fptr)(std::ostream &)
		);

private:

	std::stringstream theOss;
};


//! lock wrapper for std::cout
struct out
{
	//! add a sink
	static
	void
	addSink
		( std::ostream * const str
		);

	//! remove a sink
	static
	bool
	removeSink
		( std::ostream const * const str
		);

	//! default constructor
	out
		();

	//! destructor...write & flush to std::out
	~out
		();

	//! how stream gets filled
	template < typename Type >
	inline
	out &
	operator<<
		( Type const & val
		);

	//! specializer for manipulators
	inline
	out &
	operator<<
		( std::ostream & (*fptr)(std::ostream &)
		);

private:

	std::stringstream theOss;
};

//! lock wrapper for general logging (no default sink)
struct log
{
	//! add a sink
	static
	void
	addSink
		( std::ostream * const str
		, bool const & useFlush = true
		);

	//! remove a sink
	static
	bool
	removeSink
		( std::ostream const * const str
		);

	//! default constructor
	log();

	//! destructor...write & flush to sinks
	~log();

	//! how stream gets filled
	template < typename Type >
	inline
	log &
	operator<<
		( Type const & val
		);

	//! specializer for manipulators
	inline
	log &
	operator<<
		( std::ostream & (*fptr)(std::ostream &)
		);

private:

	std::stringstream theOss;
};

}


// Inline definitions
#include "libio/stream.inl"

#endif // io_stream_INCL_

