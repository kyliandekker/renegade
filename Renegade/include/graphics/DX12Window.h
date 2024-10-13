#pragma once

#include "core/System.h"

#include <d3d12.h>
#include <dxgi1_4.h>

#include "core/datatypes/Event.h"

struct HWND__;
typedef HWND__* HWND;

namespace renegade
{
	namespace graphics
	{
		struct ImgInfo;

		struct FrameContext
		{
			ID3D12CommandAllocator* CommandAllocator;
			unsigned __int64 FenceValue;
		};

		class DX12Window : public core::System
		{
		public:
			DX12Window();

			bool Initialize(int nArgs, ...) override;
			bool Destroy() override;

			void CreateRenderTarget();
			void Render();
			void WaitForLastSubmittedFrame();
			void CleanupRenderTarget();
			bool CreateDeviceD3D(HWND a_hWnd);
			void CleanupDeviceD3D();
			bool CleanupTextures();
			FrameContext* WaitForNextFrameResources();

			bool LoadTexture(const std::string& a_Path, ImgInfo& a_ImgInfo);

			SimpleEvent<> m_OnRenderTargetCreated;
			SimpleEvent<> m_OnRenderTargetCleaned;
			SimpleEvent<ID3D12GraphicsCommandList*> m_OnDrawRenderData;

			UINT g_frameIndex = 0;

			ID3D12Device* g_pd3dDevice = nullptr;
			ID3D12DescriptorHeap* g_pd3dRtvDescHeap = nullptr;
			ID3D12DescriptorHeap* g_pd3dSrvDescHeap = nullptr;
			ID3D12CommandQueue* g_pd3dCommandQueue = nullptr;
			ID3D12GraphicsCommandList* g_pd3dCommandList = nullptr;
			IDXGISwapChain3* g_pSwapChain = nullptr;
			ID3D12Fence* g_fence = nullptr;

			HANDLE g_fenceEvent = nullptr;
			unsigned __int64 g_fenceLastSignaledValue = 0;
			HANDLE g_hSwapChainWaitableObject = nullptr;

			UINT NUM_FRAMES_IN_FLIGHT = 3;
			UINT NUM_BACK_BUFFERS = 3;
			FrameContext g_frameContext[3] = {};
			ID3D12Resource* g_mainRenderTargetResource[3] = {};
			D3D12_CPU_DESCRIPTOR_HANDLE g_mainRenderTargetDescriptor[3] = {};

			int descriptor_index = 1;
		};
	}
}