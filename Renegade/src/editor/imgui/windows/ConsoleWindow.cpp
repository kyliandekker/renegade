#ifdef __EDITOR__

#include "editor/imgui/windows/ConsoleWindow.h"

#include <imgui/imgui_helpers.h>

#include "editor/imgui/ImGuiDefines.h"
#include "editor/EditorSettings.h"
#include "core/Engine.h"
#include "utils/string_extensions.h"

namespace renegade
{
	namespace editor
	{
		namespace imgui
		{
			ConsoleWindow::ConsoleWindow(ImGuiWindow& a_Window) : BaseWindow(a_Window, ImGuiWindowFlags_NoCollapse, std::string(ICON_FA_CONSOLE) + " Console", "Console"), m_SearchBar(a_Window)
			{
				m_SearchBar.Initialize("");

				// We want every log message. Not just the ones after ImGui has been initialized.
				logger::LOGGER.OnMessageLogged += std::bind(&ConsoleWindow::LoggerCallback, this, std::placeholders::_1);
			}

			ConsoleWindow::~ConsoleWindow()
			{
			}

			const char logo_arr[7][10] =
			{
				ICON_FA_INFO,
				ICON_FA_WARNING,
				ICON_FA_ERROR,
				ICON_FA_ASSERT,
				ICON_FA_SUCCESS,
				ICON_FA_INFO_SUCCESS,
				ICON_FA_AWESOME
			};

			ImVec4 colors_arr[7] =
			{
				ImGui::ConvertColorsRgba(0, 148, 255, 255),
				ImGui::ConvertColorsRgba(255, 216, 0, 255),
				ImGui::ConvertColorsRgba(255, 0, 0, 255),
				ImGui::ConvertColorsRgba(255, 0, 0, 255),
				ImGui::ConvertColorsRgba(76, 255, 0, 255),
				ImGui::ConvertColorsRgba(76, 255, 0, 255),
				ImGui::ConvertColorsRgba(255, 110, 220, 255),
			};

			void ConsoleWindow::Render()
			{
				EditorSettings& editorSettings = core::ENGINE.GetEditor().GetEditorSettings();

				// Filter messages if need be.
				if (m_NeedsRefresh)
				{
					std::vector<bool> filters =
					{
						editorSettings.Info(),
						editorSettings.Warning(),
						editorSettings.Error(),
						editorSettings.Assert(),
						editorSettings.Success(),
						editorSettings.InfoSuccess(),
						editorSettings.Awesome(),
					};

					// Mutex to ensure new messages cannot be added.
					std::lock_guard<std::mutex> lock(MESSAGE_MUTEX);

					m_NeedsRefresh = false;
					m_FilteredMessages.clear();

					std::string searchString = m_SearchBar.GetString();
					for (size_t i = 0; i < m_Messages.size(); i++)
					{
						auto& message = m_Messages[i];
						if (filters[(int)message.GetSeverity()])
						{
							if (searchString.empty() || (string_extensions::StringToLower(message.GetRawMessage()).find(searchString) != std::string::npos))
							{
								m_FilteredMessages.push_back(i);
							}
						}
					}
				}

				ImVec2 toolbarSize = ImVec2(ImGui::GetContentRegionAvail().x, m_Window.HeaderSize().y);
				ImGui::BeginToolbar(toolbarSize);

				ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
				ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0);

				if (ImGui::TransparentButton(
					IMGUI_FORMAT_ID(std::string(ICON_FA_ERASE) + " Clear", BUTTON_ID, "CLEAR_CONSOLE").c_str(), ImVec2(0, toolbarSize.y)))
				{
					std::lock_guard<std::mutex> lock(MESSAGE_MUTEX);
					m_FilteredMessages.clear();
					m_Messages.clear();
				}
				ImGui::SameLine();
				bool scrollToBottom = editorSettings.ScrollToBottom();
				if (ImGui::TransparentCheckboxButton(IMGUI_FORMAT_ID(std::string(ICON_FA_DOWN_TO_BOTTOM) + " Scroll To Bottom", BUTTON_ID, "SCROLL_TO_BOTTOM_CONSOLE").c_str(), &scrollToBottom, ImVec2(0, toolbarSize.y)))
				{
					editorSettings.SetScrollToBottom(scrollToBottom);
					m_NeedsRefresh = true;
				}

				ImGui::SameLine();
				bool info = editorSettings.Info();
				if (ImGui::TransparentCheckboxButton(IMGUI_FORMAT_ID(std::string(ICON_FA_INFO), BUTTON_ID, "SHOW_INFO_CONSOLE").c_str(), &info, ImVec2(toolbarSize.y, toolbarSize.y), colors_arr[LOGSEVERITY_INFO]))
				{
					editorSettings.SetInfo(info);
					m_NeedsRefresh = true;
				}

				ImGui::SameLine();
				bool warning = editorSettings.Warning();
				if (ImGui::TransparentCheckboxButton(IMGUI_FORMAT_ID(std::string(ICON_FA_WARNING), BUTTON_ID, "SHOW_WARNING_CONSOLE").c_str(), &warning, ImVec2(toolbarSize.y, toolbarSize.y), colors_arr[LOGSEVERITY_WARNING]))
				{
					editorSettings.SetWarning(warning);
					m_NeedsRefresh = true;
				}

				ImGui::SameLine();
				bool error = editorSettings.Error();
				if (ImGui::TransparentCheckboxButton(IMGUI_FORMAT_ID(std::string(ICON_FA_ERROR), BUTTON_ID, "SHOW_ERROR_CONSOLE").c_str(), &error, ImVec2(toolbarSize.y, toolbarSize.y), colors_arr[LOGSEVERITY_ERROR]))
				{
					editorSettings.SetError(error);
					m_NeedsRefresh = true;
				}

