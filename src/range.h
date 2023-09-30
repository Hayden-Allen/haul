#pragma once

namespace haul
{
	struct range_contains
	{
		bool left = false, right = false;
	};
	struct range_overlaps
	{
		range_contains min = {}, max = {};
	};



	template<typename T>
	class range : public typed<T>
	{
	public:
		range(T const& min, T const& max) :
			m_min(min),
			m_max(max)
		{
			HAUL_ASSERT(min < max);
		}
	public:
		template<typename U = T>
		bool operator==(range<U> const& other) const
		{
			return m_min == HAUL_CAST(T, other.m_min) && m_max == HAUL_CAST(T, other.m_max);
		}
		template<typename U = T>
		bool contains(U const& value, range_contains const& params = {}) const
		{
			return (params.left ? m_min <= value : m_min < value) && (params.right ? m_max >= value : m_max > value);
		}
		template<typename U = T>
		bool contains(range<U> const& other, range_overlaps const& params = {}) const
		{
			return contains(other.m_min, params.min) && contains(other.m_max, params.max);
		}
		template<typename U = T>
		bool overlaps(range<U> const& other, range_overlaps const& params = {}) const
		{
			return contains(other.m_min, params.min) || contains(other.m_max, params.max);
		}
		template<typename U = T, typename R = T>
		U map_to(U const& value, range<R> const& target) const
		{
			float slope = 1.f * target.span() / span();
			return target.m_min + slope * (value - m_min);
		}
		template<typename U = T, typename R = T>
		U map_from(U const& value, range<R> const& source) const
		{
			return source.map_to(value, *this);
		}
		template<typename U = T>
		U min() const
		{
			return HAUL_CAST(U, m_min);
		}
		template<typename U = T>
		U max() const
		{
			return HAUL_CAST(U, m_max);
		}
		template<typename U = T>
		U span() const
		{
			return HAUL_CAST(U, m_max - m_min);
		}
	private:
		T m_min, m_max;
	};
}