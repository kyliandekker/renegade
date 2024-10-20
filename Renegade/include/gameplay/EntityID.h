#pragma once

#include <string>

namespace renegade
{
	namespace gameplay
	{
		class TransformComponent;

		struct EntityID
		{
			EntityID(unsigned int a_ID, const std::string& a_Name) : m_ID(a_ID), m_Name(a_Name){};
			EntityID() {};
			~EntityID() = default;

			bool IsValid() const { return m_ID != INVALID; };
			void SetInvalid() { m_ID = INVALID; }

			unsigned int GetID() const { return m_ID; }

			bool operator==(const EntityID& a_Other) const;
			bool operator!=(const EntityID& a_Other) const;
			bool operator<(const EntityID& a_Other) const;

			TransformComponent* transform() const;

			const std::string GetName() const;
			void SetName(const std::string& a_Name);

			bool IsActive() const;
			void SetActive(bool a_Active);
		protected:
			bool m_Active = true ;
			std::string m_Name;
			enum ID_State : unsigned int
			{
				INVALID = 0
			};
			unsigned int m_ID = INVALID;
		};
	}
}