#pragma once

#include "core/System.h"

#include <vector>
#include <string>

#include "gameplay/EntityID.h"

namespace renegade
{
	namespace gameplay
	{
		class ECSSystem;

		class EntityComponentSystem : public core::System
		{
		public:
			bool Initialize(int a_NumArgs = 0, ...) override;
			bool Destroy() override;

			void Update(const float& a_DeltaTime);

			bool IsPaused() const;
			void SetPaused(bool a_Paused);

			EntityID& CreateEntity(const std::string& a_Name);
			void DeleteEntity(const EntityID& a_ID);
			bool IsEntityValid(const EntityID& id) const;
			void ClearEntities()
			{
				m_Entities.clear();
			}

			std::string GetUniqueName(const std::string& a_Name)
			{
				std::string name = a_Name;

				bool found = true;
				int i = 0;
				while (found)
				{
					found = false;
					for (EntityID& entity : m_Entities)
					{
						if (entity.GetName() == name)
						{
							i++;
							if (i != 0)
							{
								name = a_Name + " (" + std::to_string(i) + ")";
								found = true;
							}
						}
					}
				}
				return name;
			}

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
		private:
			std::vector<ECSSystem*> m_Systems;
			std::vector<EntityID> m_Entities;
			unsigned int m_NextID = 0;
#ifdef __EDITOR__
			bool m_Paused = true;
#elif
			bool m_Paused = false;
#endif // __EDITOR__
		};
	}
}