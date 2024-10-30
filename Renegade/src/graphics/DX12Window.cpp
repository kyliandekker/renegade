#include "graphics/DX12Window.h"

#include <Windows.h>
#include <string>
#include <glm/vec4.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "logger/Logger.h"
#include "core/datatypes/Data.h"

namespace renegade
{
	namespace graphics
	{
		DX12Window::DX12Window()
		{
		}

		bool DX12Window::Initialize(int nArgs, ...)
		{
			va_list list;
			va_start(list, nArgs);

			HWND& hwnd = va_arg(list, HWND);
			uint32_t& width = va_arg(list, uint32_t);
			uint32_t& height = va_arg(list, uint32_t);

			va_end(list);

			EnableDebugLayer();
			g_TearingSupported = CheckTearingSupport();

			if (!GetAdapter(g_UseWarp))
			{
				return false;
			}

			if (!CreateDevice(g_DxgiAdapter4))
			{
				return false;
			}

			if (!CreateCommandQueue(g_Device, D3D12_COMMAND_LIST_TYPE_DIRECT))
			{
				return false;
			}

			if (!CreateSwapChain(hwnd, g_CommandQueue, width, height, g_NumFrames))
			{
				return false;
			}

			g_CurrentBackBufferIndex = g_SwapChain->GetCurrentBackBufferIndex();
		
			if (!CreateDescriptorHeap(g_Device, D3D12_DESCRIPTOR_HEAP_TYPE_RTV, D3D12_DESCRIPTOR_HEAP_FLAG_NONE, g_NumFrames, g_RTVDescriptorHeap))
			{
				return false;
			}
			g_RTVDescriptorSize = g_Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
			UpdateRenderTargetViews(g_Device, g_SwapChain, g_RTVDescriptorHeap, D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
			m_OnRenderTargetCreated();
			if (!CreateDescriptorHeap(g_Device, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE, g_NumFrames, g_SRVDescriptorHeap))
			{
				return false;
			}
			UpdateRenderTargetViews(g_Device, g_SwapChain, g_SRVDescriptorHeap, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

			for (int i = 0; i < g_NumFrames; ++i)
			{
				g_CommandAllocators[i] = CreateCommandAllocator(g_Device, D3D12_COMMAND_LIST_TYPE_DIRECT);
			}
			g_CommandList = CreateCommandList(g_Device,
				g_CommandAllocators[g_CurrentBackBufferIndex], D3D12_COMMAND_LIST_TYPE_DIRECT);

			if (!CreateFence(g_Device))
			{
				return false;
			}
			if (!CreateEventHandle())
			{
				return false;
			}

			return System::Initialize();
		}

		bool DX12Window::Destroy()
		{
			// Make sure the command queue has finished all commands before closing.
			Flush(g_CommandQueue, g_Fence, g_FenceValue, g_FenceEvent);
			CleanUpRenderTargets(g_SwapChain);
			return false;
		}

		void DX12Window::EnableDebugLayer()
		{
#if defined(_DEBUG)
			// Always enable the debug layer before doing anything DX12 related
			// so all possible errors generated while creating DX12 objects
			// are caught by the debug layer.
			Microsoft::WRL::ComPtr<ID3D12Debug> debugInterface;
			if (FAILED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugInterface))))
			{
				LOGF(LOGSEVERITY_ERROR, "Could not retrieve debug interface.");
				return;
			}
			debugInterface->EnableDebugLayer();
#endif
		}

		bool DX12Window::CheckTearingSupport()
		{
			BOOL allowTearing = FALSE;

			// Rather than create the DXGI 1.5 factory interface directly, we create the
			// DXGI 1.4 interface and query for the 1.5 interface. This is to enable the 
			// graphics debugging tools which will not support the 1.5 factory interface 
			// until a future update.
			Microsoft::WRL::ComPtr<IDXGIFactory4> factory4;
			if (SUCCEEDED(CreateDXGIFactory1(IID_PPV_ARGS(&factory4))))
			{
				Microsoft::WRL::ComPtr<IDXGIFactory5> factory5;
				if (SUCCEEDED(factory4.As(&factory5)))
				{
					if (FAILED(factory5->CheckFeatureSupport(
						DXGI_FEATURE_PRESENT_ALLOW_TEARING,
						&allowTearing, sizeof(allowTearing))))
					{
						allowTearing = FALSE;
					}
				}
			}

			return allowTearing == TRUE;
		}

