#include "graphics/Window.h"

// va_list, va_start, va_end
#include <cstdarg>
// uint32_t
#include <cstdint>

#include "logger/Logger.h"
#include "core/Engine.h"

namespace renegade
{
	namespace graphics
	{
		LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
		{
			return core::ENGINE.GetWindow().WndProcHandler(hwnd, msg, wParam, lParam);
		}
		
		bool Window::IsRunning() const
		{
			return m_Running;
		}

		bool Window::StartThread(HINSTANCE a_hInstance, uint32_t a_Width, uint32_t a_Height, LPCWSTR a_WindowName)
		{
			CreateWindow(a_hInstance, a_Width, a_Height, a_WindowName);

			// Initialize DX12.
			if (!m_DX12Window.Initialize(1, m_hWnd))
			{
				LOGF(LOGSEVERITY_ASSERT, "Creating DX12 Window failed.");
				m_DX12Window.CleanupDeviceD3D();
				return false;
			}

			Show();

			// Call event.
			m_OnWindowCreated();

			PollEvents();

			return true;
		}

		bool Window::Initialize(int a_NumArgs, ...)
		{
			va_list list;
			va_start(list, a_NumArgs);

			HINSTANCE& hInst = va_arg(list, HINSTANCE);
			uint32_t& width = va_arg(list, uint32_t);
			uint32_t& height = va_arg(list, uint32_t);
			LPCWSTR& name = va_arg(list, LPCWSTR);

			va_end(list);

			m_Thread = std::thread(&Window::StartThread, this, hInst, width, height, name);

			return true;
		}

		bool Window::Destroy()
		{
			m_Thread.join();

			m_Ready = false;

			m_DX12Window.Destroy();

			::DestroyWindow(m_hWnd);
			::UnregisterClassW(m_Wc.lpszClassName, m_Wc.hInstance);

			LOGF(LOGSEVERITY_SUCCESS, "Destroyed window.");
			return true;
		}

		LRESULT Window::WndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
		{
			m_OnMsg(hwnd, msg, wParam, lParam);
			switch (msg)
			{
				case WM_SETFOCUS:
				case WM_EXITSIZEMOVE:
				{
					m_Ready = false;

					glm::vec2 size(GetRealSize().x, GetRealSize().y);

					m_DX12Window.CleanupRenderTarget();
					DXGI_SWAP_CHAIN_DESC1 sd;
					m_DX12Window.g_pSwapChain->GetDesc1(&sd);
					sd.Width = static_cast<UINT>(GetRealSize().x);
					sd.Height = static_cast<UINT>(GetRealSize().y);

					m_DX12Window.g_pSwapChain->ResizeBuffers(sd.BufferCount, sd.Width, sd.Height, sd.Format, sd.Flags);
					m_DX12Window.CreateRenderTarget();

					m_Ready = true;
					break;
				}
				default:
				{
					return ::DefWindowProcW(hwnd, msg, wParam, lParam);
				}
			}
			return msg;
		}

		void Window::PollEvents()
		{
			MSG msg = {};
			m_Running = true;
			while (GetMessage(&msg, NULL, 0, 0) > 0)
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);

