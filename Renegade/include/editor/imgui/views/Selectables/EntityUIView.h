#pragma once

#ifdef __EDITOR__

#include "editor/imgui/views/Selectables/EditorSelectable.h"

#include <vector>

#include "gameplay/EntityID.h"
#include "editor/imgui/views/DataTypes/StringTextInput.h"
#include "editor/imgui/views/DataTypes/StringDropdown.h"

namespace renegade
{
	namespace editor
	{
		namespace imgui
		{
			class ComponentBaseUIView;
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

				gameplay::EntityID& GetEntityID() const;
			private:
				gameplay::EntityID& m_EntityID;
				std::vector<ComponentBaseUIView*> m_Components;

				// INSPECTOR.
				StringTextInput m_NameInput;
			};
		}
	}
}

#endif // __EDITOR__