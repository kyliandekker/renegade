#pragma once

#include "editor/imgui/views/ExplorerResourceViews/ExplorerResourceWithPreviewUIView.h"
#include "editor/imgui/StringDropdown.h"

namespace renegade
{
	namespace editor
	{
		class ExplorerResource;
		class SceneExplorerResource;

		namespace imgui
		{
			class ImGuiWindow;

			class SceneExplorerResourceUIView : public ExplorerResourceWithoutPreviewUIView
			{
			public:
				ExplorerResource* GetResource() override;
				void RenderSelectable() override;
			protected:
				void DoubleClicked() override;

				SceneExplorerResourceUIView(ImGuiWindow& a_Window, SceneExplorerResource* a_Resource);
				SceneExplorerResourceUIView(const SceneExplorerResourceUIView& a_Other);

				SceneExplorerResource& m_Resource;

				friend ExplorerResourceUIView;
			};
		}
	}
}