		bool DX12Window::GetAdapter(bool a_UseWarp)
		{
			Microsoft::WRL::ComPtr<IDXGIFactory4> dxgiFactory;
			UINT createFactoryFlags = 0;
#if defined(_DEBUG)
			createFactoryFlags = DXGI_CREATE_FACTORY_DEBUG;
#endif
			if (FAILED(CreateDXGIFactory2(createFactoryFlags, IID_PPV_ARGS(&dxgiFactory))))
			{
				LOGF(LOGSEVERITY_ERROR, "Failed creating DXGIFactory2.");
				return false;
			}

			Microsoft::WRL::ComPtr<IDXGIAdapter1> dxgiAdapter1;

			if (a_UseWarp)
			{
				if (FAILED(dxgiFactory->EnumWarpAdapter(IID_PPV_ARGS(&dxgiAdapter1))))
				{
					LOGF(LOGSEVERITY_ERROR, "Failed creating WARP adapter.");
					return false;
				}
				if (FAILED(dxgiAdapter1.As(&g_DxgiAdapter4)))
				{
					LOGF(LOGSEVERITY_ERROR, "Failed casting WARP adapter.");
					return false;
				}
			}
			else
			{
				SIZE_T maxDedicatedVideoMemory = 0;
				for (UINT i = 0; dxgiFactory->EnumAdapters1(i, &dxgiAdapter1) != DXGI_ERROR_NOT_FOUND; ++i)
				{
					DXGI_ADAPTER_DESC1 dxgiAdapterDesc1;
					dxgiAdapter1->GetDesc1(&dxgiAdapterDesc1);

					// Check to see if the adapter can create a D3D12 device without actually 
					// creating it. The adapter with the largest dedicated video memory
					// is favored.
					if ((dxgiAdapterDesc1.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) == 0 &&
						SUCCEEDED(D3D12CreateDevice(dxgiAdapter1.Get(),
							D3D_FEATURE_LEVEL_11_0, __uuidof(ID3D12Device), nullptr)) &&
						dxgiAdapterDesc1.DedicatedVideoMemory > maxDedicatedVideoMemory)
					{
						maxDedicatedVideoMemory = dxgiAdapterDesc1.DedicatedVideoMemory;
						if (FAILED(dxgiAdapter1.As(&g_DxgiAdapter4)))
						{
							LOGF(LOGSEVERITY_ERROR, "Failed creating adapter.");
							return false;
						}
					}
				}
			}

			LOGF(LOGSEVERITY_SUCCESS, "Created adapter.");
			return true;
		}

		bool DX12Window::CreateDevice(Microsoft::WRL::ComPtr<IDXGIAdapter4> a_Adapter)
		{
			if (FAILED(D3D12CreateDevice(a_Adapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&g_Device))))
			{
				LOGF(LOGSEVERITY_ERROR, "Failed creating D3D12 device.");
				return false;
			}

			// Enable debug messages in debug mode.
#if defined(_DEBUG)
			Microsoft::WRL::ComPtr<ID3D12InfoQueue> pInfoQueue;
			if (SUCCEEDED(g_Device.As(&pInfoQueue)))
			{
				pInfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, TRUE);
				pInfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, TRUE);
				pInfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_WARNING, TRUE);

				// Suppress whole categories of messages
				//D3D12_MESSAGE_CATEGORY Categories[] = {};

				// Suppress messages based on their severity level
				D3D12_MESSAGE_SEVERITY Severities[] =
				{
					D3D12_MESSAGE_SEVERITY_INFO
				};

				// Suppress individual messages by their ID
				D3D12_MESSAGE_ID DenyIds[] = {
					D3D12_MESSAGE_ID_CLEARRENDERTARGETVIEW_MISMATCHINGCLEARVALUE,   // I'm really not sure how to avoid this message.
					D3D12_MESSAGE_ID_MAP_INVALID_NULLRANGE,                         // This warning occurs when using capture frame while graphics debugging.
					D3D12_MESSAGE_ID_UNMAP_INVALID_NULLRANGE,                       // This warning occurs when using capture frame while graphics debugging.
				};

				D3D12_INFO_QUEUE_FILTER NewFilter = {};
				//NewFilter.DenyList.NumCategories = _countof(Categories);
				//NewFilter.DenyList.pCategoryList = Categories;
				NewFilter.DenyList.NumSeverities = _countof(Severities);
				NewFilter.DenyList.pSeverityList = Severities;
				NewFilter.DenyList.NumIDs = _countof(DenyIds);
				NewFilter.DenyList.pIDList = DenyIds;

				if (FAILED(pInfoQueue->PushStorageFilter(&NewFilter)))
				{
					LOGF(LOGSEVERITY_ERROR, "Could not push storage filter to info queue on D3D12 device.");
					return false;
				}
			}
