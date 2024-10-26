#pragma once

#include "core/System.h"

// std::is_base_of
#include <type_traits> 
#include <map>
#include <string>
#include <vector>

#include "gameplay/EntityID.h"
#include "gameplay/systems/Component.h"
#include "core/Header.h"
#include "core/Engine.h"

namespace renegade
{
	namespace gameplay
	{
		class ECSSystem : public core::System
		{
		public:
			virtual ~ECSSystem() = default;
			ECSSystem() = default;

			bool Initialize(int a_NumArgs = 0, ...) override
			{
				return System::Initialize();
			}
			bool Destroy() override
			{
				return System::Destroy();
			}

			virtual std::string GetPropertyName() const
			{
				return "";
			}

			virtual void DeleteComponent(const EntityID& a_ID) = 0;
			virtual void Update(float a_DeltaTime) = 0;
			virtual bool ContainsID(const EntityID& a_ID) = 0;
			virtual Component* GetBaseComponent(const EntityID& a_ID) { return nullptr; };
		};

		template <class ComponentType>
		class ECSBaseSystem : public ECSSystem
		{
			static_assert(std::is_base_of<Component, ComponentType>::value,
				"ComponentType must be derived from Component");
		public:
			bool Initialize(int a_NumArgs = 0, ...) override
			{
				return ECSSystem::Initialize();
			}

			bool Destroy() override
			{
				m_Components.clear();
				return ECSSystem::Destroy();
			}

			virtual ~ECSBaseSystem() = default;

			ComponentType& CreateComponent(const EntityID& a_ID)
			{
				if (ContainsID(a_ID))
				{
					return m_Components[a_ID];
				}
				ComponentType t;
				m_Components.insert(std::make_pair(a_ID, t));
				return m_Components[a_ID];
			};

			size_t GetSize() const
			{
				return m_Components.size();
			}

			bool HasComponent(const EntityID& a_ID)
			{
				return m_Components.count(a_ID) == 1;
			}

			ComponentType& GetComponent(const EntityID& a_ID)
			{
				return ContainsID(a_ID) ? m_Components.at(a_ID) : CreateComponent(a_ID);
			}

			void DeleteComponent(const EntityID& a_ID)
			{
				if (a_ID.IsValid() && ContainsID(a_ID))
				{
					m_ComponentsToDelete.push_back(a_ID);
				}
			}

			Component* GetBaseComponent(const EntityID& a_ID) override
			{
				ComponentType& comp = GetComponent(a_ID);
				return &comp;
			};

			virtual void Update(float a_DeltaTime)
			{
				if (!m_ComponentsToDelete.empty())
				{
					for (EntityID& id : m_ComponentsToDelete)
					{
						m_Components.erase(id);
					}
					m_ComponentsToDelete.clear();
					core::ENGINE.GetECS().m_OnEntityComponentsUpdated();
				}
			}

			bool ContainsID(const EntityID& a_ID) override
			{
				return m_Components.count(a_ID) == 1;
			}
		protected:
			// TODO: We can only have one for each entity. If I want multiple components this will be a problem.
			std::map<EntityID, ComponentType> m_Components;
			std::vector<EntityID> m_ComponentsToDelete;
		};
	}
}