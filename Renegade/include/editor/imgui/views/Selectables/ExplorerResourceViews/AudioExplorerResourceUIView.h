#ifdef __EDITOR__

#pragma once

#include "editor/imgui/views/Selectables/ExplorerResourceViews/ExplorerResourceWithPreviewUIView.h"
#include "editor/imgui/views/DataTypes/StringDropdown.h"

namespace renegade
{
	namespace editor
	{
		class ExplorerResource;
		class AudioExplorerResource;
		class SoundExplorerResource;
		class SongExplorerResource;
		class VOExplorerResource;

		namespace imgui
		{
			class ImGuiWindow;

			template <class T>
			class AudioExplorerResourceUIView : public ExplorerResourceWithPreviewUIView
			{
			public:
				ExplorerResource* GetResource() override;
				void RenderSelectable() override;
			protected:
				void SetData(ExplorerResource* a_Resource) override;

				AudioExplorerResourceUIView(ImGuiWindow& a_Window, T* a_Resource);
				AudioExplorerResourceUIView(const AudioExplorerResourceUIView& a_Other);

				T& m_Resource;

				StringDropdown<assets::AssetType> m_AssetTypeDropdown;

				friend ExplorerResourceUIView;
			};

			class SoundExplorerResourceUIView : public AudioExplorerResourceUIView<SoundExplorerResource>
			{
			protected:
				SoundExplorerResourceUIView(ImGuiWindow& a_Window, SoundExplorerResource* a_Resource);
				SoundExplorerResourceUIView(const SoundExplorerResourceUIView& a_Other);

				friend ExplorerResourceUIView;
			};

			class SongExplorerResourceUIView : public AudioExplorerResourceUIView<SongExplorerResource>
			{
			protected:
				SongExplorerResourceUIView(ImGuiWindow& a_Window, SongExplorerResource* a_Resource);
				SongExplorerResourceUIView(const SongExplorerResourceUIView& a_Other);

				friend ExplorerResourceUIView;
			};

			class VOExplorerResourceUIView : public AudioExplorerResourceUIView<VOExplorerResource>
			{
			protected:
				VOExplorerResourceUIView(ImGuiWindow& a_Window, VOExplorerResource* a_Resource);
				VOExplorerResourceUIView(const VOExplorerResourceUIView& a_Other);

				friend ExplorerResourceUIView;
			};
		}
	}
}

#endif // __EDITOR__