#endif
			LOGF(LOGSEVERITY_SUCCESS, "Created D3D12 device.");
			return true;
		}

		bool DX12Window::CreateCommandQueue(Microsoft::WRL::ComPtr<ID3D12Device2> a_Device, D3D12_COMMAND_LIST_TYPE a_Type)
		{
			D3D12_COMMAND_QUEUE_DESC desc = {};
			desc.Type = a_Type;
			desc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
			desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
			desc.NodeMask = 0;

			if (FAILED(a_Device->CreateCommandQueue(&desc, IID_PPV_ARGS(&g_CommandQueue))))
			{
				LOGF(LOGSEVERITY_ERROR, "Failed creating command queue.");
				return false;
			}

			LOGF(LOGSEVERITY_SUCCESS, "Created command queue.");
			return true;
		}

		bool DX12Window::CreateSwapChain(HWND hWnd, Microsoft::WRL::ComPtr<ID3D12CommandQueue> a_CommandQueue, uint32_t a_Width, uint32_t a_Height, uint32_t a_BufferCount)
		{
			Microsoft::WRL::ComPtr<IDXGIFactory4> dxgiFactory4;
			UINT createFactoryFlags = 0;
#if defined(_DEBUG)
			createFactoryFlags = DXGI_CREATE_FACTORY_DEBUG;
#endif
			if (FAILED(CreateDXGIFactory2(createFactoryFlags, IID_PPV_ARGS(&dxgiFactory4))))
			{
				LOGF(LOGSEVERITY_ERROR, "Failed creating swap chain.");
				return false;
			}

			DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
			swapChainDesc.Width = a_Width;
			swapChainDesc.Height = a_Height;
			swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			swapChainDesc.Stereo = FALSE;
			swapChainDesc.SampleDesc = { 1, 0 };
			swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			swapChainDesc.BufferCount = a_BufferCount;
			swapChainDesc.Scaling = DXGI_SCALING_STRETCH;
			swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
			swapChainDesc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
			// It is recommended to always allow tearing if tearing support is available.
			swapChainDesc.Flags = CheckTearingSupport() ? DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING : 0;

			Microsoft::WRL::ComPtr<IDXGISwapChain1> swapChain1;
			if (FAILED(dxgiFactory4->CreateSwapChainForHwnd(
				a_CommandQueue.Get(),
				hWnd,
				&swapChainDesc,
				nullptr,
				nullptr,
				&swapChain1)))
			{
				LOGF(LOGSEVERITY_ERROR, "Failed creating swap chain for hwnd.");
				return false;
			}

			// Disable the Alt+Enter fullscreen toggle feature. Switching to fullscreen
			// will be handled manually.
			if (FAILED(dxgiFactory4->MakeWindowAssociation(hWnd, DXGI_MWA_NO_ALT_ENTER)))
			{
				LOGF(LOGSEVERITY_ERROR, "Failed associating hwnd with swapchain.");
				return false;
			}

			if ((swapChain1.As(&g_SwapChain)))
			{
				LOGF(LOGSEVERITY_ERROR, "Failed casting to swapchain.");
				return false;
			}

			LOGF(LOGSEVERITY_SUCCESS, "Created swap chain.");
			return true;
		}

		bool DX12Window::CreateDescriptorHeap(Microsoft::WRL::ComPtr<ID3D12Device2> a_Device, D3D12_DESCRIPTOR_HEAP_TYPE a_Type, D3D12_DESCRIPTOR_HEAP_FLAGS a_Flags, uint32_t a_NumDescriptors, Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>& a_DescriptorHeap)
		{
			D3D12_DESCRIPTOR_HEAP_DESC desc = {};
			desc.NumDescriptors = a_NumDescriptors;
			desc.Type = a_Type;
			desc.Flags = a_Flags;

			if (FAILED(a_Device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&a_DescriptorHeap))))
			{
				LOGF(LOGSEVERITY_ERROR, "Failed creating descriptor heap.");
				return false;
			}

			LOGF(LOGSEVERITY_SUCCESS, "Created descriptor heap.");
			return true;
		}

		void DX12Window::UpdateRenderTargetViews(Microsoft::WRL::ComPtr<ID3D12Device2> a_Device, Microsoft::WRL::ComPtr<IDXGISwapChain4> a_SwapChain, Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> a_DescriptorHeap, D3D12_DESCRIPTOR_HEAP_TYPE a_Type)
		{
			auto rtvDescriptorSize = a_Device->GetDescriptorHandleIncrementSize(a_Type);

			CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(a_DescriptorHeap->GetCPUDescriptorHandleForHeapStart());

			for (int i = 0; i < g_NumFrames; ++i)
			{
				if (a_Type == D3D12_DESCRIPTOR_HEAP_TYPE_RTV)
				{
					ID3D12Resource* backBuffer = nullptr;
					if (FAILED(a_SwapChain->GetBuffer(i, IID_PPV_ARGS(&backBuffer))))
					{
						LOGF(LOGSEVERITY_ERROR, "Failed retrieving buffer from swap chain.");
						return;
					}

					a_Device->CreateRenderTargetView(backBuffer, nullptr, rtvHandle);

					g_BackBuffers[i] = backBuffer;
				}

				rtvHandle.Offset(rtvDescriptorSize);
			}
		}

		Microsoft::WRL::ComPtr<ID3D12CommandAllocator> DX12Window::CreateCommandAllocator(Microsoft::WRL::ComPtr<ID3D12Device2> a_Device, D3D12_COMMAND_LIST_TYPE a_Type)
		{
			Microsoft::WRL::ComPtr<ID3D12CommandAllocator> commandAllocator;
			if (FAILED(a_Device->CreateCommandAllocator(a_Type, IID_PPV_ARGS(&commandAllocator))))
			{
				LOGF(LOGSEVERITY_ERROR, "Failed creating command allocator.");
				return nullptr;
			}

			return commandAllocator;
		}

		Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> DX12Window::CreateCommandList(Microsoft::WRL::ComPtr<ID3D12Device2> a_Device, Microsoft::WRL::ComPtr<ID3D12CommandAllocator> a_CommandAllocator, D3D12_COMMAND_LIST_TYPE a_Type)
		{
			Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandList;
			if (FAILED(a_Device->CreateCommandList(0, a_Type, a_CommandAllocator.Get(), nullptr, IID_PPV_ARGS(&commandList))))
			{
				LOGF(LOGSEVERITY_ERROR, "Failed creating command list.");
				return nullptr;
			}

			if (FAILED(commandList->Close()))
			{
				LOGF(LOGSEVERITY_ERROR, "Failed closing command list.");
				return nullptr;
			}

			return commandList;
		}
		
		bool DX12Window::CreateFence(Microsoft::WRL::ComPtr<ID3D12Device2> a_Device)
		{
			if (FAILED(a_Device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&g_Fence))))
			{
				LOGF(LOGSEVERITY_ERROR, "Failed creating fence.");
				return false;
			}

			LOGF(LOGSEVERITY_SUCCESS, "Created fence.");
			return true;
		}

		bool DX12Window::CreateEventHandle()
		{
			g_FenceEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);
			if (!g_FenceEvent)
			{
				LOGF(LOGSEVERITY_ERROR, "Failed creating fence event.");
				return false;
			}

			LOGF(LOGSEVERITY_SUCCESS, "Created fence event.");
			return true;
		}

		void DX12Window::Resize(glm::vec2 a_Size)
		{
			m_NeedsResize = true;
			m_ScreenSize = a_Size;
		}

		Microsoft::WRL::ComPtr<ID3D12Device2> DX12Window::GetDevice() const
		{
			return g_Device;
		}

		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> DX12Window::GetSRVDescriptorHeap() const
		{
			return g_SRVDescriptorHeap;
		}

		FPSCounter DX12Window::GetFPS() const
		{
			return m_FpsCounter;
		}

		void DX12Window::CleanUpRenderTargets(Microsoft::WRL::ComPtr<IDXGISwapChain4> a_SwapChain)
		{
			m_OnRenderTargetCleaned();
			for (int i = 0; i < g_NumFrames; ++i)
			{
				if (g_BackBuffers[i])
				{
					g_BackBuffers[i]->Release();
				}
			}
		}

		void DX12Window::Flush(Microsoft::WRL::ComPtr<ID3D12CommandQueue> a_CommandQueue, Microsoft::WRL::ComPtr<ID3D12Fence> a_Fence, uint64_t& a_FenceValue, HANDLE a_FenceEvent)
		{
			uint64_t fenceValueForSignal = Signal(a_CommandQueue, a_Fence, a_FenceValue);
			WaitForFenceValue(a_Fence, fenceValueForSignal, a_FenceEvent);
		}

		uint64_t DX12Window::Signal(Microsoft::WRL::ComPtr<ID3D12CommandQueue> a_CommandQueue, Microsoft::WRL::ComPtr<ID3D12Fence> a_Fence, uint64_t& a_FenceValue)
		{
			uint64_t fenceValueForSignal = ++a_FenceValue;
			if (FAILED(a_CommandQueue->Signal(a_Fence.Get(), fenceValueForSignal)))
			{
				LOGF(LOGSEVERITY_SUCCESS, "Failed signalling fence.");
				return --a_FenceValue;
			}

			return fenceValueForSignal;
		}

		void DX12Window::WaitForFenceValue(Microsoft::WRL::ComPtr<ID3D12Fence> a_Fence, uint64_t a_FenceValue, HANDLE a_FenceEvent, std::chrono::milliseconds a_Duration)
		{
			if (a_Fence->GetCompletedValue() < a_FenceValue)
			{
				if (FAILED(a_Fence->SetEventOnCompletion(a_FenceValue, a_FenceEvent)))
				{
					LOGF(LOGSEVERITY_SUCCESS, "Failed setting event on completion.");
					return;
				}
				::WaitForSingleObject(a_FenceEvent, static_cast<DWORD>(a_Duration.count()));
			}
		}

		void DX12Window::Render()
		{
			m_FpsCounter.Update();
			auto commandAllocator = g_CommandAllocators[g_CurrentBackBufferIndex];
			auto backBuffer = g_BackBuffers[g_CurrentBackBufferIndex];

			commandAllocator->Reset();
			g_CommandList->Reset(commandAllocator.Get(), nullptr);

			// Clear the render target.
			{
				CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
					backBuffer,
					D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);

				g_CommandList->ResourceBarrier(1, &barrier);

				FLOAT clearColor[] = { 0.4f, 0.6f, 0.9f, 1.0f };
				CD3DX12_CPU_DESCRIPTOR_HANDLE rtv(g_RTVDescriptorHeap->GetCPUDescriptorHandleForHeapStart(),
					g_CurrentBackBufferIndex, g_RTVDescriptorSize);

				g_CommandList->ClearRenderTargetView(rtv, clearColor, 0, nullptr);
				g_CommandList->OMSetRenderTargets(1, &rtv, FALSE, nullptr);
				ID3D12DescriptorHeap* descriptorHeaps[] = { g_SRVDescriptorHeap.Get() };
				g_CommandList->SetDescriptorHeaps(_countof(descriptorHeaps), descriptorHeaps);
			}

			m_OnDrawRenderData(g_CommandList.Get());

			// Present
			{
				CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
					backBuffer,
					D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
				g_CommandList->ResourceBarrier(1, &barrier);

				if (FAILED((g_CommandList->Close())))
				{
					return;
				}

				ID3D12CommandList* const commandLists[] = {
					g_CommandList.Get()
				};
				g_CommandQueue->ExecuteCommandLists(_countof(commandLists), commandLists);

				g_FrameFenceValues[g_CurrentBackBufferIndex] = Signal(g_CommandQueue, g_Fence, g_FenceValue);

				UINT syncInterval = g_VSync ? 1 : 0;
				UINT presentFlags = g_TearingSupported && !g_VSync ? DXGI_PRESENT_ALLOW_TEARING : 0;
				if (FAILED((g_SwapChain->Present(syncInterval, presentFlags))))
				{
					return;
				}

				g_CurrentBackBufferIndex = g_SwapChain->GetCurrentBackBufferIndex();

				WaitForFenceValue(g_Fence, g_FrameFenceValues[g_CurrentBackBufferIndex], g_FenceEvent);
			}
		}

		void DX12Window::WaitForLastSubmittedFrame(Microsoft::WRL::ComPtr<ID3D12CommandQueue> a_CommandQueue, Microsoft::WRL::ComPtr<ID3D12Fence> a_Fence, uint64_t& a_FenceValue, HANDLE a_FenceEvent)
		{
			for (UINT i = 0; i < g_NumFrames; i++)
			{
				WaitForFenceValue(g_Fence, g_FrameFenceValues[i], g_FenceEvent);
			}
		}

		void FPSCounter::Update()
		{
			m_FrameCounter++;
			auto t1 = m_Clock.now();
			auto deltaTime = t1 - m_T0;
			m_T0 = t1;

			m_ElapsedSeconds += deltaTime.count() * 1e-9;
			if (m_ElapsedSeconds > 1.0)
			{
				m_FPS = m_FrameCounter / m_ElapsedSeconds;
				m_FrameCounter = 0;
				m_ElapsedSeconds = 0.0;
			}
		}

		double FPSCounter::GetFPS() const
		{
			return m_FPS;
		}
	}
}