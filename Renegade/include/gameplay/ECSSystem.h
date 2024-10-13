#pragma once

namespace renegade
{
	namespace gameplay
	{
		struct EntityID;

		class ECSSystem
		{
		public:
			// TODO add this, virtual void Finalize() = 0;
			virtual void DeleteComponent(EntityID& id) { (void)id; };
			virtual void Update(float deltaTime) = 0;
			virtual bool IsIdHere(EntityID& id)
			{
				(void)id;
				return false;
			};
			virtual bool HasComponents() { return false; };
			virtual void* GetComponentData(EntityID& id, size_t& size) { (void)id; size = 0; return nullptr; };
			const unsigned int Priority = 0;
		};
	}
}