#pragma once

#ifdef __EDITOR__

#include <vector>

#include "editor/imgui/windows/BaseWindow.h"
#include "editor/imgui/StringTextInput.h"

namespace renegade
{
	namespace editor
	{
		class ExplorerResource;

		namespace imgui
		{
			class ImGuiWindow;

			class ExplorerWindow : public BaseWindow
			{
			public:
				editor::ExplorerResource* m_SelectedResource = nullptr;
				editor::ExplorerResource* m_ExplorerRoot = nullptr;
				editor::ExplorerResource* m_NewExplorerRoot = nullptr;

				bool m_ShowContextMenu = false;
				bool m_ShowExplorerInspector = false;
				bool m_NeedsRefresh = true;
				bool m_NeedsRescan = false;

				char SEARCHSTRING_EXPLORER[256] = { '\0' };
				char EXPLORER_INSPECTOR_RENAME_RESOURCE[256] = { '\0' };

				std::vector<editor::ExplorerResource*> m_ResourcesToShow;

				ExplorerWindow(ImGuiWindow& a_Window);
				void SetExplorerRoot(editor::ExplorerResource* a_Resource);
				void Rescan();
				void RenderFolder(editor::ExplorerResource* a_Resource);
				void Render() override;
			private:
				SearchBarInput m_SearchBar;
			};
		}
	}
}

#endif // __EDITOR__