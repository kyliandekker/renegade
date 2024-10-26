#pragma once

#ifdef __EDITOR__

#include "core/System.h"

#include "editor/imgui/ImGuiWindow.h"
#include "editor/EditorSettings.h"
#include "editor/AssetDatabase.h"

#define SETTINGS_FOLDER "/renegade/"

struct ID3D12GraphicsCommandList;

namespace renegade
{
	namespace graphics
	{
		class Window;
	}
	namespace editor
	{
		class SceneExplorerResource;

		namespace imgui
		{
			class EditorSelectable;
		}

		class Editor : public core::System
		{
		public:
			void SetCallbacks(graphics::Window& a_Window);
			bool Initialize(int a_NumArgs, ...) override;
			bool Destroy() override;

			LRESULT CALLBACK WndProcHandler(HWND a_hWnd, UINT a_Msg, WPARAM a_wParam, LPARAM a_lParam);
			void OnRenderTargetCreated();
			void OnRenderTargetCleaned();
			void OnRender();
			void OnDrawRenderData(ID3D12GraphicsCommandList* a_CommandList);

			imgui::ImGuiWindow& GetImGuiWindow();
			EditorSettings& GetEditorSettings();
			AssetDatabase& GetAssetDatabase();

			SceneExplorerResource* GetCurrentScene() const;
			void SetCurrentScene(SceneExplorerResource* a_Scene);

			void SetDirty();
			bool GetDirty();

			imgui::EditorSelectable* GetSelectable() const;
			virtual void SetSelectable(imgui::EditorSelectable* a_Selectable);
		private:
			imgui::EditorSelectable* m_EditorSelectable;

			imgui::ImGuiWindow m_Window;
			EditorSettings m_EditorSettings;
			AssetDatabase m_AssetDatabase;

			SceneExplorerResource* m_CurrentScene = nullptr;
		};
	}
}

#endif // __EDITOR__