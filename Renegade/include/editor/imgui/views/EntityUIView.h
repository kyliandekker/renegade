#pragma once

#ifdef __EDITOR__

#include "editor/imgui/EditorSelectable.h"

#include <vector>

#include "gameplay/EntityID.h"
#include "editor/imgui/StringTextInput.h"
#include "editor/imgui/StringDropdown.h"

namespace renegade
{
	namespace editor
	{
		namespace imgui
		{
			class ComponentUIView;
			class ImGuiWindow;

			class EntityUIView : public EditorSelectable
			{
			public:
				EntityUIView(ImGuiWindow& a_Window, gameplay::EntityID& a_EntityID);
				EntityUIView() = default;

				std::string GetIcon() const;

				void Render(bool& clicked, bool selected);
				void Render() override {};

				void RenderSelectable() override;
				void Select() override;
				void Deselect() override;
			private:
				gameplay::EntityID& m_EntityID;
				std::vector<ComponentUIView*> m_Components;

				// INSPECTOR.
				StringTextInput m_NameInput;
			};
		}
	}
}

#endif // __EDITOR__