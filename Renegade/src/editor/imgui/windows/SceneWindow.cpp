#ifdef __EDITOR__

#include "editor/imgui/windows/SceneWindow.h"

#include <imgui/imgui_helpers.h>

#include "editor/imgui/ImGuiDefines.h"
#include "editor/imgui/ImGuiWindow.h"

namespace renegade
{
	namespace editor
	{
		namespace imgui
		{
			SceneWindow::SceneWindow(ImGuiWindow& a_Window) : BaseWindow(a_Window, ImGuiWindowFlags_NoCollapse, std::string(ICON_FA_SCENE) + " Scene", "Scene")
			{ }

			bool f = false, b = false;
			void SceneWindow::Render()
			{
				ImVec2 toolbarSize = ImVec2(ImGui::GetContentRegionAvail().x, m_Window.HeaderSize().y);
				ImGui::BeginToolbar(toolbarSize);

				float x = ImGui::GetContentRegionAvail().x / 2;
				x -= (toolbarSize.y / 2);
				x -= (toolbarSize.y / 2);

				ImGui::SetCursorPosX(x);

				if (ImGui::TransparentCheckboxButton(IMGUI_FORMAT_ID(f ? ICON_FA_STOP : ICON_FA_PLAY, BUTTON_ID, "PLAYSTOP_SCENE").c_str(), &f, ImVec2(toolbarSize.y, toolbarSize.y)))
				{ }
				ImGui::SameLine();

				if (ImGui::TransparentCheckboxButton(IMGUI_FORMAT_ID(ICON_FA_PAUSE, BUTTON_ID, "PAUSE_SCENE").c_str(), &b, ImVec2(toolbarSize.y, toolbarSize.y)))
				{ }

				ImGui::EndToolbar(m_Window.GetWindowPadding());
			}
		}
	}
}

#endif // __EDITOR__