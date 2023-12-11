#pragma once
#include <map>
#include <unordered_map>
#include <unordered_set>

namespace haul
{
	// bidirectional map base
	// inherited by:
	//		bdmvmap_base
	//		bduvmap_base
	template<typename K, typename V, typename K2V, typename V2K>
	class bdmap_base
	{
	public:
		V& operator[](K const& key)
		{
			return get_val(key);
		}
		K& operator()(V const& val)
		{
			return get_key(val);
		}
		V const& operator[](K const& key) const
		{
			return get_val(key);
		}
		K const& operator()(V const& val) const
		{
			return get_key(val);
		}
	public:
		virtual void insert(K const& key, V const& val) = 0;
		virtual void erase_key(K const& key) = 0;
		virtual void erase_val(V const& val) = 0;
		virtual typename K2V::mapped_type replace_key(K const& key, V const& val) = 0;
		virtual typename V2K::mapped_type replace_val(V const& val, K const& key) = 0;
		bool contains_key(K const& key) const
		{
			return m_k2v.contains(key);
		}
		bool contains_val(V const& val) const
		{
			return m_v2k.contains(val);
		}
		typename K2V::mapped_type& get_val(K const& key)
		{
			HAUL_ASSERT(m_k2v.contains(key));
			return m_k2v.at(key);
		}
		typename V2K::mapped_type& get_key(V const& val)
		{
			HAUL_ASSERT(m_v2k.contains(val));
			return m_v2k.at(val);
		}
		typename K2V::mapped_type const& get_val(K const& key) const
		{
			HAUL_ASSERT(m_k2v.contains(key));
			return m_k2v.at(key);
		}
		typename V2K::mapped_type const& get_key(V const& val) const
		{
			HAUL_ASSERT(m_v2k.contains(val));
			return m_v2k.at(val);
		}
		size_t size() const
		{
			return m_k2v.size();
		}
		auto begin()
		{
			return m_k2v.begin();
		}
		auto end()
		{
			return m_k2v.end();
		}
		auto begin() const
		{
			return m_k2v.begin();
		}
		auto end() const
		{
			return m_k2v.end();
		}
	protected:
		K2V m_k2v;
		V2K m_v2k;
	};

	// bidirectional multi-valued map base
	template<typename K, typename V, typename K2V, typename V2K>
	class bdmvmap_base : public bdmap_base<K, V, K2V, V2K>
	{
		using bdmap_base<K, V, K2V, V2K>::m_k2v;
		using bdmap_base<K, V, K2V, V2K>::m_v2k;
	public:
		void insert(K const& key, V const& val) override
		{
			if (m_k2v.contains(key))
			{
				auto& set = m_v2k.at(m_k2v.at(key));
				set.erase(key);
			}
			m_k2v.insert_or_assign(key, val);
			insert_or_create(val, key);
		}
		void erase_key(K const& key) override
		{
			HAUL_ASSERT(m_k2v.contains(key));
			V const& val = m_k2v.at(key);
			erase_and_try_remove(val, key);
			m_k2v.erase(key);
		}
		void erase_val(V const& val) override
		{
			HAUL_ASSERT(m_v2k.contains(val));
			const auto& set = m_v2k.at(val);
			for (const K& key : set)
			{
				m_k2v.erase(key);
			}
			m_v2k.erase(val);
		}
		typename K2V::mapped_type replace_key(K const& key, V const& val) override
		{
			HAUL_ASSERT(m_k2v.contains(key));
			typename K2V::mapped_type oldval = m_k2v.at(key);
			erase_and_try_remove(oldval, key);
			m_k2v.insert_or_assign(key, val);
			insert_or_create(val, key);
			return oldval;
		}
		typename V2K::mapped_type replace_val(V const& val, K const& key) override
		{
			HAUL_ASSERT(m_v2k.contains(val));
			typename V2K::mapped_type const set = m_v2k.at(val);
			for (K const& key : set)
			{
				m_k2v.erase(key);
			}
			m_v2k.insert_or_assign(val, typename V2K::mapped_type{ key });
			m_k2v.insert({ key, val });
			return set;
		}
		typename V2K::mapped_type replace_val(V const& oval, V const& nval)
		{
			HAUL_ASSERT(m_v2k.contains(oval));
			typename V2K::mapped_type const set = m_v2k.at(oval);
			for (K const& key : set)
			{
				m_k2v.insert_or_assign(key, nval);
			}
			auto pair = m_v2k.extract(oval);
			pair.key() = nval;
			m_v2k.insert(std::move(pair));
			return set;
		}
	protected:
		bdmvmap_base() {}
	private:
		void insert_or_create(V const& val, K const& key)
		{
			if (m_v2k.contains(val))
			{
				m_v2k.at(val).insert(key);
			}
			else
			{
				m_v2k.insert({ val, { key } });
			}
		}
		void erase_and_try_remove(V const& val, K const& key)
		{
			auto& set = m_v2k.at(val);
			set.erase(key);
			if (!set.size())
			{
				m_v2k.erase(val);
			}
		}
	};
	// bidirectional map
	template<typename K, typename V>
	class bdmap : public bdmvmap_base<K, V, std::map<K, V>, std::map<V, std::unordered_set<K>>>
	{
	public:
		bdmap() {}
	};
	// bidirectional unordered_map
	template<typename K, typename V>
	class bdumap : public bdmvmap_base<K, V, std::unordered_map<K, V>, std::unordered_map<V, std::unordered_set<K>>>
	{
	public:
		bdumap() {}
	};

