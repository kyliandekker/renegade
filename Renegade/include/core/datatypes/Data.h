#pragma once

#include <cstdint>
#include <string>

namespace renegade
{
	namespace core
	{
		class Data
		{
		public:
			Data() = default;
			Data(const void* a_Data, size_t a_Size);
			Data(const char* a_Data, size_t a_Size);
			Data(size_t a_Size);
			Data(const Data& a_Rhs);
			~Data();

			Data& operator=(const Data& a_Other);

			const size_t size() const
			{
				return m_Size;
			}

			void* data() const
			{
				return m_Data;
			}

			virtual void Free();
			virtual void Clear();

			bool Save(const std::string& a_Path) const;

			unsigned char operator [] (int a_Index) const { return reinterpret_cast<unsigned char*>(m_Data)[a_Index]; }
			unsigned char& operator [] (int a_Index) { return reinterpret_cast<unsigned char*>(m_Data)[a_Index]; }
		protected:
			void* m_Data = nullptr;
			size_t m_Size = 0;
		};
	}
}