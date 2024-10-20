#ifdef __EDITOR__

#include "editor/imgui/ImGuiWindow.h"

#include <cstdint>
#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_win32.h>
#include <imgui/backends/imgui_impl_dx12.h>
// va_list, va_start, va_end
#include <cstdarg>

#include "core/Engine.h"
#include "logger/Logger.h"
#include "editor/imgui/ImguiDefines.h"
#include "editor/imgui/font_file.h"
#include "editor/EditorSettings.h"
#include "editor/Editor.h"
#include "file/FileLoader.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace renegade
{
	namespace editor
	{
		namespace imgui
		{
			ImGuiWindow::ImGuiWindow() : mainWindow(*this), consoleWindow(*this), sceneWindow(*this), inspectorWindow(*this), hierarchyWindow(*this), explorerWindow(*this)
			{ }

			ImGuiWindow::~ImGuiWindow()
			{ }

			bool ImGuiWindow::Initialize(int, ...)
			{
				IMGUI_CHECKVERSION();
				ImGui::CreateContext();

				m_IniPath = std::string(file::FileLoader::GetAppDataPath() + SETTINGS_FOLDER + "imgui.ini");
				ImGuiIO& io = ImGui::GetIO();
				io.IniFilename = m_IniPath.c_str();

				if (!CreateContextWin32() || !CreateContextDX12())
				{
					return false;
				}

				CreateImGui();

				LOGF(LOGSEVERITY_SUCCESS, "Created ImGui.");

				mainWindow.Initialize();
				consoleWindow.Initialize();
				sceneWindow.Initialize();
				inspectorWindow.Initialize();
				hierarchyWindow.Initialize();
				explorerWindow.Initialize();

				return System::Initialize();
			}

			bool ImGuiWindow::Destroy()
			{
				ImGui_ImplDX12_Shutdown();
				ImGui_ImplWin32_Shutdown();
				ImGui::DestroyContext();

				LOGF(LOGSEVERITY_SUCCESS, "ImGui destroyed.");
				return true;
			}

            LRESULT ImGuiWindow::WndProcHandler(HWND a_hWnd, UINT a_Msg, WPARAM a_wParam, LPARAM a_lParam)
            {
				if (!m_Ready)
				{
					return a_Msg;
				}

				ImGui_ImplWin32_WndProcHandler(a_hWnd, a_Msg, a_wParam, a_lParam);
				return a_Msg;
            }

			void ImGuiWindow::OnRenderTargetCreated()
			{
				if (!m_Ready)
				{
					return;
				}

				ImGui_ImplDX12_CreateDeviceObjects();

				glm::vec2 size = core::ENGINE.GetWindow().GetRealSize();
				ImGuiIO& io = ImGui::GetIO();
				io.DisplaySize.x = size.x;
				io.DisplaySize.y = size.y;

				core::ENGINE.GetEditor().GetEditorSettings().SetSize(size);
			}

			void ImGuiWindow::OnRenderTargetCleaned()
			{
				if (!m_Ready)
				{
					return;
				}

				ImGui_ImplDX12_InvalidateDeviceObjects();
			}

			void ImGuiWindow::OnDrawRenderData(ID3D12GraphicsCommandList* a_CommandList)
			{
				if (!m_Ready)
				{
					return;
				}

				ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), a_CommandList);
			}

			bool ImGuiWindow::CreateContextWin32()
			{
				if (!ImGui_ImplWin32_Init(core::ENGINE.GetWindow().GetHWnd()))
				{
					LOGF(LOGSEVERITY_ERROR, "Could not create a Win32 context for ImGui.");
					return false;
				}

				LOGF(LOGSEVERITY_SUCCESS, "Created a Win32 context for ImGui.");
				return true;
			}

			bool ImGuiWindow::CreateContextDX12()
			{
				graphics::DX12Window& dx12window = core::ENGINE.GetWindow().GetDX12Window();
				if (!ImGui_ImplDX12_Init(dx12window.g_pd3dDevice, dx12window.NUM_FRAMES_IN_FLIGHT,
					DXGI_FORMAT_R8G8B8A8_UNORM, dx12window.g_pd3dSrvDescHeap,
					dx12window.g_pd3dSrvDescHeap->GetCPUDescriptorHandleForHeapStart(),
					dx12window.g_pd3dSrvDescHeap->GetGPUDescriptorHandleForHeapStart()))
				{
					LOGF(LOGSEVERITY_ERROR, "Could not create a DX12 context for ImGui.");
					return false;
				}

				LOGF(LOGSEVERITY_SUCCESS, "Created a DX12 context for ImGui.");
				return true;
			}

			ImFont* m_DefaultFont;
			ImFont* m_Capital;
			ImFont* m_CapitalIcon;
			//ImFont* m_Small;
			ImFont* m_H1;
			ImFont* m_BoldFont;
			ImFont* m_ItalicsFont;
			ImFont* m_BoldItalicsFont;
			ImFont* m_IconFont;

			void ImGuiWindow::CreateImGui()
			{
				ImGuiIO& io = ImGui::GetIO();
				io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
				io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
				io.ConfigFlags &= ~ImGuiConfigFlags_NoMouseCursorChange;
				(void)io;

				// setup Dear ImGui style
				ImGui::StyleColorsDark();

				ImFont* font1 = io.Fonts->AddFontDefault();
				(void)font1;

				constexpr ImWchar icons_ranges_b[] = { ICON_MIN_FA, ICON_MAX_16_FA, 0 };

				ImFontConfig font_config_capital;
				font_config_capital.FontDataOwnedByAtlas = false;
				m_Capital = io.Fonts->AddFontFromMemoryTTF(&font::arial, sizeof(font::arial), m_FontSize + 10, &font_config_capital);

				ImFontConfig font_config_small;
				font_config_small.FontDataOwnedByAtlas = false;
				font_config_small.MergeMode = true;
				font_config_small.PixelSnapH = true;
				font_config_small.FontDataOwnedByAtlas = false;
				m_CapitalIcon = io.Fonts->AddFontFromMemoryTTF(&font::fontAwesome, sizeof(font::fontAwesome), m_FontSize + 10, &font_config_small, icons_ranges_b);

				ImFontConfig font_config_h1;
				font_config_h1.FontDataOwnedByAtlas = false;
				m_H1 = io.Fonts->AddFontFromMemoryTTF(&font::arial, sizeof(font::arial), m_FontSize + 10, &font_config_h1);

				ImFontConfig font_config_bold;
				font_config_bold.FontDataOwnedByAtlas = false;
				m_BoldFont = io.Fonts->AddFontFromMemoryTTF(&font::arialBold, sizeof(font::arialBold), m_FontSize, &font_config_bold);

				ImFontConfig font_config_italics;
				font_config_italics.FontDataOwnedByAtlas = false;
				m_ItalicsFont = io.Fonts->AddFontFromMemoryTTF(&font::arialItalics, sizeof(font::arialItalics), m_FontSize, &font_config_italics);

				ImFontConfig font_config_boldItalics;
				font_config_boldItalics.FontDataOwnedByAtlas = false;
				m_BoldItalicsFont = io.Fonts->AddFontFromMemoryTTF(&font::arialBoldItalics, sizeof(font::arialBoldItalics), m_FontSize, &font_config_boldItalics);

				ImFontConfig font_config_default;
				font_config_default.FontDataOwnedByAtlas = false;
				m_DefaultFont = io.Fonts->AddFontFromMemoryTTF(&font::arial, sizeof(font::arial), m_FontSize, &font_config_default);

				ImFontConfig icons_config_b;
				icons_config_b.MergeMode = true;
				icons_config_b.PixelSnapH = true;
				icons_config_b.FontDataOwnedByAtlas = false;
				m_IconFont = io.Fonts->AddFontFromMemoryTTF(&font::fontAwesome, sizeof(font::fontAwesome), m_FontSize, &icons_config_b, icons_ranges_b);
				io.Fonts->Build();

				ImGui::StyleColorsDark();

				ImGuiStyle& style = ImGui::GetStyle();
				ImVec4* colors = style.Colors;
				colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
				colors[ImGuiCol_TextDisabled] = ImVec4(0.24f, 0.24f, 0.24f, 1.00f);
				colors[ImGuiCol_WindowBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
				colors[ImGuiCol_ChildBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
				colors[ImGuiCol_PopupBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
				colors[ImGuiCol_Border] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
				colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
				colors[ImGuiCol_FrameBg] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
				colors[ImGuiCol_FrameBgHovered] = ImVec4(0.24f, 0.24f, 0.24f, 1.00f);
				colors[ImGuiCol_FrameBgActive] = ImVec4(0.24f, 0.24f, 0.24f, 1.00f);
				colors[ImGuiCol_TitleBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
				colors[ImGuiCol_TitleBgActive] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
				colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
				colors[ImGuiCol_MenuBarBg] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
				colors[ImGuiCol_ScrollbarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
				colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.24f, 0.24f, 0.24f, 1.00f);
				colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.24f, 0.24f, 0.24f, 1.00f);
				colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.24f, 0.24f, 0.24f, 1.00f);
				colors[ImGuiCol_CheckMark] = ImVec4(0.86f, 0.93f, 0.89f, 1.00f);
				colors[ImGuiCol_SliderGrab] = ImVec4(0.24f, 0.24f, 0.24f, 1.00f);
				colors[ImGuiCol_SliderGrabActive] = ImVec4(0.24f, 0.24f, 0.24f, 1.00f);
				colors[ImGuiCol_Button] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
				colors[ImGuiCol_ButtonHovered] = ImVec4(0.24f, 0.24f, 0.24f, 1.00f);
				colors[ImGuiCol_ButtonActive] = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
				colors[ImGuiCol_Header] = ImVec4(0.24f, 0.24f, 0.24f, 1.00f);
				colors[ImGuiCol_HeaderHovered] = ImVec4(0.24f, 0.24f, 0.24f, 1.00f);
				colors[ImGuiCol_HeaderActive] = ImVec4(0.24f, 0.24f, 0.24f, 1.00f);
				colors[ImGuiCol_Separator] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
				colors[ImGuiCol_SeparatorHovered] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
				colors[ImGuiCol_SeparatorActive] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
				colors[ImGuiCol_ResizeGrip] = ImVec4(0.24f, 0.24f, 0.24f, 1.00f);
				colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.24f, 0.24f, 0.24f, 1.00f);
				colors[ImGuiCol_ResizeGripActive] = ImVec4(0.24f, 0.24f, 0.24f, 1.00f);
				colors[ImGuiCol_Tab] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
				colors[ImGuiCol_TabHovered] = ImVec4(0.24f, 0.24f, 0.24f, 1.00f);
				colors[ImGuiCol_TabActive] = ImVec4(0.24f, 0.24f, 0.24f, 1.00f);
				colors[ImGuiCol_TabUnfocused] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
				colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.24f, 0.24f, 0.24f, 1.00f);
				colors[ImGuiCol_PlotLines] = ImVec4(0.86f, 0.93f, 0.89f, 1.00f);
				colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.86f, 0.93f, 0.89f, 1.00f);
				colors[ImGuiCol_PlotHistogram] = ImVec4(0.86f, 0.93f, 0.89f, 1.00f);
				colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.86f, 0.93f, 0.89f, 1.00f);
				colors[ImGuiCol_TextSelectedBg] = ImVec4(0.24f, 0.24f, 0.24f, 1.00f);
				colors[ImGuiCol_DragDropTarget] = ImVec4(0.86f, 0.93f, 0.89f, 1.00f);
				colors[ImGuiCol_NavHighlight] = ImVec4(0.86f, 0.93f, 0.89f, 1.00f);
				colors[ImGuiCol_NavWindowingHighlight] = ImVec4(0.86f, 0.93f, 0.89f, 1.00f);
				colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
				colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);

				style.WindowBorderSize = 1;
				style.WindowRounding = 8;
				style.WindowPadding = ImVec2(0, 0);
				style.ItemInnerSpacing = ImVec2(0, 0);
				style.TouchExtraPadding = ImVec2(0.00f, 0.00f);
				style.IndentSpacing = 25;
				style.ScrollbarSize = 15;
				style.GrabMinSize = 10;
				style.TabRounding = 8;
				style.TabBorderSize = 1;
				style.PopupBorderSize = 1;
				style.FrameBorderSize = 1;
				style.ChildBorderSize = 1;
				style.ChildRounding = 8;
				style.FrameRounding = 8;
				style.PopupRounding = 8;
				style.ScrollbarRounding = 8;
				style.GrabRounding = 8;
				style.LogSliderDeadzone = 4;
				style.FramePadding = m_FramePadding;
				style.ItemSpacing = m_FramePadding;

				m_HeaderSize = ImVec2(0, FontSize() * 2);
			}

			ImFont*& ImGuiWindow::H1() const
			{
				return m_H1;
			}

			ImFont*& ImGuiWindow::Capital() const
			{
				return m_Capital;
			}

			ImFont*& ImGuiWindow::CapitalIcon() const
			{
				return m_CapitalIcon;
			}

			ImFont*& ImGuiWindow::Bold() const
			{
				return m_BoldFont;
			}

			ImFont*& ImGuiWindow::Italics() const
			{
				return m_ItalicsFont;
			}

			ImFont*& ImGuiWindow::BoldItalics() const
			{
				return m_BoldItalicsFont;
			}

			float ImGuiWindow::FontSize() const
			{
				return m_FontSize;
			}

			float ImGuiWindow::SmallFontSize() const
			{
				return m_SmallFontSize;
			}

			ImVec2 ImGuiWindow::GetFramePadding() const
			{
				return m_FramePadding;
			}

			ImVec2 ImGuiWindow::GetWindowPadding() const
			{
				return m_WindowPadding;
			}

			ImVec2 ImGuiWindow::HeaderSize() const
			{
				return m_HeaderSize;
			}

            ExplorerWindow& ImGuiWindow::GetExplorerWindow()
            {
				return explorerWindow;
            }

			HierarchyWindow& ImGuiWindow::GetHierarchyWindow()
			{
				return hierarchyWindow;
			}

			void ImGuiWindow::UpdateMouseCursor()
			{
				if (ImGui::IsAnyItemHovered() || ImGui::IsWindowHovered(ImGuiHoveredFlags_AllowWhenBlockedByPopup))
				{
					// Set the cursor to a hand pointer
					ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
				}
				else
				{
					// Restore the default arrow cursor when not hovering over any item
					ImGui::SetMouseCursor(ImGuiMouseCursor_Arrow);
				}

				ImGuiIO& io = ImGui::GetIO();
				ImGuiMouseCursor imgui_cursor = ImGui::GetMouseCursor();
				if (io.MouseDrawCursor || imgui_cursor == ImGuiMouseCursor_None)
				{
					::SetCursor(NULL);
				}
				else
				{
					// Map ImGui cursor types to Win32 system cursors
					LPTSTR win32_cursor = IDC_ARROW; // Default arrow

					switch (imgui_cursor)
					{
						case ImGuiMouseCursor_TextInput:    win32_cursor = IDC_IBEAM; break;
						case ImGuiMouseCursor_ResizeAll:    win32_cursor = IDC_SIZEALL; break;
						case ImGuiMouseCursor_ResizeNS:     win32_cursor = IDC_SIZENS; break;
						case ImGuiMouseCursor_ResizeEW:     win32_cursor = IDC_SIZEWE; break;
						case ImGuiMouseCursor_ResizeNESW:   win32_cursor = IDC_SIZENESW; break;
						case ImGuiMouseCursor_ResizeNWSE:   win32_cursor = IDC_SIZENWSE; break;
						case ImGuiMouseCursor_Hand:         win32_cursor = IDC_HAND; break;
						case ImGuiMouseCursor_NotAllowed:   win32_cursor = IDC_NO; break;
						default:                            win32_cursor = IDC_ARROW; break;
					}

					// Set the system cursor using Win32 API
					::SetCursor(LoadCursor(NULL, win32_cursor));
				}
			}

			void ImGuiWindow::Render()
			{
				if (!m_Ready)
				{
					return;
				}

				ImGui_ImplDX12_NewFrame();
				ImGui_ImplWin32_NewFrame();
				ImGui::NewFrame();

				const ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

				ImGui::SetNextWindowPos(ImVec2(0, 0));

				ImGui::PushFont(m_IconFont);

				glm::vec2 size = core::ENGINE.GetWindow().GetRealSize();
				mainWindow.SetSize(ImVec2(size.x, size.y));

				ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
				mainWindow.Update();
				ImGui::PopStyleVar();

				hierarchyWindow.Update();
				sceneWindow.Update();
				inspectorWindow.Update();
				consoleWindow.Update();
				explorerWindow.Update();

				UpdateMouseCursor();

				ImGui::PopFont();

				ImGui::EndFrame();

				ImGui::Render();
			}
		}
	}
}

#endif // __EDITOR__