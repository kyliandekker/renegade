#pragma once

#ifdef __EDITOR__

#include "editor/imgui/EditorSelectable.h"

#include <vector>
#include <string>

#include "editor/imgui/StringTextInput.h"
#include "editor/imgui/StringDropdown.h"
#include "assets/AssetType.h"

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

			enum class ExplorerResourceEditMode
			{
				None,
				Rename
			};

			class ExplorerResourceUIView : public EditorSelectable
			{
			public:
				ExplorerResourceUIView(ImGuiWindow& a_Window);
				ExplorerResourceUIView();

				void SetResource(ExplorerResource* a_Resource);

				std::string m_Name;
				std::string m_Icon;

				// Whether the node is folded out.
				bool m_FoldedOut = false;

				// Whether it needs to be shown or not.
				bool m_Show = true;

				ExplorerResourceEditMode m_ResourceEditMode = ExplorerResourceEditMode::None;

				ExplorerResource* m_Resource = nullptr;
				ImageExplorerResource* m_ImageResource = nullptr;

				void ClearChildren();
				void GetChildren();
				virtual void RenderIcon();
				virtual void Render(bool& clicked, bool& right_clicked, bool& double_clicked, bool selected);

				bool HasFolders() const;

				std::vector<ExplorerResourceUIView> m_Resources;
				ExplorerResourceUIView* m_Parent = nullptr;

				// INSPECTOR.
				void RenderSelectable() override;
				StringTextInput m_NameInput;
				StringDropdown<assets::AssetType> m_AssetTypeDropdown;
			};
		}
	}
}

#endif // __EDITOR__