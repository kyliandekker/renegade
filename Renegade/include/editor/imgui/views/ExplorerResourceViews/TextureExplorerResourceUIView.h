#pragma once

#include "editor/imgui/views/ExplorerResourceViews/ExplorerResourceWithPreviewUIView.h"
#include "editor/imgui/StringDropdown.h"

namespace renegade
{
	namespace editor
	{
		class ExplorerResource;
		class ImageExplorerResource;
		class TextureExplorerResource;
		class SpriteExplorerResource;
		class FontExplorerResource;

		namespace imgui
		{
			class ImGuiWindow;

			template <class T>
			class ImageExplorerResourceUIView : public ExplorerResourceWithPreviewUIView
			{
			public:
				ExplorerResource* GetResource() override;
				void RenderSelectable() override;
			protected:
				void SetData(ExplorerResource* a_Resource) override;

				ImageExplorerResourceUIView(ImGuiWindow& a_Window, T* a_Resource);
				ImageExplorerResourceUIView(const ImageExplorerResourceUIView& a_Other);

				T& m_Resource;

				StringDropdown<assets::AssetType> m_AssetTypeDropdown;

				friend ExplorerResourceUIView;
			};

			class TextureExplorerResourceUIView : public ImageExplorerResourceUIView<TextureExplorerResource>
			{
			protected:
				TextureExplorerResourceUIView(ImGuiWindow& a_Window, TextureExplorerResource* a_Resource);
				TextureExplorerResourceUIView(const TextureExplorerResourceUIView& a_Other);

				friend ExplorerResourceUIView;
			};

			class SpriteExplorerResourceUIView : public ImageExplorerResourceUIView<SpriteExplorerResource>
			{
			protected:
				SpriteExplorerResourceUIView(ImGuiWindow& a_Window, SpriteExplorerResource* a_Resource);
				SpriteExplorerResourceUIView(const SpriteExplorerResourceUIView& a_Other);

				friend ExplorerResourceUIView;
			};

			class FontExplorerResourceUIView : public ImageExplorerResourceUIView<FontExplorerResource>
			{
			protected:
				FontExplorerResourceUIView(ImGuiWindow& a_Window, FontExplorerResource* a_Resource);
				FontExplorerResourceUIView(const FontExplorerResourceUIView& a_Other);

				friend ExplorerResourceUIView;
			};
		}
	}
}