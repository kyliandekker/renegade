#ifdef __EDITOR__

#include "editor/imgui/views/ComponentUIView.h"

#include <imgui/imgui_helpers.h>

#include "editor/imgui/ImGuiDefines.h"
#include "core/Engine.h"
#include "utils/string_extensions.h"

namespace renegade
{
	namespace editor
	{
		namespace imgui
		{
			void ComponentUIView::Render()
			{
				ImGui::SetNextItemOpen(m_FoldedOut);
				ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0);
				m_FoldedOut = ImGui::CollapsingHeader(IMGUI_FORMAT_ID(ICON_FA_SCENE + std::string(" ") + GetName(), FOLDOUT_ID, string_extensions::StringToUpper(GetName()) + "_INSPECTOR").c_str());
				if (m_FoldedOut)
				{
					ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
					ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0);

					ImVec2 size = ImVec2(30, 30);
					if (ImGui::TransparentButton(IMGUI_FORMAT_ID(std::string(ICON_FA_COPY), BUTTON_ID, string_extensions::StringToUpper(GetName()) + "_COPY_HIERARCHY").c_str(), size))
					{
						CopyComponentData();
					}
					ImGui::SameLine();
					if (ImGui::TransparentButton(IMGUI_FORMAT_ID(std::string(ICON_FA_PASTE), BUTTON_ID, string_extensions::StringToUpper(GetName()) + "_PASTE_HIERARCHY").c_str(), size))
					{
						PasteComponentData();
					}
					ImGui::SameLine();
					if (ImGui::TransparentButton(IMGUI_FORMAT_ID(std::string(ICON_FA_DELETE), BUTTON_ID, string_extensions::StringToUpper(GetName()) + "_DELETE_HIERARCHY").c_str(), size))
					{
						DeleteComponent();
					}

					ImGui::PopStyleVar();
					ImGui::PopStyleVar();

					ImGui::SetCursorPosY(ImGui::GetCursorPosY() + ImGui::GetStyle().ItemSpacing.y);

					ImGui::Indent();
					RenderInner();
					ImGui::Unindent();
				}
				ImGui::PopStyleVar();
			}

			void ComponentUIView::DeleteComponent()
			{
			}

			void ComponentUIView::CopyComponentData()
			{
			}

			void ComponentUIView::PasteComponentData()
			{
			}
		}
	}
}

#endif // __EDITOR__