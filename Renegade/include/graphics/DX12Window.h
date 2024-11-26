#pragma once

#include "core/System.h"

#include <dx12/directx/d3dx12.h>
#include <dxgi1_6.h>
#include <string>
#include <wrl.h>
#include <cstdint>
#include <chrono>
#include <glm/vec2.hpp>
#include <memory>
#include <DirectXMath.h>

#undef max

#include "core/datatypes/Event.h"
#include "dx12/CommandQueue.h"

struct HWND__;
typedef HWND__* HWND;

namespace renegade
{
	namespace graphics
	{
		inline const uint8_t g_NumSwapChainBuffers = 3;
		inline const uint8_t g_NumSrvBuffers = 2;
		inline bool g_UseWarp = false;
		inline bool g_VSync = false;

		struct ImgInfo;

		class FPSCounter
		{
		public:
			double GetFPS() const;
		private:
			friend class DX12Window;
			void Update();
			void Initialize();

			double m_FPS = 0.0;
			uint64_t m_FrameCounter = 0;
			double m_ElapsedSeconds = 0.0;
			std::chrono::high_resolution_clock m_Clock;
			std::chrono::steady_clock::time_point m_T0 = m_Clock.now();
		};

		class DX12Window : public core::System
		{
		public:
			DX12Window();

			bool Initialize(int nArgs, ...) override;
			bool Destroy() override;

			SimpleEvent<> m_OnRenderTargetCreated;
			SimpleEvent<> m_OnRenderTargetCleaned;
			SimpleEvent<ID3D12GraphicsCommandList*> m_OnDrawRenderData;

			void Render();
			void Resize(glm::vec2 a_Size);
			void Reset();

			Microsoft::WRL::ComPtr<ID3D12Device2> GetDevice() const;
			Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> GetSRVDescriptorHeap() const;

			FPSCounter GetFPS() const;

			void Wait();
			void StopWaiting();

			Microsoft::WRL::ComPtr<ID3D12Resource> m_RenderTargetTexture;
			CD3DX12_CPU_DESCRIPTOR_HANDLE m_RenderTargetSrvHandleCPU;
			CD3DX12_GPU_DESCRIPTOR_HANDLE m_RenderTargetSrvHandleGPU;
		protected:
			glm::vec2 m_ScreenSize;

			void EnableDebugLayer();
			bool CheckTearingSupport();
			bool GetAdapter(bool a_UseWarp);
			bool CreateDevice(Microsoft::WRL::ComPtr<IDXGIAdapter4> a_Adapter);
			bool CreateSwapChain(HWND hWnd, Microsoft::WRL::ComPtr<ID3D12CommandQueue> a_CommandQueue, uint32_t a_Width, uint32_t a_Height, uint32_t a_BufferCount);
			bool CreateDescriptorHeap(Microsoft::WRL::ComPtr<ID3D12Device2> a_Device, D3D12_DESCRIPTOR_HEAP_TYPE a_Type, D3D12_DESCRIPTOR_HEAP_FLAGS a_Flags, uint32_t a_NumDescriptors, Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>& a_DescriptorHeap);
			void UpdateRenderTargetViews(Microsoft::WRL::ComPtr<ID3D12Device2> a_Device, Microsoft::WRL::ComPtr<IDXGISwapChain4> a_SwapChain, Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> a_DescriptorHeap, D3D12_DESCRIPTOR_HEAP_TYPE a_Type, uint32_t a_NumDescriptors);
			Microsoft::WRL::ComPtr<ID3D12CommandAllocator> CreateCommandAllocator(Microsoft::WRL::ComPtr<ID3D12Device2> a_Device, D3D12_COMMAND_LIST_TYPE a_Type);
			Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> CreateCommandList(Microsoft::WRL::ComPtr<ID3D12Device2> a_Device, Microsoft::WRL::ComPtr<ID3D12CommandAllocator> a_CommandAllocator, D3D12_COMMAND_LIST_TYPE a_Type);
			bool CreateFence(Microsoft::WRL::ComPtr<ID3D12Device2> a_Device);
			bool CreateEventHandle();

			void ClearRTV(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList2> a_CommandList, D3D12_CPU_DESCRIPTOR_HANDLE a_Rtv, FLOAT* a_ClearColor);
			void ClearDepth(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList2> a_CommandList, D3D12_CPU_DESCRIPTOR_HANDLE a_Dsv, FLOAT a_Depth);
			void TransitionResource(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList2> a_CommandList, Microsoft::WRL::ComPtr<ID3D12Resource> a_Resource, D3D12_RESOURCE_STATES a_BeforeState, D3D12_RESOURCE_STATES a_AfterState);
			
			void CleanUpRenderTargets(Microsoft::WRL::ComPtr<IDXGISwapChain4> a_SwapChain);
			void Flush();

			bool CreateOffscreenRenderTarget(uint32_t a_Width, uint32_t a_Height);

			FPSCounter m_FpsCounter;
			glm::vec2 m_Size;

			dx12::CommandQueue m_DirectCommandQueue;

			Microsoft::WRL::ComPtr<IDXGIAdapter4> g_DxgiAdapter4;
			Microsoft::WRL::ComPtr<ID3D12Device2> g_Device;
			Microsoft::WRL::ComPtr<IDXGISwapChain4> g_SwapChain;
			Microsoft::WRL::ComPtr<ID3D12Resource> g_BackBuffers[g_NumSwapChainBuffers];
			Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> g_RTVDescriptorHeap;
			Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> g_SRVDescriptorHeap;

			Microsoft::WRL::ComPtr<ID3D12Fence> g_Fence;
			bool g_TearingSupported = false;
			uint64_t g_FenceValue = 0;
			uint64_t g_FrameFenceValues[g_NumSwapChainBuffers] = {};
			HANDLE g_FenceEvent;
			UINT g_CurrentBackBufferIndex = 0;
			UINT g_RTVDescriptorSize = 0;
			UINT g_SRVDescriptorSize = 0;
			UINT m_ExtraRenderTargets = 1;
		};
	}
}