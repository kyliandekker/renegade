#ifdef __EDITOR__

#include "editor/Editor.h"

#include "core/Engine.h"
#include "editor/imgui/EditorSelectable.h"
#include "utils/string_extensions.h"
#include "editor/explorer_resources/SceneExplorerResource.h"

namespace renegade
{
	namespace editor
	{
        void Editor::SetCallbacks(graphics::Window& a_Window)
        {
			a_Window.m_OnMsg += std::bind(&editor::Editor::WndProcHandler, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
			a_Window.m_OnRender += std::bind(&editor::Editor::OnRender, this);
			a_Window.GetDX12Window().m_OnRenderTargetCleaned += std::bind(&editor::Editor::OnRenderTargetCleaned, this);
			a_Window.GetDX12Window().m_OnRenderTargetCreated += std::bind(&editor::Editor::OnRenderTargetCreated, this);
			a_Window.GetDX12Window().m_OnDrawRenderData += std::bind(&editor::Editor::OnDrawRenderData, this, std::placeholders::_1);
			a_Window.m_OnWindowCreated += [this]()
				{
					InitializeSystem(this);
				};
        }

        bool Editor::Initialize(int, ...)
		{
			m_EditorSettings.Load();
			bool success = InitializeSystem(&m_AssetDatabase);
			success |= InitializeSystem(&m_Window);
			return success && System::Initialize();
		}

		bool Editor::Destroy()
		{
			bool success = m_Window.Destroy() && m_AssetDatabase.Destroy();
			LOGF(LOGSEVERITY_SUCCESS, "Editor destroyed.");
			return success && System::Destroy();
		}

        LRESULT Editor::WndProcHandler(HWND a_hWnd, UINT a_Msg, WPARAM a_wParam, LPARAM a_lParam)
        {
            return m_Window.WndProcHandler(a_hWnd, a_Msg, a_wParam, a_lParam);
        }

		void Editor::OnRenderTargetCreated()
		{
			m_Window.OnRenderTargetCreated();
		}

		void Editor::OnRenderTargetCleaned()
		{
			m_Window.OnRenderTargetCleaned();
		}

		void Editor::OnRender()
		{
			m_AssetDatabase.CheckAssetDatabase();
			m_Window.Render();
		}

		void Editor::OnDrawRenderData(ID3D12GraphicsCommandList* a_CommandList)
		{
			m_Window.OnDrawRenderData(a_CommandList);
		}

        imgui::ImGuiWindow& Editor::GetImGuiWindow()
        {
			return m_Window;
        }

        EditorSettings& Editor::GetEditorSettings()
        {
			return m_EditorSettings;
        }

		AssetDatabase& Editor::GetAssetDatabase()
		{
			return m_AssetDatabase;
		}

        SceneExplorerResource* Editor::GetCurrentScene() const
        {
            return m_CurrentScene;
        }

		void Editor::SetCurrentScene(SceneExplorerResource* a_Scene)
		{
			m_CurrentScene = a_Scene;
			m_CurrentScene->Load();
		}

        void Editor::SetDirty()
        {
			if (m_CurrentScene)
			{
				m_CurrentScene->SetDirty();
			}
        }

		bool Editor::GetDirty()
		{
			return m_CurrentScene ? m_CurrentScene->IsDirty() : false;
		}

        imgui::EditorSelectable* Editor::GetSelectable() const
        {
            return m_EditorSelectable;
        }

		void Editor::SetSelectable(imgui::EditorSelectable* a_Selectable)
		{
			if (m_EditorSelectable)
			{
				m_EditorSelectable->Deselect();
			}
			m_EditorSelectable = a_Selectable;
			if (m_EditorSelectable)
			{
				m_EditorSelectable->Select();
			}
		}

        core::Data& Editor::GetClipboard()
        {
			return m_Clipboard;
        }

		void Editor::SetClipboard(core::Data& a_Data)
		{
			m_Clipboard = a_Data;
		}
	}
}

#endif // __EDITOR__