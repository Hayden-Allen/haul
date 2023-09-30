#pragma once

namespace haul
{
	template<typename T>
	struct rect
	{
	public:
		point<T> min, max;
	public:
		template<typename U = T>
		bool operator==(rect<U> const& other) const
		{
			return min == other.min && max == other.max;
		}
		template<typename U = T>
		bool operator!=(rect<U> const& other) const
		{
			return !operator==(other);
		}
		template<typename U = T>
		bool operator<(rect<U> const& other) const
		{
			return min < other.min && max < other.max;
		}
		template<typename U = T>
		bool intersects(rect<U> const& other, range_overlaps const& params = {}) const
		{
			range<T> tx = { min.x, max.x }, ox = { other.min.x, other.max.x };
			range<T> ty = { min.y, max.y }, oy = { other.min.y, other.max.y };
			return tx.overlaps(ox, params) && ty.overlaps(oy, params);
		}
		template<typename U = T>
		bool contains(point<U> const& p, range_contains const& params = {}) const
		{
			range<T> const tx = { min.x, max.x }, ty = { min.y, max.y };
			return tx.contains(p.x, params) && ty.contains(p.y, params);
		}
		template<typename U = T>
		bool contains(rect<U> const& other, range_overlaps const& params = {}) const
		{
			range<T> const tx = { min.x, max.x }, ox = { other.min.x, other.max.x };
			range<T> const ty = { min.y, max.y }, oy = { other.min.y, other.max.y };
			return tx.contains(ox, params) && ty.contains(oy, params);
		}
		template<typename U = T>
		U width() const
		{
			return HAUL_CAST(U, max.x - min.x);
		}
		template<typename U = T>
		U height() const
		{
			return HAUL_CAST(U, max.y - min.y);
		}
		template<typename U = T>
		U area() const
		{
			return HAUL_CAST(U, width() * height());
		}
		template<typename U = T>
		U perimeter() const
		{
			return HAUL_CAST(U, 2 * width() + 2 * height());
		}
	};
}