	// bidirectional unique-valued map base
	template<typename K, typename V, typename K2V, typename V2K>
	class bduvmap_base : public bdmap_base<K, V, K2V, V2K>
	{
		using bdmap_base<K, V, K2V, V2K>::m_k2v;
		using bdmap_base<K, V, K2V, V2K>::m_v2k;
	public:
		void insert(K const& key, V const& val) override
		{
			if (m_k2v.contains(key))
			{
				m_v2k.erase(m_k2v.at(key));
			}
			if (m_v2k.contains(val))
			{
				m_k2v.erase(m_v2k.at(val));
			}
			m_k2v.insert_or_assign(key, val);
			m_v2k.insert_or_assign(val, key);
		}
		void erase_key(K const& key) override
		{
			HAUL_ASSERT(m_k2v.contains(key));
			V const& val = m_k2v.at(key);
			m_v2k.erase(val);
			m_k2v.erase(key);
		}
		void erase_val(V const& val) override
		{
			HAUL_ASSERT(m_v2k.contains(val));
			K const& key = m_v2k.at(val);
			m_k2v.erase(key);
			m_v2k.erase(val);
		}
		typename K2V::mapped_type replace_key(K const& key, V const& val) override
		{
			HAUL_ASSERT(m_k2v.contains(key));
			typename K2V::mapped_type oldval = m_k2v.at(key);
			m_k2v.insert_or_assign(key, val);
			m_v2k.erase(oldval);
			m_v2k.insert_or_assign(val, key);
			return oldval;
		}
		typename V2K::mapped_type replace_val(V const& val, K const& key) override
		{
			HAUL_ASSERT(m_v2k.contains(val));
			typename V2K::mapped_type oldkey = m_v2k.at(val);
			m_v2k.insert_or_assign(val, key);
			m_k2v.erase(oldkey);
			m_k2v.insert_or_assign(key, val);
			return oldkey;
		}
	protected:
		bduvmap_base() {}
	};
	// bidirectional unique-valued map
	template<typename K, typename V>
	class bduvmap final : public bduvmap_base<K, V, std::map<K, V>, std::map<V, K>>
	{
	public:
		bduvmap() {}
	};
	// bidirectional unique-valued unique_map
	template<typename K, typename V>
	class bduvumap final : public bduvmap_base<K, V, std::unordered_map<K, V>, std::unordered_map<V, K>>
	{
	public:
		bduvumap() {}
	};
}