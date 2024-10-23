#pragma once

#include <string>

namespace renegade
{
	namespace gameplay
	{
		class TransformComponent;

		struct EntityID
		{
			EntityID(unsigned int a_ID) : m_ID(a_ID) {};
			EntityID() {};
			~EntityID() = default;

			bool IsValid() const { return m_ID != INVALID; };
			void SetInvalid() { m_ID = INVALID; }

			unsigned int GetID() const { return m_ID; }

			bool operator==(const EntityID& a_Other) const;
			bool operator!=(const EntityID& a_Other) const;
			bool operator<(const EntityID& a_Other) const;
		protected:
			enum ID_State : unsigned int
			{
				INVALID = 0
			};
			unsigned int m_ID = INVALID;
		};
	}
}