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
\brief Inline Definitions for mem::GuardedQueue
*/


#include <sstream>


template <typename DataType>
inline
mem::GuardedQueue<DataType> :: GuardedQueue
	()
	: theClassMutex()
	, theQ()
	, theQueueMutex()
	, theKeepGoing(true)
{
}

// copy constructor -- compiler provided
// assignment operator -- compiler provided
// destructor -- compiler provided


template <typename DataType>
inline
bool
mem::GuardedQueue<DataType> :: isActive
	()
{
	bool active(true);
	{
		std::lock_guard<std::mutex> lock(theClassMutex);
		active = theKeepGoing;
	}
	return active;
}

template <typename DataType>
inline
void
mem::GuardedQueue<DataType> :: deactivate
	()
{
	{
		std::lock_guard<std::mutex> lock(theClassMutex);
		theKeepGoing = false;
	}
}

template <typename DataType>
inline
bool
mem::GuardedQueue<DataType> :: hasData
	()
{
	bool isEmpty(true);
	{
		std::lock_guard<std::mutex> lock(theClassMutex);
		isEmpty = theQ.empty();
	}
	return (! isEmpty);
}

template <typename DataType>
inline
void
mem::GuardedQueue<DataType> :: addItem
	( DataType const & item
	)
{
	{
		std::lock_guard<std::mutex> lock(theQueueMutex);
		theQ.push_back(item);
	}
}

template <typename DataType>
inline
DataType
mem::GuardedQueue<DataType> :: nextDataItem
	()
{
	DataType item;
	{
		std::lock_guard<std::mutex> lock(theQueueMutex);
		if (! theQ.empty())
		{
			item = theQ.front();
			theQ.pop_front();
		}
	}
	return item;
}

template <typename DataType>
inline
std::string
mem::GuardedQueue<DataType> :: infoString
	( std::string const & title
	) const
{
	std::ostringstream oss;
	if (! title.empty())
	{
		oss << title << std::endl;
	}

	oss << "theQueue.size: " << theQ.size();

	return oss.str();
}

