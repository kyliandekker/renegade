#pragma once

#ifdef __EDITOR__

#include "editor/imgui/views/ImGuiUIView.h"

#include <string>

#include "gameplay/systems/Component.h"
#include "gameplay/ECSBaseSystem.h"

namespace renegade
{
	namespace gameplay
	{
		class EntityID;
	}
	namespace editor
	{
		namespace imgui
		{
			class ImGuiWindow;

			class ComponentBaseUIView : public ImGuiUIView
			{
			public:
				ComponentBaseUIView(ImGuiWindow& a_Window, gameplay::EntityID& a_EntityID) : ImGuiUIView(a_Window), m_EntityID(a_EntityID)
				{}

				void Render() {};
				void RenderBaseComponent(gameplay::Component& a_Component, gameplay::ECSSystem& a_System);
			protected:
				gameplay::EntityID& m_EntityID;

				virtual void RenderInner() {};
				virtual std::string GetName() const { return ""; };
				bool m_FoldedOut = true;
			};

			template <class C, class S>
			class ComponentUIView : public ComponentBaseUIView
			{
			public:
				ComponentUIView(ImGuiWindow& a_Window, gameplay::EntityID& a_EntityID, C& a_Component, S& a_System) : ComponentBaseUIView(a_Window, a_EntityID), m_Component(a_Component), m_System(a_System)
				{}

				void Render() override
				{
					RenderBaseComponent(m_Component, m_System);
				}
			protected:
				C& m_Component;
				S& m_System;
			};
		}
	}
}

#endif // __EDITOR__