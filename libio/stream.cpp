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
\brief Definitions for io::stream
*/

// declaration include
#include "libio/stream.h"

#include "libsys/time.h"

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <mutex>
#include <string>
#include <vector>

namespace
{
	std::mutex localSinkMutex;

	std::vector<std::ostream *> localErrSinks;
	std::vector<std::ostream *> localOutSinks;
	std::vector<std::pair<std::ostream *, bool> > localLogSinks;
}

//
// addSink
//
void
io::err :: addSink
	( std::ostream * const str
	)
{
	std::lock_guard<std::mutex> lock(localSinkMutex);

	// always make sure std::cerr is part of err
	if (localErrSinks.empty())
	{
		localErrSinks.push_back(&std::cerr);
	}

	// add the requested sink
	localErrSinks.push_back(str);
}

//
// removeSink
//
bool
io::err :: removeSink
	( std::ostream const * const str
	)
{
	bool found(false);
	std::lock_guard<std::mutex> lock(localSinkMutex);

	std::vector<std::ostream*>::iterator const itf
		(std::find(localErrSinks.begin(), localErrSinks.end(), str));

	if (itf != localErrSinks.end())
	{
		localErrSinks.erase(itf);
		found = true;
	}

	return found;
}

//
// constructor
//
io::err :: err
	()
	: theOss()
{
}

//
// destructor
//
io::err :: ~err
	()
{
	std::lock_guard<std::mutex> lock(localSinkMutex);

	if (localErrSinks.empty())
	{
		localErrSinks.push_back(&std::cerr);
	}

	for (std::vector<std::ostream*>::iterator
		its(localErrSinks.begin()) ; its != localErrSinks.end() ; ++its)
	{
		std::ostream * const ostr(*its);

		theOss.seekg(0);
		std::istreambuf_iterator<char> in(theOss);
		std::istreambuf_iterator<char> const eos;
		std::ostreambuf_iterator<char> out(*ostr);
		std::copy(in, eos, out);

		ostr->flush();
	}
}

//
// addSink
//
void
io::out :: addSink
	( std::ostream * const str
	)
{
	std::lock_guard<std::mutex> lock(localSinkMutex);
	if (localOutSinks.empty())
	{
		localOutSinks.push_back(&std::cout);
	}
	localOutSinks.push_back(str);
}

//
// removeSink
//
bool
io::out :: removeSink
	( std::ostream const * const str
	)
{
	bool found(false);
	std::lock_guard<std::mutex> lock(localSinkMutex);

	std::vector<std::ostream*>::iterator const itf
		(std::find(localOutSinks.begin(), localOutSinks.end(), str));
	if (itf != localOutSinks.end())
	{
		localOutSinks.erase(itf);
		found = true;
	}

	return found;
}

//
// constructor
//
io::out :: out
	()
	: theOss()
{
}

//
// destructor
//
io::out :: ~out
	()
{
	std::lock_guard<std::mutex> lock(localSinkMutex);

	if (localOutSinks.empty())
	{
		localOutSinks.push_back(&std::cout);
	}

	for (std::vector<std::ostream*>::iterator
		its(localOutSinks.begin()) ; its != localOutSinks.end() ; ++its)
	{
		std::ostream * const ostr(*its);

		theOss.seekg(0);
		std::istreambuf_iterator<char> in(theOss);
		std::istreambuf_iterator<char> const eos;
		std::ostreambuf_iterator<char> out(*ostr);
		std::copy(in, eos, out);

		ostr->flush();
	}
}

//
// addSink
//
void
io::log :: addSink
	( std::ostream * const str
	, bool const & useFlush
	)
{
	std::lock_guard<std::mutex> lock(localSinkMutex);
	localLogSinks.push_back(std::make_pair(str, useFlush));
}

//
// removeSink
//
bool
io::log :: removeSink
	( std::ostream const * const str
	)
{
	bool found(false);
	std::lock_guard<std::mutex> lock(localSinkMutex);

	// find stream
	std::vector<std::pair<std::ostream *, bool> >::iterator itf
		(localLogSinks.begin());
	for ( ; localLogSinks.end() != itf ; ++itf)
	{
		std::ostream const * const currStrm = itf->first;
		if (str == currStrm)
		{
			break;
		}
	}

	if (itf != localLogSinks.end())
	{
		localLogSinks.erase(itf);
		found = true;
	}

	return found;
}

//
// constructor
//
io::log :: log
	()
	: theOss()
{
	double const now(sys::time::relativeNow());
	theOss << std::setprecision(6u) << std::setw(14u) << now << " ";
}

//
// destructor
//
io::log :: ~log
	()
{
	std::lock_guard<std::mutex> lock(localSinkMutex);

	for (std::vector<std::pair<std::ostream*, bool>>::iterator
		its(localLogSinks.begin()) ; its != localLogSinks.end() ; ++its)
	{
		std::ostream * const ostr = its->first;
		bool const & useFlush = its->second;

		theOss.seekg(0);
		std::istreambuf_iterator<char> in(theOss);
		std::istreambuf_iterator<char> const eos;
		std::ostreambuf_iterator<char> out(*ostr);
		std::copy(in, eos, out);

		if (useFlush)
		{
			ostr->flush();
		}
	}
}
