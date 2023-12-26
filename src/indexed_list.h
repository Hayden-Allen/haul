#pragma once
#include <stdint.h>
#include <vector>

namespace haul
{
	template<typename T, typename IDX = uint64_t>
	class indexed_list
	{
	public:
		indexed_list(IDX const max_size) :
			m_capacity(max_size),
			m_next(0),
			m_list(max_size)
		{
			m_openings.reserve(max_size);
		}
		indexed_list(indexed_list const& other) = delete;
		indexed_list(indexed_list&& other) = delete;
		virtual ~indexed_list() {}
	public:
		IDX insert(T const* const t)
		{
			IDX index = get_next();
			m_list[index] = t;
			return index;
		}
		T* remove(IDX const index)
		{
			T const* const ret = m_list[index];
			m_list[index] = nullptr;
			m_openings.push_back(index);
			return ret;
		}
		T* get(IDX const index)
		{
			return m_list[index];
		}
		IDX size() const
		{
			return m_next - (IDX)m_openings.size();
		}
		bool empty() const
		{
			return (IDX)m_openings.size() == m_next;
		}
		bool full() const
		{
			return m_next == m_capacity && m_openings.empty();
		}
	protected:
		IDX m_capacity, m_next;
		std::vector<T const*> m_list;
		std::vector<IDX> m_openings;
	protected:
		IDX get_next()
		{
			IDX ret = m_next;
			if (m_openings.size())
			{
				IDX ret = m_openings.back();
				m_openings.pop_back();
			}
			else
			{
				m_next++;
			}
			return ret;
		}
	};
} // namespace haul
