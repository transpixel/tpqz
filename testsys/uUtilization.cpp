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
\brief  This file contains unit test for sys::Utilization
*/

#include "libsys/Utilization.h"

#include "libio/stream.h"
#include "libdat/info.h"

#include <iostream>
#include <limits>
#include <sstream>
#include <string>
#include <thread>


namespace
{

//! Check for common functions
std::string
sys_Utilization_test0
	()
{
	std::ostringstream oss;
	/*
	sys::Utilization const aNull;
	if (aNull.isValid())
	{
		oss << "Failure of null value test" << std::endl;
		oss << "infoString: " << aNull.infoString("aNull") << std::endl;
	}
	*/
	return oss.str();
}

//! Check nonsense cases
std::string
sys_Utilization_test0a
	()
{
	std::ostringstream oss;

	sys::Utilization used(0u);
	if (! used.isZero()) // what should this be? undefined?
	{
		oss << "Failure of zero:isZero @ 0 test" << std::endl;
	}
	if (  used.isIncomplete()) // should always be 'full'
	{
		oss << "Failure of zero:isIncomplete @ 0 test" << std::endl;
	}
	used.increase();
	if (! used.isZero()) // what should this be? undefined?
	{
		oss << "Failure of zero:isZero @ 0(2nd) test" << std::endl;
	}
	if (  used.isIncomplete()) // should always be 'full'
	{
		oss << "Failure of zero:isIncomplete @ 0(2nd) test" << std::endl;
	}
	used.decrease();
	if (! used.isZero()) // what should this be? undefined?
	{
		oss << "Failure of zero:isZero @ 0(3rd) test" << std::endl;
	}
	if (  used.isIncomplete()) // should always be 'full'
	{
		oss << "Failure of zero:isIncomplete @ 0(3rd) test" << std::endl;
	}

	return oss.str();
}

//! Check trivial cases
std::string
sys_Utilization_test0b
	()
{
	std::ostringstream oss;

	sys::Utilization used(1u);
	if (! used.isZero())
	{
		oss << "Failure of one:isZero @ 0 test" << std::endl;
	}
	if (! used.isIncomplete()) // should always be 'full'
	{
		oss << "Failure of one:isIncomplete @ 0 test" << std::endl;
	}
	used.increase(); // now at full capacity
	if (used.isZero())
	{
		oss << "Failure of one:isZero @ 1 test" << std::endl;
	}
	if (used.isIncomplete())
	{
		oss << "Failure of one:isIncomplete @ 1 test" << std::endl;
	}
	used.decrease(); // back to empty state
	if (! used.isZero())
	{
		oss << "Failure of one:isZero @ 0(2nd) test" << std::endl;
	}
	if (! used.isIncomplete())
	{
		oss << "Failure of one:isIncomplete @ 0(2nd) test" << std::endl;
	}

	return oss.str();
}

//! Check basic use
std::string
sys_Utilization_test1
	()
{
	std::ostringstream oss;

	// check a single instance
	sys::Utilization used(2u);
	if (! used.isZero())
	{
		oss << "Failure of two:isZero @ 0 test" << std::endl;
	}
	if (! used.isIncomplete())
	{
		oss << "Failure of two:isIncomplete @ 0 test" << std::endl;
	}
	used.increase();
	if (used.isZero())
	{
		oss << "Failure of two:isZero @ 1 test" << std::endl;
	}
	if (! used.isIncomplete())
	{
		oss << "Failure of two:isIncomplete @ 1 test" << std::endl;
	}
	used.increase();
	if (used.isIncomplete())
	{
		oss << "Failure of two:isIncomplete @ full test" << std::endl;
	}

	return oss.str();
}

	struct Increaser
	{
		sys::Utilization * const theCounter;
		size_t const theMax;

		void
		operator()
			() const
		{
			assert(theCounter);
			for (size_t nn(0u) ; nn < theMax ; ++nn)
			{
				theCounter->increase();
			}
		}
	};

	struct Decreaser
	{
		sys::Utilization * const theCounter;
		size_t const theMax;

		void
		operator()
			() const
		{
			assert(theCounter);
			for (size_t nn(0u) ; nn < theMax ; ++nn)
			{
				theCounter->decrease();
			}
		}
	};

//! Check concurrent thread use
std::string
sys_Utilization_test2
	()
{
	std::ostringstream oss;

	sys::Utilization theCounter(std::numeric_limits<size_t>::max());
	constexpr size_t big{  128u*1024u };
	Increaser const inc{ & theCounter, big };
	Decreaser const dec{ & theCounter, big };

	// preincrement enough so that decrement can never hit zero
	std::thread preInc(Increaser{ & theCounter, 5u*big });
	preInc.join();

	// mix a bunch of increasing and decreasing threads
	std::thread i1(inc); std::thread d1(dec);
	std::thread i2(inc); std::thread d2(dec);
	std::thread i3(inc); std::thread d3(dec);
	std::thread i4(inc); std::thread d4(dec);
	std::thread i5(inc); std::thread d5(dec);

	// wait for all threads to complete
	i1.join(); d1.join();
	i2.join(); d2.join();
	d3.join(); i3.join();
	d4.join(); i4.join();
	d5.join(); i5.join();

	// post-decrement to remove initial offset
	std::thread postDec(Decreaser{ & theCounter, 5u*big });
	postDec.join();

	// check that result is back to zero
	if (! theCounter.isZero())
	{
		oss << "Failure of isZero multiThread test" << std::endl;
	}

	return oss.str();
}


}

//! Unit test for sys::Utilization
int
main
	( int const /*argc*/
	, char const * const * /*argv*/
	)
{
	std::ostringstream oss;

	// run tests
	oss << sys_Utilization_test0();
	oss << sys_Utilization_test0a();
	oss << sys_Utilization_test0b();
	oss << sys_Utilization_test1();
	oss << sys_Utilization_test2();

	// check/report results
	std::string const errMessages(oss.str());
	if (! errMessages.empty())
	{
		io::err() << errMessages << std::endl;
		return 1;
	}
	return 0;
}
