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
\brief Inline Definitions for sys::Utilization
*/


namespace sys
{

inline
// explicit
Utilization :: Utilization
	( size_t const & maxCount
	)
	: theMutex{}
	, theMaxCount{ maxCount }
	, theCount{ 0u }
	, theIsValid{ (0u < theMaxCount) }
{}

inline
void
Utilization :: increase
	()
{
	if (isValid())
	{
		{ std::lock_guard<std::mutex> lock(theMutex);
			assert(theCount < theMaxCount);
			++theCount;
		}
	}
}

inline
void
Utilization :: decrease
	()
{
	if (isValid())
	{
		bool hadSomething{ true };
		{ std::lock_guard<std::mutex> lock(theMutex);
			if (0u < theCount)
			{
				--theCount;
			}
			else
			{
				hadSomething = false;
			}
		}
		assert(hadSomething);
	}
}

inline
bool
Utilization :: isZero
	()
{
	bool atzero;
	{ std::lock_guard<std::mutex> lock(theMutex);
		atzero = (0u == theCount);
	}
	return atzero;
}

inline
bool
Utilization :: isIncomplete
	()
{
	bool hasRoom;
	{ std::lock_guard<std::mutex> lock(theMutex);
		hasRoom = (theCount < theMaxCount);
	}
	return hasRoom;
}

}

