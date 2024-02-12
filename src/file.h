#pragma once
#include <fstream>

namespace haul
{
	class file
	{
	public:
		HAUL_DCM(file);
		virtual ~file()
		{}
	public:
		virtual u64 get_pos()
		{
			return (u64)m_file.tellg();
		}
	protected:
		std::fstream m_file;
	protected:
		file(const std::string& fp, const std::ios_base::openmode& mode) :
			m_file(fp, mode)
		{
			HAUL_ASSERT(m_file.is_open());
		}
	};

	class input_file final : public file
	{
	private:
		std::vector<char> m_buf;
		u64 m_pos;
	public:
		input_file(const std::string& fp) :
			file(fp, std::ios::ate | std::ios::in | std::ios::binary),
			m_pos(0)
		{
			std::streamsize const bytes = m_file.tellg();
			m_file.seekg(0, std::ios::beg);
			std::vector<char> buffer(bytes);
			if (!m_file.read(buffer.data(), bytes))
			{
				__debugbreak();
			}
			m_file.close();
			m_buf = buffer;
		}
		HAUL_DCM(input_file);
	public:
		u64 get_pos() override
		{
			return m_pos;
		}
		template<typename T = u8>
		T get8()
		{
			const u8 val = m_buf[m_pos];
			m_pos++;
			return HAUL_CAST(T, val);
		}
		template<typename T = u16>
		T get16()
		{
			const u8 b0 = m_buf[m_pos + 0];
			const u8 b1 = m_buf[m_pos + 1];
			m_pos += 2;
			return HAUL_CAST(T, ((u16)b0) | ((u16)b1 << 8));
		}
		template<typename T = u32>
		T get32()
		{
			const u8 b0 = m_buf[m_pos + 0];
			const u8 b1 = m_buf[m_pos + 1];
			const u8 b2 = m_buf[m_pos + 2];
			const u8 b3 = m_buf[m_pos + 3];
			m_pos += 4;
			return HAUL_CAST(T, ((u32)b0) | ((u32)b1 << 8) | ((u32)b2 << 16) | ((u32)b3 << 24));
		}
		template<typename T = u64>
		T get64()
		{
			const u8 b0 = m_buf[m_pos + 0];
			const u8 b1 = m_buf[m_pos + 1];
			const u8 b2 = m_buf[m_pos + 2];
			const u8 b3 = m_buf[m_pos + 3];
			const u8 b4 = m_buf[m_pos + 4];
			const u8 b5 = m_buf[m_pos + 5];
			const u8 b6 = m_buf[m_pos + 6];
			const u8 b7 = m_buf[m_pos + 7];
			m_pos += 8;
			return HAUL_CAST(T, ((u64)b0) | ((u64)b1 << 8) | ((u64)b2 << 16) | ((u64)b3 << 24) | ((u64)b4 << 32) | ((u64)b5 << 40) | ((u64)b6 << 48) | ((u64)b7 << 56));
		}
		template<typename T>
		void get(T* const buf, size_t count)
		{
			const u64 bytes = count * sizeof(T) / sizeof(char);
			memcpy((void*)buf, m_buf.data() + m_pos, bytes);
			m_pos += bytes;
		}
		std::string get_string()
		{
			const u64 len = get64();
			char* buf = new char[len + 1];
			buf[len] = 0;
			get(buf, len);
			std::string const ret = buf;
			delete[] buf;
			return ret;
		}
	};

	class output_file final : public file
	{
	public:
		output_file(const std::string& fp) :
			file(fp, std::ios::out | std::ios::binary)
		{}
		HAUL_DCM(output_file);
	public:
		template<typename T = u8>
		output_file& put8(T b)
		{
			m_file.write((char*)&b, 1);
			return *this;
		}
		template<typename T = u16>
		output_file& put16(T s)
		{
			u8 bytes[2] = { 0, 0 };
			bytes[0] = s & 0xff;
			bytes[1] = (s >> 8) & 0xff;
			m_file.write((char*)bytes, 2);
			return *this;
		}
		template<typename T = u32>
		output_file& put32(u32 i)
		{
			u8 bytes[4] = { 0, 0, 0, 0 };
			bytes[0] = i & 0xff;
			bytes[1] = (i >> 8) & 0xff;
			bytes[2] = (i >> 16) & 0xff;
			bytes[3] = (i >> 24) & 0xff;
			m_file.write((char*)bytes, 4);
			return *this;
		}
		template<typename T = u64>
		output_file& put64(u64 l)
		{
			u8 bytes[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
			bytes[0] = l & 0xff;
			bytes[1] = (l >> 8) & 0xff;
			bytes[2] = (l >> 16) & 0xff;
			bytes[3] = (l >> 24) & 0xff;
			bytes[4] = (l >> 32) & 0xff;
			bytes[5] = (l >> 40) & 0xff;
			bytes[6] = (l >> 48) & 0xff;
			bytes[7] = (l >> 56) & 0xff;
			m_file.write((char*)bytes, 8);
			return *this;
		}
		template<typename T>
		output_file& put(const T* const data, size_t count)
		{
			m_file.write(reinterpret_cast<const char* const>(data), count * sizeof(T) / sizeof(char));
			return *this;
		}
		output_file& put_string(const std::string& s)
		{
			const u64 len = s.size();
			put64(len);
			put(s.data(), len);
			return *this;
		}
	};
} // namespace haul