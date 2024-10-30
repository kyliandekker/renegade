#pragma once

#include "core/System.h"

#include <dx12/directx/d3dx12.h>
#include <dxgi1_6.h>
#include <string>
#include <wrl.h>
#include <cstdint>
#include <chrono>

#undef max

#include "core/datatypes/Event.h"
#include <glm/vec2.hpp>

struct HWND__;
typedef HWND__* HWND;

namespace renegade
{
	namespace graphics
	{
		inline const uint8_t g_NumFrames = 3;
		inline bool g_UseWarp = false;
		inline bool g_VSync = true;

		struct ImgInfo;

		class FPSCounter
		{
		public:
			double GetFPS() const;
		private:
			friend class DX12Window;
			void Update();

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

			Microsoft::WRL::ComPtr<ID3D12Device2> GetDevice() const;
			Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> GetSRVDescriptorHeap() const;

			FPSCounter GetFPS() const;
		protected:
			glm::vec2 m_ScreenSize;
			bool m_NeedsResize = false;

			void EnableDebugLayer();
			bool CheckTearingSupport();
			bool GetAdapter(bool a_UseWarp);
			bool CreateDevice(Microsoft::WRL::ComPtr<IDXGIAdapter4> a_Adapter);
			bool CreateCommandQueue(Microsoft::WRL::ComPtr<ID3D12Device2> a_Device, D3D12_COMMAND_LIST_TYPE a_Type);
			bool CreateSwapChain(HWND hWnd, Microsoft::WRL::ComPtr<ID3D12CommandQueue> a_CommandQueue, uint32_t a_Width, uint32_t a_Height, uint32_t a_BufferCount);
			bool CreateDescriptorHeap(Microsoft::WRL::ComPtr<ID3D12Device2> a_Device, D3D12_DESCRIPTOR_HEAP_TYPE a_Type, D3D12_DESCRIPTOR_HEAP_FLAGS a_Flags, uint32_t a_NumDescriptors, Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>& a_DescriptorHeap);
			void UpdateRenderTargetViews(Microsoft::WRL::ComPtr<ID3D12Device2> a_Device, Microsoft::WRL::ComPtr<IDXGISwapChain4> a_SwapChain, Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> a_DescriptorHeap, D3D12_DESCRIPTOR_HEAP_TYPE a_Type);
			Microsoft::WRL::ComPtr<ID3D12CommandAllocator> CreateCommandAllocator(Microsoft::WRL::ComPtr<ID3D12Device2> a_Device, D3D12_COMMAND_LIST_TYPE a_Type);
			Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> CreateCommandList(Microsoft::WRL::ComPtr<ID3D12Device2> a_Device, Microsoft::WRL::ComPtr<ID3D12CommandAllocator> a_CommandAllocator, D3D12_COMMAND_LIST_TYPE a_Type);
			bool CreateFence(Microsoft::WRL::ComPtr<ID3D12Device2> a_Device);
			bool CreateEventHandle();

			void WaitForLastSubmittedFrame(Microsoft::WRL::ComPtr<ID3D12CommandQueue> a_CommandQueue, Microsoft::WRL::ComPtr<ID3D12Fence> a_Fence, uint64_t& a_FenceValue, HANDLE a_FenceEvent);
			void CleanUpRenderTargets(Microsoft::WRL::ComPtr<IDXGISwapChain4> a_SwapChain);
			void Flush(Microsoft::WRL::ComPtr<ID3D12CommandQueue> a_CommandQueue, Microsoft::WRL::ComPtr<ID3D12Fence> a_Fence, uint64_t& a_FenceValue, HANDLE a_FenceEvent);
			uint64_t Signal(Microsoft::WRL::ComPtr<ID3D12CommandQueue> a_CommandQueue, Microsoft::WRL::ComPtr<ID3D12Fence> a_Fence, uint64_t& a_FenceValue);
			void WaitForFenceValue(Microsoft::WRL::ComPtr<ID3D12Fence> a_Fence, uint64_t a_FenceValue, HANDLE a_FenceEvent, std::chrono::milliseconds a_Duration = std::chrono::milliseconds::max());

			FPSCounter m_FpsCounter;

			Microsoft::WRL::ComPtr<IDXGIAdapter4> g_DxgiAdapter4;
			Microsoft::WRL::ComPtr<ID3D12Device2> g_Device;
			Microsoft::WRL::ComPtr<ID3D12CommandQueue> g_CommandQueue;
			Microsoft::WRL::ComPtr<IDXGISwapChain4> g_SwapChain;
			ID3D12Resource* g_BackBuffers[g_NumFrames];
			Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> g_CommandList;
			Microsoft::WRL::ComPtr<ID3D12CommandAllocator> g_CommandAllocators[g_NumFrames];
			Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> g_RTVDescriptorHeap;
			Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> g_SRVDescriptorHeap;

			Microsoft::WRL::ComPtr<ID3D12Fence> g_Fence;
			bool g_TearingSupported = false;
			uint64_t g_FenceValue = 0;
			uint64_t g_FrameFenceValues[g_NumFrames] = {};
			HANDLE g_FenceEvent;
			UINT g_CurrentBackBufferIndex = 0;
			UINT g_RTVDescriptorSize = 0;
		};
	}
}