				ImGui::SameLine();
				bool assert = editorSettings.Assert();
				if (ImGui::TransparentCheckboxButton(IMGUI_FORMAT_ID(std::string(ICON_FA_ASSERT), BUTTON_ID, "SHOW_ASSERT_CONSOLE").c_str(), &assert, ImVec2(toolbarSize.y, toolbarSize.y), colors_arr[LOGSEVERITY_ASSERT]))
				{
					editorSettings.SetAssert(assert);
					m_NeedsRefresh = true;
				}

				ImGui::SameLine();
				bool success = editorSettings.Success();
				if (ImGui::TransparentCheckboxButton(IMGUI_FORMAT_ID(std::string(ICON_FA_SUCCESS), BUTTON_ID, "SHOW_SUCCESS_CONSOLE").c_str(), &success, ImVec2(toolbarSize.y, toolbarSize.y), colors_arr[LOGSEVERITY_SUCCESS]))
				{
					editorSettings.SetSuccess(success);
					m_NeedsRefresh = true;
				}

				ImGui::SameLine();
				bool infoSuccess = editorSettings.InfoSuccess();
				if (ImGui::TransparentCheckboxButton(IMGUI_FORMAT_ID(std::string(ICON_FA_INFO_SUCCESS), BUTTON_ID, "SHOW_INFO_SUCCESS_CONSOLE").c_str(), &infoSuccess, ImVec2(toolbarSize.y, toolbarSize.y), colors_arr[LOGSEVERITY_INFO_SUCCESS]))
				{
					editorSettings.SetInfoSuccess(infoSuccess);
					m_NeedsRefresh = true;
				}

				ImGui::SameLine();
				bool awesome = editorSettings.Awesome();
				if (ImGui::TransparentCheckboxButton(IMGUI_FORMAT_ID(std::string(ICON_FA_AWESOME), BUTTON_ID, "SHOW_AWESOME_CONSOLE").c_str(), &awesome, ImVec2(toolbarSize.y, toolbarSize.y), colors_arr[LOGSEVERITY_AWESOME]))
				{
					editorSettings.SetAwesome(awesome);
					m_NeedsRefresh = true;
				}

				ImGui::SameLine();
				float searchbarWidth = 200;
				ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetContentRegionAvail().x - (searchbarWidth + m_Window.GetWindowPadding().x));
				if (m_SearchBar.Render(IMGUI_FORMAT_ID("", INPUT_ID, "SEARCHBAR_CONSOLE").c_str(), ImVec2(searchbarWidth, toolbarSize.y), m_Window.GetFramePadding().x / 2))
				{
					m_NeedsRefresh = true;
				}

				ImGui::PopStyleVar();
				ImGui::PopStyleVar();

				ImGui::EndToolbar(ImVec2(ImGui::GetStyle().ItemSpacing.x, 0));

				ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(m_Window.GetFramePadding().x * 2, m_Window.GetFramePadding().y * 2));
				if (ImGui::BeginChild(
					IMGUI_FORMAT_ID("", CHILD_ID, "BOX_LOGGER").c_str(),
					ImVec2(
						ImGui::GetContentRegionAvail().x - ImGui::GetStyle().ItemSpacing.x,
						ImGui::GetContentRegionAvail().y - ImGui::GetStyle().ItemSpacing.y
					),
					ImGuiChildFlags_Borders
				))
				{
					static const float timestamp_width = ImGui::CalcTextSize("00:00:00:0000").x;

					ImGui::PushTextWrapPos();
					std::lock_guard<std::mutex> lock(MESSAGE_MUTEX);
					for (size_t i = 0; i < m_FilteredMessages.size(); i++)
					{
						auto& message = m_Messages[m_FilteredMessages[i]];

						ImVec4 color = colors_arr[(int)message.GetSeverity()];
						ImGui::TextColored(color, std::string(logo_arr[(int)message.GetSeverity()]).c_str());
						ImGui::SameLine();
						ImGui::TextUnformatted(message.GetRawMessage().c_str());

						time_t time_t = std::chrono::system_clock::to_time_t(message.GetTime());
						struct tm buf;

						localtime_s(&buf, &time_t);

						std::string s(30, '\0');
						std::strftime(&s[0], s.size(), "%Y-%m-%d %H:%M:%S", &buf);

						ImGui::TextColored(ImVec4(1, 1, 1, 0.5f), s.c_str());
						ImGui::SameLine(160);
						ImGui::TextColored(ImVec4(1, 1, 1, 0.5f), message.GetLocation().c_str());

						if (i < m_FilteredMessages.size() - 1)
						{
							ImGui::Separator();
						}

						if (scrollToBottom)
						{
							ImGui::SetScrollHereY(1.0f);
						}
					}

					ImGui::PopTextWrapPos();
				}
				ImGui::EndChild();
				ImGui::PopStyleVar();
			}

            bool ConsoleWindow::Initialize()
            {
				return BaseWindow::Initialize();
            }

			bool ConsoleWindow::Destroy()
			{
				logger::LOGGER.OnMessageLogged -= std::bind(&ConsoleWindow::LoggerCallback, this, std::placeholders::_1);
				return BaseWindow::Destroy();
			}

			void ConsoleWindow::Clear()
			{

			}

			void ConsoleWindow::AddMessage(const logger::Message& a_Message)
			{
				std::lock_guard<std::mutex> lock(MESSAGE_MUTEX);
				m_Messages.push_back(a_Message);
				m_NeedsRefresh = true;
			}

			void ConsoleWindow::LoggerCallback(const logger::Message& a_Message)
			{
				AddMessage(a_Message);
			}
		}
	}
}

#endif // __EDITOR__