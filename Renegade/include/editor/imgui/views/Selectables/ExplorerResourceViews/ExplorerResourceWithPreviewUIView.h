#ifdef __EDITOR__

#pragma once

#include "editor/imgui/views/Selectables/ExplorerResourceUIView.h"

namespace renegade
{
	namespace editor
	{
		class ExplorerResource;

		namespace imgui
		{
			class ImGuiWindow;

			class ExplorerResourceWithPreviewUIView : public ExplorerResourceUIView
			{
			public:
				void RenderBaseSelectable(ExplorerResource* a_Resource) override;
				void EndBaseSelectable() override;

				void RenderPreviewWindow();
				void EndPreviewWindow();
			protected:
				ExplorerResourceWithPreviewUIView(ImGuiWindow& a_Window);
				ExplorerResourceWithPreviewUIView(const ExplorerResourceWithPreviewUIView& a_Other);

				friend ExplorerResourceUIView;
			};

			class ExplorerResourceWithoutPreviewUIView : public ExplorerResourceUIView
			{
			public:
				void RenderBaseSelectable(ExplorerResource* a_Resource) override;
				void EndBaseSelectable() override;
			protected:
				ExplorerResourceWithoutPreviewUIView(ImGuiWindow& a_Window);
				ExplorerResourceWithoutPreviewUIView(const ExplorerResourceWithoutPreviewUIView& a_Other);

				friend ExplorerResourceUIView;
			};
		}
	}
}

#endif // __EDITOR__