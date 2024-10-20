#include "gameplay/EntityComponentSystem.h"

#include "logger/Logger.h"

#include "gameplay/ECSBaseSystem.h"

#include <gameplay/systems/TransformSystem.h>

namespace renegade
{
	namespace gameplay
	{
		bool EntityComponentSystem::Initialize(int, ...)
		{
			CreateSystem<TransformSystem>();

			LOGF(LOGSEVERITY_SUCCESS, "ECS initialized.");
			return System::Initialize();
		}
		
		bool EntityComponentSystem::Destroy()
		{
			for (ECSSystem* system : m_Systems)
			{
				delete system;
			}
			LOGF(LOGSEVERITY_SUCCESS, "ECS destroyed.");
			return System::Destroy();
		}

		void EntityComponentSystem::Update(const float& a_DeltaTime)
		{
			if (m_Paused)
			{
				return;
			}

			for (auto& sys : m_Systems)
			{
				sys->Update(a_DeltaTime);
			}
		}

		bool EntityComponentSystem::IsPaused() const
		{
			return m_Paused;
		}

		// TODO: Make it so that m_Paused gets set after the frame is finished. You know, threading and all that.
		void EntityComponentSystem::SetPaused(bool a_Paused)
		{
			m_Paused = a_Paused;
		}

		EntityID& EntityComponentSystem::CreateEntity(const std::string& a_Name)
		{
			EntityID id(++m_NextID, a_Name);
			m_Entities.push_back(id);

			return m_Entities[m_Entities.size() - 1];
		}

		void EntityComponentSystem::DeleteEntity(const EntityID& a_ID)
		{
			for (auto& sys : m_Systems)
			{
				sys->DeleteComponent(a_ID);
			}
			m_Entities.erase(
				std::remove_if(m_Entities.begin(), m_Entities.end(),
					[&a_ID](const EntityID& entity) { return entity == a_ID; }),
				m_Entities.end());
		};

		bool EntityComponentSystem::IsEntityValid(const EntityID& a_ID) const
		{
			return a_ID.IsValid();
		}

        std::vector<EntityID>& EntityComponentSystem::GetEntities()
        {
			return m_Entities;
        }
	}
}