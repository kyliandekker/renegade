#include "core/Engine.h"

// HINSTANCE, HANDLE, DWORD
#include <windows.h>
// freopen_s
#include <stdio.h>
// thread
#include <thread>
// va_list, va_start, va_end
#include <cstdarg>

#include "logger/Logger.h"

using SecondsDuration = std::chrono::duration<float, std::ratio<1>>;

namespace renegade
{
	namespace core
	{
		bool Engine::Initialize(int a_NumArgs, ...)
		{
			// Initialize logger.
			// TODO: Logger is still a global var while I'd prefer it to be a local var of Engine.
			logger::LOGGER.Initialize();
			// Wait until the system is ready.
			while (!logger::LOGGER.Ready())
			{
				// Wait...
				std::this_thread::yield();
			}

			m_FileLoader.Initialize();
			// Wait until the system is ready.
			while (!m_FileLoader.Ready())
			{
				// Wait...
				std::this_thread::yield();
			}

			va_list list;
			va_start(list, a_NumArgs);

			HINSTANCE& hInst = va_arg(list, HINSTANCE);
			uint32_t& width = va_arg(list, uint32_t);
			uint32_t& height = va_arg(list, uint32_t);
			LPCWSTR& name = va_arg(list, LPCWSTR);

			va_end(list);

#ifdef __EDITOR__
			m_Editor.InitializeEditorSettings();
			width = m_Editor.GetEditorSettings().Size().x;
			height = m_Editor.GetEditorSettings().Size().y;
			// Initialize editor.
			m_Editor.SetCallbacks(m_Window);
#endif // __EDITOR__

			// Initialize the window.
			m_Window.Initialize(4, hInst, width, height, name);
			// Wait until the system is ready.
			while (!m_Window.Ready())
			{
				// Wait...
				std::this_thread::yield();
			}

			// Initialize the ECS.
			m_ECS.Initialize();
			// Wait until the system is ready.
			while (!m_Window.Ready())
			{
				// Wait...
				std::this_thread::yield();
			}

			LOG(LOGSEVERITY_SUCCESS, "Engine initialized.");

			// TODO: Start the audio thread.
			// TODO: Initialize other systems.
			
			while (true /* m_Window.Ready() */)
			{
				m_ECS.Update(m_DeltaTime);
			}

			return System::Initialize();
		}

		bool Engine::Destroy()
		{
#ifdef __EDITOR__
			// Destroy the editor.
			m_Editor.Destroy();
#endif // __EDITOR__

			// Destroy the window first.
			m_Window.Destroy();

			// Destroy the logger last.
			logger::LOGGER.Destroy();

			m_Ready = false;

			LOG(LOGSEVERITY_SUCCESS, "Engine destroyed.");
			return System::Destroy();
		}

		graphics::Window& Engine::GetWindow()
		{
			return m_Window;
		}

		gameplay::EntityComponentSystem& Engine::GetECS()
		{
			return m_ECS;
		}

		file::FileLoader& Engine::GetFileLoader()
		{
			return m_FileLoader;
		}

#ifdef __EDITOR__
		editor::Editor& Engine::GetEditor()
		{
			return m_Editor;
		}
#endif // __EDITOR__
	}
}