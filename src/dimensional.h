#pragma once

namespace haul
{
	template<typename T>
	class dimensional2 : public typed<T>
	{
	public:
		template<typename U = T>
		U width() const
		{
			return HAUL_CAST(U, m_w);
		}
		template<typename U = T>
		U height() const
		{
			return HAUL_CAST(U, m_h);
		}
	protected:
		T m_w, m_h;
	protected:
		template<typename W = T, typename H = T>
		dimensional2(W const w, H const h) :
			m_w(HAUL_CAST(T, w)),
			m_h(HAUL_CAST(T, h))
		{}
	};

	template<typename T>
	class dimensional3 : public dimensional2<T>
	{
	public:
		template<typename U = T>
		U depth() const
		{
			return HAUL_CAST(U, m_d);
		}
	protected:
		T m_d;
	protected:
		template<typename W = T, typename H = T, typename D = T>
		dimensional3(W const w, H const h, D const d) :
			dimensional2<T>(w, h),
			m_d(HAUL_CAST(T, d))
		{}
	};
}