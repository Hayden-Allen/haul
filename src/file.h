#pragma once
#include <fstream>

namespace haul
{
	template<std::ios_base::openmode FLAGS>
	class file
	{
	public:
		HAUL_DCM(file);
		virtual ~file()
		{
			m_file.close();
		}
	protected:
		std::fstream m_file;
	protected:
		file(const std::string& fp) :
			m_file(fp, FLAGS)
		{
			HAUL_ASSERT(m_file.is_open());
		}
	};

	class input_file final : public file<std::ios::binary | std::ios::in>
	{
	public:
		input_file(const std::string& fp) :
			file<std::ios::binary | std::ios::in>(fp)
		{}
		HAUL_DCM(input_file);
	public:
		bool is_eof() const
		{
			return m_file.eof();
		}
		template<typename T = u8>
		T get8()
		{
			return HAUL_CAST(T, m_file.get());
		}
		template<typename T = u16>
		T get16()
		{
			return HAUL_CAST(T, (HAUL_CAST(u16, get8()) << 8) | get8());
		}
		template<typename T = u32>
		T get32()
		{
			return HAUL_CAST(T, (HAUL_CAST(u32, get16()) << 16) | get16());
		}
		template<typename T = u64>
		T get64()
		{
			return HAUL_CAST(T, (HAUL_CAST(u64, get32()) << 32) | get32());
		}
		template<typename T>
		void get(T* const buf, u64 const num_elements)
		{
			m_file.read(reinterpret_cast<char* const>(buf), num_elements * sizeof(T) / sizeof(char));
		}
	};

	class output_file final : public file<std::ios::binary | std::ios::out>
	{
	public:
		output_file(const std::string& fp) :
			file<std::ios::binary | std::ios::out>(fp)
		{}
		HAUL_DCM(output_file);
	public:
		template<typename T = u8>
		output_file& put8(T const b)
		{
			return put(HAUL_CAST(u64, b), 1);
		}
		template<typename T = u16>
		output_file& put16(T const s)
		{
			return put(HAUL_CAST(u64, s), 2);
		}
		template<typename T = u32>
		output_file& put32(T const i)
		{
			return put(HAUL_CAST(u64, i), 4);
		}
		template<typename T = u64>
		output_file& put64(T const l)
		{
			return put(l, 8);
		}
		template<typename T>
		output_file& put(T const* const data, u64 const num_elements)
		{
			m_file.write(reinterpret_cast<char const* const>(data), num_elements * sizeof(T) / sizeof(char));
			return *this;
		}
	private:
		output_file& put(u64 const data, u8 const num_bytes)
		{
			HAUL_ASSERT(num_bytes <= 8);

			for (s32 i = HAUL_CAST(s32, num_bytes - 1); i >= 0; i--)
			{
				uint64_t mask = HAUL_CAST(u64, 0xff) << (HAUL_CAST(u64, i) * 8);
				m_file << HAUL_CAST(u8, (data & mask) >> i * 8);
			}

			return *this;
		}
	};
}