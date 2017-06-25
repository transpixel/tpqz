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

// TODO - hack for debugging image processing changes

struct PixelTracker
{
	static constexpr dat::RowCol const sNullRC{{ 1u, 0u }};
	static constexpr dat::RowCol const sZeroRC{{ 0u, 0u }};
	static constexpr dat::RowCol const sDarkRC{{ 2u, 0u }};
	static constexpr dat::RowCol const sNormRC{{ 3u, 0u }};
	static constexpr dat::RowCol const sLiteRC{{ 4u, 0u }};
	std::ostringstream theOss{};
	std::set<std::string> theTitles{};

	PixelTracker
		() = default;

	static
	void
	setGridMarkers
		( dat::grid<std::array<img::fpix_t, 3u> > * const & ptGrid
		)
	{
		dat::grid<std::array<img::fpix_t, 3u> > & grid = *ptGrid;
		dat::MinMax<img::fpix_t> const minmax
			(img::stats::pixelMinMax<img::fpix_t>(grid));

		img::fpix_t const pixNull{ dat::nullValue<img::fpix_t>() };
		img::fpix_t const pixZero{ 0.f };
		img::fpix_t const pixDark{ minmax.min() };
		img::fpix_t const pixNorm{ .5f*(minmax.min() + minmax.max()) };
		img::fpix_t const pixLite{ std::ceil(minmax.max()) };
		std::fill(grid(sNullRC).begin(), grid(sNullRC).end(), pixNull);
		std::fill(grid(sZeroRC).begin(), grid(sZeroRC).end(), pixZero);
		std::fill(grid(sDarkRC).begin(), grid(sDarkRC).end(), pixDark);
		std::fill(grid(sNormRC).begin(), grid(sNormRC).end(), pixNorm);
		std::fill(grid(sLiteRC).begin(), grid(sLiteRC).end(), pixLite);
	}

	template <typename PixType>
	void
	logMarkers
		( dat::grid<PixType> const & grid
		, std::string const & title = std::string()
		, bool const & logOnlyUnique = false
		)
	{
		bool const alreadyLogged{ (0u < theTitles.count(title)) };
		if ((! logOnlyUnique) || (! alreadyLogged))
		{
			theTitles.insert(title);
			dat::MinMax<typename PixType::value_type> const minmax
				(img::stats::pixelMinMax<typename PixType::value_type>(grid));
			theOss
				<< title << '\n'
				<< dat::infoString(minmax, "minmax") << '\n'
				<< " . " << dat::infoString(grid(sNullRC)) << '\n'
				<< " . " << dat::infoString(grid(sZeroRC)) << '\n'
				<< " . " << dat::infoString(grid(sDarkRC)) << '\n'
				<< " . " << dat::infoString(grid(sNormRC)) << '\n'
				<< " . " << dat::infoString(grid(sLiteRC)) << '\n'
				<< std::endl;
		}
	}
};

