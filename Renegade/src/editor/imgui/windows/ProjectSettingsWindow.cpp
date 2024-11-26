#include "editor/imgui/windows/ProjectSettingsWindow.h"

#include "editor/imgui/ImGuiDefines.h"
#include "core/Engine.h"

namespace renegade
{
	namespace editor
	{
		namespace imgui
		{
			ProjectSettingsWindow::ProjectSettingsWindow(ImGuiWindow& a_Window) : BaseWindow(a_Window, ImGuiWindowFlags_NoCollapse, std::string(ICON_FA_PROJECT_SETTINGS) + " Project Settings", "Project Settings") 
			
			{
			}

			void ProjectSettingsWindow::Render()
			{
				ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetStyle().ItemSpacing.x);
				ImGui::SetCursorPosY(ImGui::GetCursorPosY() + ImGui::GetStyle().ItemSpacing.y);
				ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(m_Window.GetFramePadding().x * 2, m_Window.GetFramePadding().y * 2));
				if (ImGui::BeginChild(
					IMGUI_FORMAT_ID("", CHILD_ID, "PROJECT_SETTINGS_CHILD").c_str(),
					ImVec2(
						ImGui::GetContentRegionAvail().x - ImGui::GetStyle().ItemSpacing.x,
						ImGui::GetContentRegionAvail().y - ImGui::GetStyle().ItemSpacing.y
					),
					ImGuiChildFlags_Borders
				))
				{
				}
				ImGui::EndChild();
				ImGui::PopStyleVar();
			}
		}
	}
}