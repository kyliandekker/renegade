#pragma once

#ifdef __EDITOR__

#include "editor/imgui/EditorSelectable.h"

#include <vector>
#include <string>

#include "editor/imgui/StringTextInput.h"
#include "assets/AssetType.h"

namespace renegade
{
	namespace editor
	{
		class ExplorerResource;
		enum class ExplorerResourceType;

		namespace imgui
		{
			class ImGuiWindow;

			class ExplorerResourceUIView : public EditorSelectable
			{
			public:
				virtual ~ExplorerResourceUIView();

				static ExplorerResourceUIView* CreateViewFromExplorerResource(ExplorerResource* a_Resource, ImGuiWindow& a_Window);

				// EXPLORER.
				const std::string& GetName() const;
				const std::string& GetIcon() const;

				bool IsFoldedOut() const;
				void SetFoldedOut(bool a_FoldedOut);

				void Render(bool& clicked, bool& right_clicked, bool& double_clicked, bool selected);
				virtual void RenderIcon();
				virtual void DoubleClicked() {};
				void Render() override {};

				// INSPECTOR.
				virtual void RenderBaseSelectable(ExplorerResource* a_Resource);
				virtual void EndBaseSelectable();

				virtual bool HasFolders() const;
				virtual ExplorerResource* GetResource() { return nullptr; }

				std::vector<ExplorerResourceUIView*> m_Resources;
				ExplorerResourceUIView* m_Parent = nullptr;

				void ClearChildren();
			protected:
				virtual void SetData(ExplorerResource* a_Resource);

				ExplorerResourceUIView(ImGuiWindow& a_Window);

				std::string m_Name;
				std::string m_Icon;
				std::string m_StrResourceType;
				ExplorerResourceType m_ResourceType;

				// Whether the node is folded out.
				bool m_FoldedOut = false;

				// INSPECTOR.
				StringTextInput m_NameInput;
			};
		}
	}
}

#endif // __EDITOR__