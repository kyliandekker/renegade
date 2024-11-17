#pragma once

#include <glm/vec2.hpp>
#include <Windows.h>
#include <thread>
#include <string>

#include "core/System.h"
#include "graphics/DX12Window.h"
#include "core/datatypes/Event.h"

#if defined(CreateWindow)
#undef CreateWindow
#endif

namespace renegade
{
	namespace graphics
	{
		class Window : public core::System
		{
		public:
			bool Initialize(int a_NumArgs = 0, ...) override;
			bool Destroy() override;

			LRESULT CALLBACK WndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
			void PollEvents();

			void Show();
			void Hide();
			void ToggleFullscreen();
			void SetTitle(const std::string& a_Title);

			glm::vec2 GetRealSize() const;

			HWND& GetHWnd();
			WNDCLASSEX& GetWc();
			DX12Window& GetDX12Window();

			SimpleEvent<> m_OnWindowCreated;
			SimpleEvent<HWND, UINT, WPARAM, LPARAM> m_OnMsg;
			SimpleEvent<> m_OnRender;

			std::thread m_Thread;
		private:
			std::string m_BaseWindowTitle;

			bool StartThread(HINSTANCE a_hInstance, uint32_t a_Width, uint32_t a_Height, LPCWSTR a_WindowName);

			DX12Window m_DX12Window;

			bool CreateWindow(HINSTANCE a_hInstance, uint32_t a_Width, uint32_t a_Height, LPCWSTR a_WindowTitle);

			HWND m_hWnd = HWND();
			WNDCLASSEX m_Wc = WNDCLASSEX();

			bool m_Fullscreen = false;
		};
	}
}