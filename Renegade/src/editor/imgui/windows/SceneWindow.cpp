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
				ImVec2 header_size = ImVec2(ImGui::GetContentRegionAvail().x, m_Window.FontSize() * 2); // Width, Height
				BeginToolbar(header_size);

				float x = ImGui::GetContentRegionAvail().x / 2;
				x -= (header_size.y / 2);
				x -= (header_size.y / 2);

				ImGui::SetCursorPosX(x);

				if (ImGui::TransparentCheckboxButton(IMGUI_FORMAT_ID(f ? ICON_FA_STOP : ICON_FA_PLAY, BUTTON_ID, "PLAYSTOP_SCENE").c_str(), &f, ImVec2(header_size.y, header_size.y)))
				{ }
				ImGui::SameLine();

				if (ImGui::TransparentCheckboxButton(IMGUI_FORMAT_ID(ICON_FA_PAUSE, BUTTON_ID, "PAUSE_SCENE").c_str(), &b, ImVec2(header_size.y, header_size.y)))
				{ }

				EndToolbar();
			}
		}
	}
}

#endif // __EDITOR__