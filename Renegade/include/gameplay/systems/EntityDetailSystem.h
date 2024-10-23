#pragma once

#include "gameplay/ECSBaseSystem.h"
#include "gameplay/systems/EntityDetailComponent.h"

namespace renegade
{
	namespace gameplay
	{
		class EntityDetailSystem : public ECSBaseSystem<EntityDetailComponent>
		{
		public:
			EntityDetailSystem() {}
			~EntityDetailSystem() = default;

			virtual void Update(float a_DeltaTime) override {};
		};
	}
}