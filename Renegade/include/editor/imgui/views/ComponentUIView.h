#pragma once

#ifdef __EDITOR__

#include <string>

#include "editor/imgui/views/ImGuiUIView.h"

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

			class ComponentUIView : public ImGuiUIView
			{
			public:
				ComponentUIView(ImGuiWindow& a_Window, gameplay::EntityID& a_EntityID) : ImGuiUIView(a_Window), m_EntityID(a_EntityID)
				{}

				void Render() override;
			protected:
				gameplay::EntityID& m_EntityID;

				virtual void RenderInner() = 0;
				virtual void DeleteComponent();
				virtual std::string GetName() const = 0;
				bool m_FoldedOut = true;
			};
		}
	}
}

#endif // __EDITOR__