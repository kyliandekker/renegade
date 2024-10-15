#ifdef __EDITOR__

#include "editor/imgui/ImGuiUIView.h"

#include <imgui/imgui.h>
#include <imgui/imgui_helpers.h>

#include "editor/imgui/ImGuiWindow.h"

namespace renegade
{
	namespace editor
	{
		namespace imgui
		{
			ImGuiUIView::ImGuiUIView(ImGuiWindow& a_Window) : m_Window(a_Window)
			{ }

			ImGuiWindow& ImGuiUIView::Window() const
			{
				return m_Window;
			}

			void ImGuiUIView::BeginToolbar(const ImVec2& a_Size)
			{
				ImDrawList* draw_list = ImGui::GetWindowDrawList();

				ImVec2 header_pos = ImGui::GetCursorScreenPos();
				ImVec2 header_size = a_Size;

				ImVec2 header_min = header_pos;
				ImVec2 header_max = ImVec2(header_pos.x + header_size.x, header_pos.y + header_size.y);

				draw_list->AddRectFilled(header_min, header_max, ImGui::ImColorConvertFloat4ToU32(ImGui::GetStyleColorVec4(ImGuiCol_Button)), 0);

				ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
				ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0);
			}

			void ImGuiUIView::EndToolbar()
			{
				ImGui::PopStyleVar();
				ImGui::PopStyleVar();

				ImGui::SetCursorPosY(ImGui::GetCursorPosY() + m_Window.GetWindowPadding().y);
				ImGui::SetCursorPosX(ImGui::GetCursorPosX() + m_Window.GetWindowPadding().x);
			}
		}
	}
}

#endif // __EDITOR__