#pragma once

#include "editor/imgui/views/Selectables/ExplorerResourceViews/ExplorerResourceWithPreviewUIView.h"

namespace renegade
{
	namespace editor
	{
		class ExplorerResource;

		namespace imgui
		{
			class ImGuiWindow;

			class DefaultExplorerResourceUIView : public ExplorerResourceWithoutPreviewUIView
			{
			public:
				ExplorerResource* GetResource() override;
				void RenderSelectable() override;
			protected:
				DefaultExplorerResourceUIView(ImGuiWindow& a_Window, ExplorerResource* a_Resource);
				DefaultExplorerResourceUIView(const DefaultExplorerResourceUIView& a_Other);

				ExplorerResource& m_Resource;

				friend ExplorerResourceUIView;
			};
		}
	}
}