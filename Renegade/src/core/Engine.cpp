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

namespace renegade
{
	namespace core
	{
		FILE* console = nullptr;
		bool Engine::Initialize(int a_NumArgs, ...)
		{
			// Initialize console first.
#ifdef _DEBUG
			AllocConsole();
			freopen_s(&console, "CONOUT$", "w", stdout);

			HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
			DWORD dwMode = 0;
			GetConsoleMode(hOut, &dwMode);
			dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
			SetConsoleMode(hOut, dwMode);
#endif

			// Initialize logger.
			InitializeSystem(&logger::LOGGER);

			va_list list;
			va_start(list, a_NumArgs);

			HINSTANCE& hInst = va_arg(list, HINSTANCE);
			uint32_t& width = va_arg(list, uint32_t);
			uint32_t& height = va_arg(list, uint32_t);
			LPCWSTR& name = va_arg(list, LPCWSTR);

			va_end(list);

#ifdef __EDITOR__
			// Initialize editor.
			m_Editor.SetCallbacks(m_Window);
#endif // __EDITOR__

			// Initialize the window.
			InitializeSystem(&m_Window, 4, hInst, width, height, name);

			LOGF(LOGSEVERITY_SUCCESS, "Engine initialized.");

			// TODO: Start the audio thread.
			// TODO: Initialize ECS
			// TODO: Initialize other systems.

			while (true)
			{ }

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

#ifdef _DEBUG
			fclose(console);
#endif
			m_Ready = false;

			return System::Destroy();
		}

		graphics::Window& Engine::GetWindow()
		{
			return m_Window;
		}
		
#ifdef __EDITOR__
		editor::Editor& Engine::GetEditor()
		{
			return m_Editor;
		}
#endif // __EDITOR__
	}
}