				if (!m_Ready)
				{
					return;
				}
				m_OnRender();
				m_DX12Window.Render();
			}
		}

		void Window::Show()
		{
			::ShowWindow(m_hWnd, 1);
			::UpdateWindow(m_hWnd);
		}

		void Window::Hide()
		{
			::ShowWindow(m_hWnd, 0);
		}

		void Window::ToggleFullscreen()
		{
			RECT windowRect;

			m_Fullscreen = !m_Fullscreen;
			if (m_Fullscreen)
			{
				::GetWindowRect(m_hWnd, &windowRect);

				UINT windowStyle = WS_OVERLAPPEDWINDOW & ~(WS_CAPTION | WS_SYSMENU |
					WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX);

				::SetWindowLongW(m_hWnd, GWL_STYLE, windowStyle);

				HMONITOR hMonitor = ::MonitorFromWindow(m_hWnd, MONITOR_DEFAULTTONEAREST);
				MONITORINFOEX monitorInfo = {};
				monitorInfo.cbSize = sizeof(MONITORINFOEX);
				::GetMonitorInfo(hMonitor, &monitorInfo);
				::SetWindowPos(m_hWnd, HWND_TOP, monitorInfo.rcMonitor.left,
					monitorInfo.rcMonitor.top, monitorInfo.rcMonitor.right - monitorInfo.rcMonitor.left,
					monitorInfo.rcMonitor.bottom - monitorInfo.rcMonitor.top,
					SWP_FRAMECHANGED | SWP_NOACTIVATE);

				::ShowWindow(m_hWnd, SW_MAXIMIZE);
			}
			else
			{
				// Restore all the window decorators.
				::SetWindowLong(m_hWnd, GWL_STYLE, WS_OVERLAPPEDWINDOW);

				::GetWindowRect(m_hWnd, &windowRect);

				::SetWindowPos(m_hWnd, HWND_NOTOPMOST, windowRect.left,
					windowRect.top, windowRect.right - windowRect.left,
					windowRect.bottom - windowRect.top, SWP_FRAMECHANGED | SWP_NOACTIVATE);

				::ShowWindow(m_hWnd, SW_NORMAL);
			}
		}

        void Window::SetTitle(const std::string& a_Title)
        {
			::SetWindowTextA(m_hWnd, a_Title.c_str());
        }

		glm::vec2 Window::GetRealSize() const
		{
			RECT rect;
			GetClientRect(m_hWnd, &rect);

			return glm::vec2(static_cast<float>(rect.right - rect.left), static_cast<float>(rect.bottom - rect.top));
		}

        HWND& Window::GetHWnd()
		{ 
			return m_hWnd;
		}

		WNDCLASSEX& Window::GetWc()
		{ 
			return m_Wc;
		}

		DX12Window& Window::GetDX12Window()
		{ 
			return m_DX12Window;
		}

		bool Window::CreateWindow(HINSTANCE a_hInstance, uint32_t a_Width, uint32_t a_Height, LPCWSTR a_WindowTitle)
		{
			glm::vec2 size(a_Width, a_Height);

			m_Wc = {};
			m_Wc.cbSize = sizeof(WNDCLASSEX);
			m_Wc.style = 0;
			m_Wc.lpfnWndProc = WndProc;
			m_Wc.cbClsExtra = 0;
			m_Wc.cbWndExtra = 0;
			m_Wc.hInstance = a_hInstance;
			m_Wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
			m_Wc.hCursor = LoadCursor(NULL, IDC_ARROW);
			m_Wc.hbrBackground = CreateSolidBrush(RGB(0, 0, 0));
			m_Wc.lpszMenuName = NULL;
			m_Wc.lpszClassName = L"Window";
			m_Wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
			// TODO: Icon.
			// 
			//m_Wc.hIcon = static_cast<HICON>(::LoadImage(hInst,
			//	MAKEINTRESOURCE(IDI_ICON2),
			//	IMAGE_ICON,
			//	16, 16,
			//	LR_DEFAULTCOLOR));
			// 
			//m_Wc.hIconSm = static_cast<HICON>(::LoadImage(hInst,
			//	MAKEINTRESOURCE(IDI_ICON2),
			//	IMAGE_ICON,
			//	16, 16,
			//	LR_DEFAULTCOLOR));

			if (!RegisterClassEx(&m_Wc))
			{
				LOGF(LOGSEVERITY_ASSERT, "Window registration failed.");
				return false;
			}

			m_hWnd = CreateWindowEx(
				WS_EX_CLIENTEDGE,
				L"Window",
				a_WindowTitle,
				WS_MINIMIZEBOX | WS_SYSMENU | WS_SIZEBOX,
				CW_USEDEFAULT, CW_USEDEFAULT, static_cast<int>(size.x), static_cast<int>(size.y),
				NULL, NULL, a_hInstance, this);

			if (!m_hWnd)
			{
				LOGF(LOGSEVERITY_ASSERT, "Failed to create window.");
				return false;
			}

			LOGF(LOGSEVERITY_SUCCESS, "Window has been created.");
			return System::Initialize();
		}
	}
}