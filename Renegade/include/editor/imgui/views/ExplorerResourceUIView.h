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
		class SceneExplorerResource;

		namespace imgui
		{
			class ImGuiWindow;

			class ExplorerResourceUIView : public EditorSelectable
			{
			public:
				ExplorerResourceUIView(ImGuiWindow& a_Window, ExplorerResource& a_Resource);
				ExplorerResourceUIView(const ExplorerResourceUIView& a_Other);

				ExplorerResourceUIView& operator=(const ExplorerResourceUIView& a_Other);

				const std::string& GetName() const;
				const std::string& GetIcon() const;

				bool IsFoldedOut() const;
				void SetFoldedOut(bool a_FoldedOut);

				const ExplorerResource& GetResource() const;

				void ClearChildren();
				void GetChildren();
				void RenderIcon();
				void Render(bool& clicked, bool& right_clicked, bool& double_clicked, bool selected);
				void DoubleClicked();
				void Render() override {};

				bool HasFolders() const;

				std::vector<ExplorerResourceUIView> m_Resources;
				ExplorerResourceUIView* m_Parent = nullptr;

				// INSPECTOR.
				void RenderSelectable() override;
			private:
				void ReadResourceData();

				std::string m_Name;
				std::string m_Icon;

				// We must ensure there is always a resource. The other resource types are optional.
				ExplorerResource& m_Resource;

				union ResourceData
				{
					SceneExplorerResource* scene;
					ImageExplorerResource* image;
				};

				ResourceData m_ResourceData;

				// Whether the node is folded out.
				bool m_FoldedOut = false;

				// INSPECTOR.
				StringTextInput m_NameInput;
				StringDropdown<assets::AssetType> m_AssetTypeDropdown;
			};
		}
	}
}

#endif // __EDITOR__