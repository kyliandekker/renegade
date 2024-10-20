#pragma once

#ifdef __EDITOR__

#include <vector>

#include "editor/imgui/windows/BaseWindow.h"
#include "editor/imgui/StringTextInput.h"
#include "editor/imgui/views/ExplorerResourceUIView.h"

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
				ExplorerWindow(ImGuiWindow& a_Window);

				bool Initialize() override;

				bool m_ShowExplorerInspector = false;
				bool m_NeedsRefresh = true;

				char SEARCHSTRING_EXPLORER[256] = { '\0' };
				char EXPLORER_INSPECTOR_RENAME_RESOURCE[256] = { '\0' };

				std::vector<ExplorerResourceUIView*> m_FilteredResources;
				ExplorerResourceUIView m_AssetRoot;
				ExplorerResourceUIView* m_FolderRoot = nullptr;
				ExplorerResourceUIView* m_NewFolderRoot = nullptr;

				void SetExplorerRoot(ExplorerResourceUIView* a_Resource);
				void RenderFolder(ExplorerResourceUIView& a_Resource);
				void Render() override;

				void OnScanCompleted();
				void OnBeforeScan();
			private:
				SearchBarInput m_SearchBar;
			};
		}
	}
}

#endif // __EDITOR__