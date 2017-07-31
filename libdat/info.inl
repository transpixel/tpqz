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
\brief Definitions for dat::info
*/


#include <iomanip>
#include <sstream>

#include <typeinfo>
#if defined(__GNUG__)
	#include <cxxabi.h>
#endif


namespace dat
{

namespace priv
{
	//! Configure stream for floating point output
	inline
	void
	configStream
		( std::ostream & strm
		, size_t const & prec
		)
	{
		strm << std::fixed << std::setprecision(prec);
	}

}
		

//======================================================================

template <typename Type>
std::string
nameOfType
	( const Type & arg
	)
{
	std::string name{ "????" };

#	if defined(__GNUG__)
		int status{};
		char * realname
			{ abi::__cxa_demangle(typeid(arg).name(), 0, 0, &status) };
		if (realname)
		{
			name = realname;
		}
		free(realname);
#	endif

	return name;
}
inline
void
putTitle
	( std::ostream & strm
	, std::string const & title
	, size_t const & fieldWide
	)
{
	if (! title.empty())
	{
		strm << std::setw(fieldWide) << title << " ";
	}
}

template <typename FType, EnableIf< std::is_floating_point<FType> >...>
inline
std::string
infoString
	( FType const & value
	, std::string const & title
	, size_t const & fieldsWide
	, size_t const & fieldsPrecision
	)
{
	std::ostringstream oss;
	priv::configStream(oss, fieldsPrecision);
	putTitle(oss, title);

	oss << std::setw(fieldsWide) << (long double)value;

	return oss.str();
}

template <typename IType, EnableIf< std::is_integral<IType> >...>
inline
std::string
infoString
	( IType const & arg
	, std::string const & title
	, size_t const & fieldsWide
	)
{
	std::ostringstream oss;
	putTitle(oss, title);

	oss << std::setw(fieldsWide) << (long long)arg;

	return oss.str();
}

inline
std::string
infoString
	( bool const & value
	, std::string const & title
	, size_t const & fieldsWide
	)
{
	std::ostringstream oss;
	putTitle(oss, title);

	oss << std::boolalpha << std::setw(fieldsWide) << value;

	return oss.str();
}

inline
std::string
infoString
	( void * const & ptValue
	, std::string const & title
	, size_t const & fieldsWide
	)
{
	std::ostringstream oss;
	putTitle(oss, title);

	oss << std::setw(fieldsWide) << (void*)ptValue;

	return oss.str();
}


inline
std::string
infoString
	( std::string const & value
	, std::string const & title
	, size_t const & fieldsWide
	)
{
	std::ostringstream oss;
	putTitle(oss, title);

	oss << std::setw(fieldsWide) << value;

	return oss.str();
}

template <typename FType, EnableIf< std::is_floating_point<FType> >...>
inline
std::string
infoString
	( std::complex<FType> const & value
	, std::string const & title
	, size_t const & fieldsWide
	, size_t const & fieldsPrecision
	)
{
	std::ostringstream oss;
	putTitle(oss, title);

	oss
		<< " " << dat::infoString(value.real(), "", fieldsWide, fieldsPrecision)
		<< " " << dat::infoString(value.imag(), "", fieldsWide, fieldsPrecision)
		;

	return oss.str();
}

template <typename FwdIter>
inline
std::string
infoString
	( FwdIter const & beg
	, FwdIter const & end
	, std::string const & title
	, size_t const & nafter
	, size_t const & nbefore
	, size_t const & fieldsPerLine
	)
{
	std::ostringstream oss;
	size_t const twide(std::max((size_t)15u, title.size()));
	putTitle(oss, title, twide);

	size_t const fwide(1u + nbefore + 1u + nafter);
	size_t count(0u);
	for (FwdIter iter(beg) ; end != iter ; ++iter, ++count)
	{
		if ((0u != count) && (0u == (count % fieldsPerLine)))
		{
			oss << '\n';
			// match title size
			putTitle(oss, " ", twide);
		}
		oss
			<< std::fixed << std::setprecision(nafter)
			<< " " << std::setw(fwide) << infoString(*iter);
	}
	return oss.str();
}

template <typename Type, size_t Dim>
inline
std::string
infoString
	( std::array<Type, Dim> const & arg
	, std::string const & title
	, size_t const & nafter
	, size_t const & nbefore
	, size_t const & fieldsPerLine
	)
{
	return infoString
		(arg.begin(), arg.end(), title, nafter, nbefore, fieldsPerLine);
}

template
	< typename CType
	, EnableIf
		< dat::has::infoString<CType, std::string(std::string const &)> >...
	>
std::string
infoString
	( CType const & arg
	, std::string const & title
	)
{
	return arg.infoString(title);
}

template <typename Type1, typename Type2>
inline
std::string
infoString
	( std::pair<Type1, Type2> const & value
	, std::string const & title
	, size_t const & fieldsWide
	)
{
	std::ostringstream oss;
	putTitle(oss, title);

	oss
		<< infoString(value.first, "", fieldsWide)
		<< std::string(" ")
		<< infoString(value.second, "", fieldsWide)
		;

	return oss.str();
}

//======================================================================
}

