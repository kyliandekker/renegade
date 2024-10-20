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
#ifdef _DEBUG
		float FPSCounter::GetFPS()
		{
			float fps = m_Frames / m_TimeAccumulation;
			constexpr float STEPINTERVAL = 4.0f; // In seconds
			constexpr float INTERVAL = 1.0f / STEPINTERVAL;
			if ((m_TimeAccumulation * INTERVAL) > 1.0f)
			{
				m_Frames = 0, m_TimeAccumulation = 0;
			}
			return fps;
		}

		void FPSCounter::AddFrame()
		{
			m_TimeAccumulation += core::ENGINE.GetDeltaTime();
			m_Frames++;
		}
#endif // _DEBUG

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
#endif // _DEBUG

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

			// Initialize the ECS.
			InitializeSystem(&m_ECS);

			LOGF(LOGSEVERITY_SUCCESS, "Engine initialized.");

			// TODO: Start the audio thread.
			// TODO: Initialize other systems.
			
			UpdateDeltaTime();
			while (true)
			{
				UpdateDeltaTime();

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

#ifdef _DEBUG
			fclose(console);
#endif
			m_Ready = false;

			LOGF(LOGSEVERITY_SUCCESS, "Engine destroyed.");
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

#ifdef __EDITOR__
		editor::Editor& Engine::GetEditor()
		{
			return m_Editor;
		}

		float Engine::GetFPS()
		{
			return m_FpsCounter.GetFPS();
		}

		float Engine::GetDeltaTime() const
		{
			return m_DeltaTime;
		}

		void Engine::UpdateDeltaTime()
		{
			std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
			SecondsDuration elapsed = std::chrono::duration_cast<SecondsDuration>(now - m_LastFrame);

			m_LastFrame = now;
			m_DeltaTime = elapsed.count();
			m_FpsCounter.AddFrame();
		}
#endif 
	}
}