#ifdef __EDITOR__

#include "editor/imgui/views/Components/ComponentUIView.h"

#include <imgui/imgui_helpers.h>
#include <rapidjson/document.h>
#include <rapidjson/utils.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/prettywriter.h>

#include "editor/imgui/ImGuiDefines.h"
#include "editor/imgui/ImGuiWindow.h"
#include "utils/string_extensions.h"

namespace renegade
{
	namespace editor
	{
		namespace imgui
		{
            void ComponentBaseUIView::RenderBaseComponent(gameplay::Component& a_Component, gameplay::ECSSystem& a_System)
            {
				ImGui::SetNextItemOpen(m_FoldedOut);
				ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0);
				m_FoldedOut = ImGui::CollapsingHeader(IMGUI_FORMAT_ID(GetName(), FOLDOUT_ID, string_extensions::StringToUpper(GetName()) + "_INSPECTOR").c_str());
				if (m_FoldedOut)
				{
					ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
					ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0);

					ImVec2 size = ImVec2(30, 30);
					if (ImGui::TransparentButton(IMGUI_FORMAT_ID(std::string(ICON_FA_COPY), BUTTON_ID, string_extensions::StringToUpper(GetName()) + "_COPY_HIERARCHY").c_str(), size))
					{
						rapidjson::Document document;
						document.SetObject();
						a_Component.Serialize(document, document.GetAllocator());

						rapidjson::StringBuffer buffer;
						rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
						document.Accept(writer);

						core::Data data(buffer.GetString(), buffer.GetSize());
						core::ENGINE.GetEditor().SetClipboard(data);
					}
					ImGui::SameLine();
					if (ImGui::TransparentButton(IMGUI_FORMAT_ID(std::string(ICON_FA_PASTE), BUTTON_ID, string_extensions::StringToUpper(GetName()) + "_PASTE_HIERARCHY").c_str(), size))
					{
						rapidjson::Document document;
						document.SetObject();
						core::Data data = core::ENGINE.GetEditor().GetClipboard();
						document.Parse(reinterpret_cast<char*>(data.data()), data.size());
						a_Component.Deserialize(document, document.GetAllocator());
					}
					ImGui::SameLine();
					if (ImGui::TransparentButton(IMGUI_FORMAT_ID(std::string(ICON_FA_DELETE), BUTTON_ID, string_extensions::StringToUpper(GetName()) + "_DELETE_HIERARCHY").c_str(), size))
					{
						a_System.DeleteComponent(m_EntityID);
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
        }
	}
}

#endif // __EDITOR__