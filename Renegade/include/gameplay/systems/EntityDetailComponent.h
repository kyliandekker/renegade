#pragma once

#include "gameplay/systems/Component.h"

namespace renegade
{
	namespace gameplay
	{
		struct EntityID;

		class EntityDetailComponent : public Component
		{
		public:
			const std::string GetName() const;
			void SetName(const std::string& a_Name);

			bool IsActive() const;
			void SetActive(bool a_Active);

			EntityID* GetParent() const;
			void SetParent(EntityID& a_EntityID);

			std::string GetPropertyName() const override;
			void Serialize(rapidjson::Document& a_Document) const override;
			void Deserialize(const rapidjson::Document& a_Document) override;
		private:
			bool m_Active = true;
			std::string m_Name;
			EntityID* m_Parent = nullptr;
		};
	}
}