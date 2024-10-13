#pragma once

#include "core/datatypes/DataStream.h"

namespace renegade
{
	namespace core
	{
		class ReserveDataStream : public DataStream
		{
		public:
			ReserveDataStream() = default;
			ReserveDataStream(size_t a_Size);
			ReserveDataStream(const ReserveDataStream& a_Rhs);
			ReserveDataStream(const DataStream& a_Rhs);

			ReserveDataStream& operator=(const ReserveDataStream& a_Other);

			void Free() override;

			/// <summary>
			/// Writes data to the datastream.
			/// </summary>
			bool Write(void const* a_Data, size_t a_Size) override;
			bool Save(const std::string& a_Path) const;
			bool Seek(size_t a_Offset, size_t a_Whence);
			size_t Tell() const;
		protected:
			void Reallocate(size_t a_ExtraSize);
			size_t m_ReservedSize = 0;
		};
	}
}