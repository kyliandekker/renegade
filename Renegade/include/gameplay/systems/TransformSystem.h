#pragma once

#include "gameplay/ECSBaseSystem.h"
#include "gameplay/systems/TransformComponent.h"

namespace renegade
{
	namespace gameplay
	{
		class TransformSystem : public ECSBaseSystem<TransformComponent>
		{
		public:
			TransformSystem() {}
			~TransformSystem() = default;

			virtual void Update(float a_DeltaTime) override {};
		};
	}
}