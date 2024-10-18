#include "gameplay/ECSBaseSystem.h"

namespace renegade
{
	namespace gameplay
	{
		bool ECSSystem::Initialize(int a_NumArgs, ...)
		{
			return System::Initialize();
		}

		bool ECSSystem::Destroy()
		{
			return System::Destroy();
		}

		template<class ComponentType>
		bool ECSBaseSystem<ComponentType>::Initialize(int a_NumArgs, ...)
		{
			return ECSSystem::Initialize();
		}

		template<class ComponentType>
		bool ECSBaseSystem<ComponentType>::Destroy()
		{
			m_Components.clear();
			return ECSSystem::Destroy();
		}

		template<class ComponentType>
		template<typename ...Args>
		ComponentType& ECSBaseSystem<ComponentType>::CreateComponent(const EntityID& a_ID, Args... a_Args)
		{
			if (IsIdHere(id))
			{
				return m_Components[id];
			}
			ComponentType t(args...);
			m_Components.insert(std::make_pair(id, t));
			return m_Components[id];
		};

		template <class ComponentType>
		size_t ECSBaseSystem<ComponentType>::GetSize() const
		{
			return m_Components.size();
		}

		template<class ComponentType>
		bool ECSBaseSystem<ComponentType>::HasComponent(const EntityID& a_ID)
		{
			return m_Components.count(id) == 1;
		}

		template<class ComponentType>
		ComponentType* ECSBaseSystem<ComponentType>::GetComponent(const EntityID& a_ID)
		{
			return IsIdHere(id) ? &m_Components.at(id) : nullptr;
		}

		template<class ComponentType>
		void ECSBaseSystem<ComponentType>::DeleteComponent(const EntityID& a_ID)
		{
			if (id.IsValid() && IsIdHere(id))
			{
				m_Components.erase(id);
			}
		}
	}
}