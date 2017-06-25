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
\brief Inline definitions for la::EigenWrapper
*/


namespace la
{


template <int DimRow, int DimCol>
inline
// explicit
EigenWrapper<DimRow, DimCol> :: EigenWrapper
	( Eigen::Matrix<double, DimRow, DimCol> const & emat
	)
	: theMat(emat)
{
}

template <int DimRow, int DimCol>
inline
// explicit
EigenWrapper<DimRow, DimCol> :: EigenWrapper
	( std::initializer_list<double> const & values
	)
	: theMat{}
{
	int row{ 0 };
	for (double const & value : values)
	{
		theMat(row++, 0) = value;
	}
}

template <int DimRow, int DimCol>
inline
// explicit
EigenWrapper<DimRow, DimCol> :: EigenWrapper
	( std::initializer_list<std::initializer_list<double> >
		const & rcValues
	)
	: theMat{}
{
	int row{ 0 };
	for (std::initializer_list<double> const values : rcValues)
	{
		int col{ 0 };
		for (double const & value : values)
		{
			assert(row < DimRow);
			assert(col < DimCol);
			theMat(row, col++) = value;
		}
		++row;
	}
}

template <int DimRow, int DimCol>
inline
size_t
EigenWrapper<DimRow, DimCol> :: high
	() const
{
	assert(0u <= theMat.rows());
	return static_cast<size_t>(theMat.rows());
}

template <int DimRow, int DimCol>
inline
size_t
EigenWrapper<DimRow, DimCol> :: wide
	() const
{
	assert(0u <= theMat.cols());
	return static_cast<size_t>(theMat.cols());
}

template <int DimRow, int DimCol>
inline
dat::Extents
EigenWrapper<DimRow, DimCol> :: hwSize
	() const
{
	return dat::Extents(high(), wide());
}

template <int DimRow, int DimCol>
inline
double const &
EigenWrapper<DimRow, DimCol> :: operator()
	( int const & row
	, int const & col
	) const
{
	return theMat(row, col);
}

template <int DimRow, int DimCol>
inline
double &
EigenWrapper<DimRow, DimCol> :: operator()
	( int const & row
	, int const & col
	)
{
	return theMat(row, col);
}

template <int DimRow, int DimCol>
inline
double *
EigenWrapper<DimRow, DimCol> :: begin
	()
{
	return theMat.data();
}

template <int DimRow, int DimCol>
inline
double *
EigenWrapper<DimRow, DimCol> :: end
	()
{
	return theMat.data() + theMat.size();
}

template <int DimRow, int DimCol>
inline
double const *
EigenWrapper<DimRow, DimCol> :: begin
	() const
{
	return theMat.data();
}

template <int DimRow, int DimCol>
inline
double const *
EigenWrapper<DimRow, DimCol> :: end
	() const
{
	return theMat.data() + theMat.size();
}

template <int DimRow, int DimCol>
inline
bool
EigenWrapper<DimRow, DimCol> :: isValid
	() const
{
	return (0u < theMat.size());
}

template <int DimRow, int DimCol>
inline
bool
EigenWrapper<DimRow, DimCol> :: nearlyEquals
	( EigenWrapper const & other
	, double const & tol
	) const
{
	return dat::nearlyEquals
		( begin(), end()
		, other.begin(), other.end()
		, tol
		);
}

template <int DimRow, int DimCol>
inline
std::string
EigenWrapper<DimRow, DimCol> :: infoString
	( std::string const & title
	) const
{
	std::ostringstream oss;
	if (! title.empty())
	{
		oss << title << " ";
	}
	if (isValid())
	{
		oss << "High,Wide:"
			<< " " << dat::infoString(high())
			<< " " << dat::infoString(wide())
			;
	}
	else
	{
		oss << " <null>";
	}
	return oss.str();
}

template <int DimRow, int DimCol>
inline
std::string
EigenWrapper<DimRow, DimCol> :: infoStringContents
	( std::string const & title
	, std::string const & fmt
	) const
{
	std::ostringstream oss;
	oss << infoString(title);
	if (isValid())
	{
		int const numRows{ (int)high() };
		int const numCols{ (int)wide() };
		for (int row{0} ; row < numRows ; ++row)
		{
			oss << '\n';
			for (int col(0) ; col < numCols ; ++col)
			{
				oss << " " << io::sprintf(fmt, theMat(row,col));
				//	dat::infoString(theMat(row,col));
			}
		}
	}
	return oss.str();
}

}

