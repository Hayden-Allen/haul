#pragma once

namespace haul
{
	template<typename T>
	struct point
	{
	public:
		T x, y;
	public:
		template<typename U = T>
		bool operator==(point<U> const& other) const
		{
			return x == other.x && y == other.y;
		}
		template<typename U = T>
		bool operator<(point<U> const& other) const
		{
			return x < other.x && y < other.y;
		}
	};
}