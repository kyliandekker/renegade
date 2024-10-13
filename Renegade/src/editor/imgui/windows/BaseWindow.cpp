#ifdef __EDITOR__

#include "editor/imgui/windows/BaseWindow.h"

#include <imgui/imgui.h>
#include <imgui/imgui_helpers.h>

#include "utils/string_extensions.h"
#include "editor/imgui/ImGuiWindow.h"

namespace renegade
{
	namespace editor
	{
		namespace imgui
		{
			BaseWindow::BaseWindow(ImGuiWindow& a_Window, ImGuiWindowFlags a_Flags, const std::string& a_Name, std::string a_WindowID, bool a_FullScreen) : m_Flags(a_Flags), m_Name(a_Name), m_WindowID(a_WindowID), m_FullScreen(a_FullScreen), ImGuiUIView(a_Window)
			{ }

			BaseWindow::~BaseWindow() = default;

			bool BaseWindow::WindowBegin()
			{
				return ImGui::Begin(std::string(m_Name + "###WINDOW_" + string_extensions::StringToUpper(m_WindowID)).c_str(), &m_Enabled, m_Flags);
			}

			void BaseWindow::WindowEnd()
			{
				ImGui::End();
			}

			void BaseWindow::Update()
			{
				if (!m_Enabled)
				{
					return;
				}

				if (m_FullScreen)
				{
					ImGui::SetNextWindowPos(ImVec2(0, 0));
					ImGui::SetNextWindowSize(ImVec2(m_Size.x, m_Size.y));
				}

				bool showRender = true;
				if (!m_RenderWindowItself)
				{
					showRender = WindowBegin();
				}

				if (showRender)
				{
					Render();
				}

				if (!m_RenderWindowItself)
				{
					WindowEnd();
				}
			}

			void BaseWindow::SetSize(ImVec2 a_Size)
			{
				m_Size = a_Size;
			}

			bool BaseWindow::IsFullScreen() const
			{
				return m_FullScreen;
			}

			void BaseWindow::SetEnabled(bool a_Enabled)
			{
				m_Enabled = a_Enabled;
			}

			void BaseWindow::ToggleEnabled()
			{
				m_Enabled = !m_Enabled;
			}

			std::string BaseWindow::GetName() const
			{
				return m_Name;
			}

			bool BaseWindow::IsEnabled() const
			{
				return m_Enabled;
			}

            void BaseWindow::BeginToolbar(const ImVec2& a_Size)
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

			void BaseWindow::EndToolbar()
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