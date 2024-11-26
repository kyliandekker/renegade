#pragma once

#ifdef __EDITOR__

#include "core/System.h"

#include <windows.h>
#include <string>
#include <imgui/imgui.h>

#include "graphics/Window.h"
#include "editor/imgui/windows/MainWindow.h"
#include "editor/imgui/windows/HierarchyWindow.h"
#include "editor/imgui/windows/SceneWindow.h"
#include "editor/imgui/windows/InspectorWindow.h"
#include "editor/imgui/windows/ConsoleWindow.h"
#include "editor/imgui/windows/ExplorerWindow.h"
#include "editor/imgui/windows/LoadProjectWindow.h"
#include "editor/imgui/windows/ProjectSettingsWindow.h"

struct ID3D12GraphicsCommandList;

namespace renegade
{
	namespace logger
	{
		struct Message;
	}

	namespace editor
	{
		namespace imgui
		{
			enum class EditorState
			{
				EditorState_LoadProject,
				EditorState_Editor,
			};

			class ImGuiWindow : public core::System
			{
			public:
				ImGuiWindow();
				~ImGuiWindow();

				bool Initialize(int a_NumArgs = 0, ...) override;
				bool Destroy() override;

				LRESULT CALLBACK WndProcHandler(HWND a_hWnd, UINT a_Msg, WPARAM a_wParam, LPARAM a_wlParam);
				void OnRenderTargetCreated();
				void OnRenderTargetCleaned();
				void OnDrawRenderData(ID3D12GraphicsCommandList* a_CommandList);

				bool CreateContextWin32();
				bool CreateContextDX12();
				void CreateImGui();
				void Render();

				ImFont*& Capital() const;
				ImFont*& CapitalIcon() const;
				ImFont*& H1() const;
				ImFont*& Bold() const;
				ImFont*& Italics() const;
				ImFont*& BoldItalics() const;

				float FontSize() const;
				float SmallFontSize() const;
				ImVec2 GetFramePadding() const;
				ImVec2 GetWindowPadding() const;
				ImVec2 HeaderSize() const;

				ExplorerWindow& GetExplorerWindow();
				HierarchyWindow& GetHierarchyWindow();
			private:
				void UpdateMouseCursor();
				void SetLoadProjectState();

				std::string m_IniPath;

				float m_FontSize = 17.5f;
				float m_SmallFontSize = 12.0f;

				ImVec2 m_FramePadding = ImVec2(8, 8);
				ImVec2 m_WindowPadding = ImVec2(8, 8);
				ImVec2 m_HeaderSize;

				EditorState m_EditorState = EditorState::EditorState_LoadProject;

				MainWindow mainWindow;
				ConsoleWindow consoleWindow;
				SceneWindow sceneWindow;
				InspectorWindow inspectorWindow;
				HierarchyWindow hierarchyWindow;
				ExplorerWindow explorerWindow;
				LoadProjectWindow loadProjectWindow;
				ProjectSettingsWindow projectSettingsWindow;
			};
		}
	}
}

#endif // __EDITOR__