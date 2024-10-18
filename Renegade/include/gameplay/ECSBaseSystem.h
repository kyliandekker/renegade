#pragma once

#include "core/System.h"

#include <map>

#include "gameplay/EnityID.h"

namespace renegade
{
	namespace gameplay
	{
		class ECSSystem : public core::System
		{
		public:
			virtual ~ECSSystem() = default;

			bool Initialize(int a_NumArgs = 0, ...) override;
			bool Destroy() override;

			virtual void DeleteComponent(const EntityID& a_ID) = 0;
			virtual void Update(float a_DeltaTime) = 0;
			virtual bool IsIdHere(const EntityID& a_ID) = 0;
		};

		template <class ComponentType>
		class ECSBaseSystem : public ECSSystem
		{
		public:
			bool Initialize(int a_NumArgs = 0, ...) override;
			bool Destroy() override;

			virtual ~ECSBaseSystem() = default;

			template <typename... Args>
			ComponentType& CreateComponent(const EntityID& a_ID, Args... a_Args);

			size_t GetSize() const;

			bool HasComponent(const EntityID& a_ID);

			ComponentType* GetComponent(const EntityID& a_ID);

			void DeleteComponent(const EntityID& a_ID);

			virtual void Update(float a_DeltaTime) = 0;

		protected:
			// TODO: We can only have one for each entity. If I want multiple components this will be a problem.
			std::map<EntityID, ComponentType> m_Components;
		};
	}
}