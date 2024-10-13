#pragma once

#include "core/datatypes/Data.h"
#include <cstdint>

#ifndef SEEK_SET
#define SEEK_SET 0
#endif
#ifndef SEEK_CUR
#define SEEK_CUR 1
#endif
#ifndef SEEK_END
#define SEEK_END 2
#endif

namespace renegade
{
	namespace core
	{
		class DataStream : public Data
		{
		public:
			DataStream() = default;
			DataStream(void* a_Data, size_t a_Size);
			DataStream(size_t a_Size);
			DataStream(const DataStream& a_Rhs);
			DataStream(const Data& a_Rhs);

			DataStream& operator=(const DataStream& a_Other);

			void Free() override;

			/// <summary>
			/// Stores the data from the DataStream in a memory stream provided by the user.
			/// </summary>
			bool Read(void* a_Data, size_t a_DataSize, size_t a_Size);
			/// <summary>
			/// Writes data to the datastream.
			/// </summary>
			virtual bool Write(void const* a_Data, size_t a_Size);

			bool Save(const std::string& a_Path) const;

			bool Seek(size_t a_Offset, size_t a_Whence);
			size_t Tell() const;
		protected:
			size_t m_Pos = 0;
		};
	}
}