#pragma once

#ifdef __EDITOR__

#include "editor/imgui/windows/BaseWindow.h"

#include <vector>

namespace renegade
{
	namespace editor
	{
		namespace imgui
		{
			class HierarchyWindow : public BaseWindow
			{
			public:
				HierarchyWindow(ImGuiWindow& a_Window);
				void Render() override;

				void SetShowContextMenu(bool a_ShowContextMenu);
				void SetIsRenaming(bool a_IsRenaming);

				bool ShowContextMenu() const;
				bool IsRenaming() const;
			private:
				bool m_ShowContextMenu = false;
				bool m_IsRenaming = false;
				bool m_NeedsRefresh = true;
			};
		}
	}
}

#endif // __EDITOR__