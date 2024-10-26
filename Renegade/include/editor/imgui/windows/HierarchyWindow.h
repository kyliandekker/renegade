#pragma once

#ifdef __EDITOR__

#include "editor/imgui/windows/BaseWindow.h"

#include <vector>

#include "editor/imgui/StringTextInput.h"
#include "editor/imgui/views/EntityUIView.h"

namespace renegade
{
	namespace editor
	{
		namespace imgui
		{
			class ImGuiWindow;

			class HierarchyWindow : public BaseWindow
			{
			public:
				HierarchyWindow(ImGuiWindow& a_Window);

				bool Initialize() override;
				bool Destroy() override;

				void Render() override;

				void UpdateEntities();
				void UpdateEntityComponents();
			private:
				gameplay::EntityID m_LastID;
				bool m_NeedsRefresh = true;
				std::vector<EntityUIView> m_FilteredEntities;

				SearchBarInput m_SearchBar;
			};
		}
	}
}

#endif // __EDITOR__