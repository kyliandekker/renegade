#pragma once

#include "core/System.h"

#include <vector>
#include <string>
#include <mutex>

#include "gameplay/EntityID.h"
#include "core/datatypes/Event.h"
#include "core/Header.h"

namespace renegade
{
	namespace gameplay
	{
		class ECSSystem;

		class EntityComponentSystem : public core::System
		{
		public:
			SimpleEvent<> m_OnEntitiesUpdated;

			bool Initialize(int a_NumArgs = 0, ...) override;
			bool Destroy() override;

			void Update(const float& a_DeltaTime);

			bool IsPaused() const;
			void SetPaused(bool a_Paused);

			bool HasStarted() const;
			void SetStarted(bool a_Started);

			EntityID CreateEntity(const std::string& a_Name);
			void Delete(const EntityID& a_ID);
			bool IsEntityValid(const EntityID& a_ID) const;
			void Clear();

			std::string GetUniqueName(const std::string& a_Name);

			template <class T>
			T& CreateSystem()
			{
				T* system = new T();
				m_Systems.push_back(system);
				return *system;
			}

			template <class T>
			T& GetSystem()
			{
				for (ECSSystem* sys : m_Systems)
				{
					T* result = dynamic_cast<T*>(sys);
					if (result)
					{
						return *result;
					}
				}
				return CreateSystem<T>();
			};

			std::vector<EntityID>& GetEntities();
			std::vector<ECSSystem*> GetSystemsContainingEntity(const EntityID& a_ID);
			std::vector<ECSSystem*> GetSystems();
		private:
			void DeleteEntity(const EntityID& a_ID);
			void ClearEntities();

			bool m_Clear = false;
			std::vector<ECSSystem*> m_Systems;
			std::vector<EntityID> m_Entities;
			std::vector<EntityID> m_EntitiesToDelete;
			std::vector<EntityID> m_EntitiesToAdd;
			unsigned int m_NextID = 0;
			bool m_Started = false;
#ifdef __EDITOR__
			bool m_Paused = true;
#elif
			bool m_Paused = false;
#endif // __EDITOR__
		};
	}
}