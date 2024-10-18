#include "gameplay/EntityComponentSystem.h"

#include "logger/Logger.h"

#include "gameplay/ECSBaseSystem.h"

namespace renegade
{
	namespace gameplay
	{
		bool EntityComponentSystem::Initialize(int, ...)
		{
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

		EntityID& EntityComponentSystem::CreateEntity()
		{
			EntityID id(++m_NextID);
			m_Entities.push_back(id);

			return m_Entities[m_Entities.size() - 1];
		}

		void EntityComponentSystem::DeleteEntity(const EntityID& a_ID)
		{
			for (auto& sys : m_Systems)
			{
				sys->DeleteComponent(a_ID);
			}
		};

		bool EntityComponentSystem::IsEntityValid(const EntityID& a_ID) const
		{
			return a_ID.IsValid();
		}

		template <class T>
		T& EntityComponentSystem::CreateSystem()
		{
			T* system = new T();
			m_Systems.insert(system);
			return *system;
		}

		template <class T>
		T& EntityComponentSystem::GetSystem()
		{
			for (ECSBaseSystem* sys : m_Systems)
			{
				T* result = dynamic_cast<T*>(sys);
				if (result)
				{
					return *result;
				}
			}
			return CreateSystem<T>();
		};
	}
}