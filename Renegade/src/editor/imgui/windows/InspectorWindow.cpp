#ifdef __EDITOR__

#include "editor/imgui/windows/InspectorWindow.h"

#include <imgui/imgui_helpers.h>

#include "editor/imgui/ImGuiDefines.h"
#include "logger/Logger.h"
#include "editor/imgui/ImGuiWindow.h"

namespace renegade
{
	namespace editor
	{
		namespace imgui
		{
			InspectorWindow::InspectorWindow(ImGuiWindow& a_Window) : BaseWindow(a_Window, ImGuiWindowFlags_NoCollapse, std::string(ICON_FA_INSPECTOR) + " Inspector", "Inspector")
			{ }

			void InspectorWindow::Render()
			{
				//if (ImGui::Button("Info"))
				//{
				//	LOGF(LOGSEVERITY_INFO, "This is an info message.");
				//}
				//ImGui::SameLine();
				//if (ImGui::Button("Warning"))
				//{
				//	LOGF(LOGSEVERITY_WARNING, "This is a warning message.");
				//}
				//ImGui::SameLine();
				//if (ImGui::Button("Error"))
				//{
				//	LOGF(LOGSEVERITY_ERROR, "This is an error message.");
				//}
				//if (ImGui::Button("Assert"))
				//{
				//	LOGF(LOGSEVERITY_ASSERT, "This is an assert message.");
				//}
				//ImGui::SameLine();
				//if (ImGui::Button("Success"))
				//{
				//	LOGF(LOGSEVERITY_SUCCESS, "This is a success message.");
				//}
				//ImGui::SameLine();
				//if (ImGui::Button("Info Success"))
				//{
				//	LOGF(LOGSEVERITY_INFO_SUCCESS, "This is an info success message.");
				//}
				//ImGui::SameLine();
				//if (ImGui::Button("Awesome"))
				//{
				//	LOGF(LOGSEVERITY_AWESOME, "This is an awesome message.");
				//}
			}
		}
	}
}

#endif // __EDITOR__