#pragma once

#include "core/System.h"

#include <vector>

#include "gameplay/EnityID.h"

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

			EntityID& CreateEntity();
			void DeleteEntity(const EntityID& a_ID);
			bool IsEntityValid(const EntityID& id) const;

			template <class T>
			T& CreateSystem();

			template <class T>
			T& GetSystem();
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