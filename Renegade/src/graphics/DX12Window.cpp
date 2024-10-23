#include "graphics/DX12Window.h"

#include <d3dx12_core.h>
#include <d3dx12_resource_helpers.h>
#include <d3dx12_barriers.h>
#include <Windows.h>
#include <d3d12.h>
#include <dxgi1_4.h>
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

			va_end(list);

			bool success = CreateDeviceD3D(hwnd);
			if (success)
			{
				LOGF(LOGSEVERITY_SUCCESS, "DX12 initialized.");
			}

			return success;
		}

		bool DX12Window::Destroy()
		{
			CleanupDeviceD3D();
			return true;
		}

		void DX12Window::CreateRenderTarget()
		{
			for (UINT i = 0; i < NUM_RTV_DESC_HANDLES; i++)
			{
				ID3D12Resource* pBackBuffer = nullptr;
				g_pSwapChain->GetBuffer(i, IID_PPV_ARGS(&pBackBuffer));
				g_pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, g_rtvRenderTargetDescriptor[i]);
				g_mainRenderTargetResource[i] = pBackBuffer;
			}
			m_OnRenderTargetCreated();
		}

		glm::vec4 clear_color = glm::vec4(0.0f, 0.0f, 0.0f, 1.00f);
		void DX12Window::Render()
		{
			FrameContext* frameCtx = WaitForNextFrameResources();
			UINT backBufferIdx = g_pSwapChain->GetCurrentBackBufferIndex();
			frameCtx->CommandAllocator->Reset();

			D3D12_RESOURCE_BARRIER barrier = {};
			barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
			barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
			barrier.Transition.pResource = g_mainRenderTargetResource[backBufferIdx];
			barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
			barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
			barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
			g_pd3dCommandList->Reset(frameCtx->CommandAllocator, nullptr);
			g_pd3dCommandList->ResourceBarrier(1, &barrier);

			// Render Dear ImGui graphics
			const float clear_color_with_alpha[4] = { clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w };
			g_pd3dCommandList->ClearRenderTargetView(g_rtvRenderTargetDescriptor[backBufferIdx], clear_color_with_alpha, 0, nullptr);
			g_pd3dCommandList->OMSetRenderTargets(1, &g_rtvRenderTargetDescriptor[backBufferIdx], FALSE, nullptr);
			g_pd3dCommandList->SetDescriptorHeaps(1, &g_pd3dSrvDescHeap);

			m_OnDrawRenderData(g_pd3dCommandList);

			barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
			barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
			g_pd3dCommandList->ResourceBarrier(1, &barrier);
			g_pd3dCommandList->Close();

			g_pd3dCommandQueue->ExecuteCommandLists(1, (ID3D12CommandList* const*)&g_pd3dCommandList);

			g_pSwapChain->Present(1, 0); // Present with vsync
			//g_pSwapChain->Present(0, 0); // Present without vsync

			UINT64 fenceValue = g_fenceLastSignaledValue + 1;
			g_pd3dCommandQueue->Signal(g_fence, fenceValue);
			g_fenceLastSignaledValue = fenceValue;
			frameCtx->FenceValue = fenceValue;
		}

		void DX12Window::WaitForLastSubmittedFrame()
		{
			FrameContext* frameCtx = &g_frameContext[g_frameIndex % NUM_FRAMES_IN_FLIGHT];

			UINT64 fenceValue = frameCtx->FenceValue;
			if (fenceValue == 0)
			{
				return; // No fence was signaled
			}

			frameCtx->FenceValue = 0;
			if (g_fence->GetCompletedValue() >= fenceValue)
			{
				return;
			}

			g_fence->SetEventOnCompletion(fenceValue, g_fenceEvent);
			WaitForSingleObject(g_fenceEvent, INFINITE);
		}

		void DX12Window::CleanupRenderTarget()
		{
			WaitForLastSubmittedFrame();

			m_OnRenderTargetCleaned();
			for (UINT i = 0; i < NUM_RTV_DESC_HANDLES; i++)
			{
				if (g_mainRenderTargetResource[i]) { g_mainRenderTargetResource[i]->Release(); g_mainRenderTargetResource[i] = nullptr; }
			}
		}

		bool DX12Window::CreateDeviceD3D(HWND a_hWnd)
		{
			// Setup swap chain
			DXGI_SWAP_CHAIN_DESC1 sd;
			{
				ZeroMemory(&sd, sizeof(sd));
				sd.BufferCount = NUM_RTV_DESC_HANDLES;
				sd.Width = 0;
				sd.Height = 0;
				sd.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
				sd.Flags = DXGI_SWAP_CHAIN_FLAG_FRAME_LATENCY_WAITABLE_OBJECT;
				sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
				sd.SampleDesc.Count = 1;
				sd.SampleDesc.Quality = 0;
				sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
				sd.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
				sd.Scaling = DXGI_SCALING_STRETCH;
				sd.Stereo = FALSE;
			}

			// [DEBUG] Enable debug interface
#ifdef DX12_ENABLE_DEBUG_LAYER
			ID3D12Debug* debugController;
			if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
			{
				debugController->EnableDebugLayer();
				ID3D12Debug1* debugController1;
				if (SUCCEEDED(debugController->QueryInterface(IID_PPV_ARGS(&debugController1))))
				{
					debugController1->SetEnableGPUBasedValidation(TRUE);
				}
			}

#endif

			// Create device
			D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;
			if (D3D12CreateDevice(nullptr, featureLevel, IID_PPV_ARGS(&g_pd3dDevice)) != S_OK)
			{
				LOGF(LOGSEVERITY_ASSERT, "Could not create D3D12 device.");
				return false;
			}

			// [DEBUG] Setup debug interface to break on any warnings/errors
#ifdef DX12_ENABLE_DEBUG_LAYER
			if (debugController != nullptr)
			{
				ID3D12InfoQueue* pInfoQueue = nullptr;
				g_pd3dDevice->QueryInterface(IID_PPV_ARGS(&pInfoQueue));
				pInfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, true);
				pInfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, true);
				pInfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_WARNING, true);
				pInfoQueue->Release();
				debugController->Release();
			}
#endif

			{
				// Setup RTV heap.
				g_desc = {};
				g_desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
				g_desc.NumDescriptors = NUM_RTV_DESC_HANDLES;
				g_desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
				g_desc.NodeMask = 1;
				if (g_pd3dDevice->CreateDescriptorHeap(&g_desc, IID_PPV_ARGS(&g_pd3dRtvDescHeap)) != S_OK)
				{
					LOGF(LOGSEVERITY_ASSERT, "Could not create descriptor heap for DX12.");
					return false;
				}

				SIZE_T rtvDescriptorSize = g_pd3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
				D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = g_pd3dRtvDescHeap->GetCPUDescriptorHandleForHeapStart();
				for (UINT i = 0; i < NUM_RTV_DESC_HANDLES; i++)
				{
					g_rtvRenderTargetDescriptor[i] = rtvHandle;
					rtvHandle.ptr += rtvDescriptorSize;
				}

				// Setup SRV heap.
				g_imguiDesc = {};
				g_imguiDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
				g_imguiDesc.NumDescriptors = NUM_SRV_DESC_HANDLES;
				g_imguiDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
				if (g_pd3dDevice->CreateDescriptorHeap(&g_imguiDesc, IID_PPV_ARGS(&g_pd3dSrvDescHeap)) != S_OK)
				{
					LOGF(LOGSEVERITY_ASSERT, "Could not create descriptor heap for DX12.");
					return false;
				}

				// Ensure this heap is valid
				if (!g_pd3dSrvDescHeap)
				{
					LOGF(LOGSEVERITY_ERROR, "SRV descriptor heap is null.");
					return false;
				}

				for (size_t i = 0; i < NUM_SRV_DESC_HANDLES; i++)
				{
					UINT handle_increment = g_pd3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

					DescHandle& handle = m_SrvDescriptorHandles[i];
					handle.InUse = 0;
					handle.Index = static_cast<int>(i);
					handle.CpuHandle = g_pd3dSrvDescHeap->GetCPUDescriptorHandleForHeapStart();
					handle.CpuHandle.ptr += (handle_increment * handle.Index);
					handle.GpuHandle = g_pd3dSrvDescHeap->GetGPUDescriptorHandleForHeapStart();
					handle.GpuHandle.ptr += (handle_increment * handle.Index);
				}

#ifdef __EDITOR__
				m_SrvDescriptorHandles[0].InUse = 1;
#endif
			}

			{
				D3D12_COMMAND_QUEUE_DESC desc = {};
				desc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
				desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
				desc.NodeMask = 1;
				if (g_pd3dDevice->CreateCommandQueue(&desc, IID_PPV_ARGS(&g_pd3dCommandQueue)) != S_OK)
				{
					LOGF(LOGSEVERITY_ASSERT, "Could not create command queue for DX12.");
					return false;
				}
			}

			for (UINT i = 0; i < NUM_FRAMES_IN_FLIGHT; i++)
			{
				if (g_pd3dDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&g_frameContext[i].CommandAllocator)) != S_OK)
				{
					LOGF(LOGSEVERITY_ASSERT, "Could not create command allocator for DX12.");
					return false;
				}
			}

			if (g_pd3dDevice->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, g_frameContext[0].CommandAllocator, nullptr, IID_PPV_ARGS(&g_pd3dCommandList)) != S_OK ||
				g_pd3dCommandList->Close() != S_OK)
			{
				LOGF(LOGSEVERITY_ASSERT, "Could not create command list for DX12.");
				return false;
			}

			if (g_pd3dDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&g_fence)) != S_OK)
			{
				LOGF(LOGSEVERITY_ASSERT, "Could not create fence for DX12.");
				return false;
			}

			g_fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
			if (g_fenceEvent == nullptr)
			{
				LOGF(LOGSEVERITY_ASSERT, "Fence event was nullptr for DX12.");
				return false;
			}

			{
				IDXGIFactory4* dxgiFactory = nullptr;
				IDXGISwapChain1* swapChain1 = nullptr;
				if (CreateDXGIFactory1(IID_PPV_ARGS(&dxgiFactory)) != S_OK)
				{
					LOGF(LOGSEVERITY_ASSERT, "Could not create DXGI Factory for DX12.");
					return false;
				}
				if (dxgiFactory->CreateSwapChainForHwnd(g_pd3dCommandQueue, a_hWnd, &sd, nullptr, nullptr, &swapChain1) != S_OK)
				{
					LOGF(LOGSEVERITY_ASSERT, "Could not create swap chain for DX12.");
					return false;
				}
				if (swapChain1->QueryInterface(IID_PPV_ARGS(&g_pSwapChain)) != S_OK)
				{
					LOGF(LOGSEVERITY_ASSERT, "Could not query interface for DX12.");
					return false;
				}
				swapChain1->Release();
				dxgiFactory->Release();
				g_pSwapChain->SetMaximumFrameLatency(NUM_RTV_DESC_HANDLES);
				g_hSwapChainWaitableObject = g_pSwapChain->GetFrameLatencyWaitableObject();
			}

			CreateRenderTarget();
			LOGF(LOGSEVERITY_SUCCESS, "Created D3D Device for DX12.");
			return true;
		}

		void DX12Window::CleanupDeviceD3D()
		{
			CleanupRenderTarget();
			if (g_pSwapChain)
			{
				g_pSwapChain->SetFullscreenState(false, nullptr);
				g_pSwapChain->Release();
				g_pSwapChain = nullptr;
			}
			if (g_hSwapChainWaitableObject != nullptr)
			{
				CloseHandle(g_hSwapChainWaitableObject);
			}
			for (UINT i = 0; i < NUM_FRAMES_IN_FLIGHT; i++)
			{
				if (g_frameContext[i].CommandAllocator)
				{
					g_frameContext[i].CommandAllocator->Release();
					g_frameContext[i].CommandAllocator = nullptr;
				}
			}
			if (g_pd3dCommandQueue)
			{
				g_pd3dCommandQueue->Release();
				g_pd3dCommandQueue = nullptr;
			}
			if (g_pd3dCommandList)
			{
				g_pd3dCommandList->Release();
				g_pd3dCommandList = nullptr;
			}
			if (g_pd3dRtvDescHeap)
			{
				g_pd3dRtvDescHeap->Release();
				g_pd3dRtvDescHeap = nullptr;
			}
			if (g_pd3dSrvDescHeap)
			{
				g_pd3dSrvDescHeap->Release();
				g_pd3dSrvDescHeap = nullptr;
			}
			if (g_fence)
			{
				g_fence->Release();
				g_fence = nullptr;
			}
			if (g_fenceEvent)
			{
				CloseHandle(g_fenceEvent);
				g_fenceEvent = nullptr;
			}
			if (g_pd3dDevice)
			{
				g_pd3dDevice->Release();
				g_pd3dDevice = nullptr;
			}

			LOGF(LOGSEVERITY_SUCCESS, "Cleaned up D3D device from DX12.");
		}

		bool DX12Window::CleanupTextures()
		{
			int index = 0;
#ifdef __EDITOR__
			index = 1;
#endif
			for (size_t i = index; i < NUM_SRV_DESC_HANDLES; i++)
			{
				m_SrvDescriptorHandles[i].Release();
			}
			return true;
		}

		FrameContext* DX12Window::WaitForNextFrameResources()
		{
			UINT nextFrameIndex = g_frameIndex + 1;
			g_frameIndex = nextFrameIndex;

			HANDLE waitableObjects[] = { g_hSwapChainWaitableObject, nullptr };
			DWORD numWaitableObjects = 1;

			FrameContext* frameCtx = &g_frameContext[nextFrameIndex % NUM_FRAMES_IN_FLIGHT];
			UINT64 fenceValue = frameCtx->FenceValue;
			if (fenceValue != 0) // means no fence was signaled
			{
				frameCtx->FenceValue = 0;
				g_fence->SetEventOnCompletion(fenceValue, g_fenceEvent);
				waitableObjects[1] = g_fenceEvent;
				numWaitableObjects = 2;
			}

			if (numWaitableObjects > 0)
			{
				WaitForMultipleObjects(numWaitableObjects, waitableObjects, TRUE, INFINITE);
			}

			return frameCtx;
		}

		bool DX12Window::LoadTexture(const std::string& a_Path, DescHandle*& a_DescHandle)
		{
			for (size_t i = 0; i < NUM_SRV_DESC_HANDLES; i++)
			{
				if (m_SrvDescriptorHandles[i].Path == a_Path)
				{
					a_DescHandle = &m_SrvDescriptorHandles[i];
					a_DescHandle->InUse++;
					return true;
				}
			}

			for (size_t i = 0; i < NUM_SRV_DESC_HANDLES; i++)
			{
				if (m_SrvDescriptorHandles[i].InUse == 0)
				{
					a_DescHandle = &m_SrvDescriptorHandles[i];
					break;
				}
			}

			if (a_DescHandle->Invalid())
			{
				return false;
			}

			int texChannels;
			if (stbi_info(a_Path.c_str(), &a_DescHandle->Width, &a_DescHandle->Height, &texChannels) != 1)
			{
				LOGF(LOGSEVERITY_ERROR, "Could not load texture info '%s'.", a_Path.c_str());
				return false;
			}

			core::Data textureData((a_DescHandle->Width * a_DescHandle->Height) * 4);
			unsigned char* texPtr = textureData.tdata<unsigned char>();

			texPtr = stbi_load(a_Path.c_str(), &a_DescHandle->Width, &a_DescHandle->Height, &texChannels, STBI_rgb_alpha);

			if (textureData.empty())
			{
				LOGF(LOGSEVERITY_ERROR, "Could not load texture '%s'.", a_Path.c_str());
				return false;
			}

			// Create texture resource
			D3D12_HEAP_PROPERTIES props;
			memset(&props, 0, sizeof(D3D12_HEAP_PROPERTIES));
			props.Type = D3D12_HEAP_TYPE_DEFAULT;
			props.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
			props.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;

			D3D12_RESOURCE_DESC desc;
			ZeroMemory(&desc, sizeof(desc));
			desc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
			desc.Alignment = 0;
			desc.Width = a_DescHandle->Width;
			desc.Height = a_DescHandle->Height;
			desc.DepthOrArraySize = 1;
			desc.MipLevels = 1;
			desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			desc.SampleDesc.Count = 1;
			desc.SampleDesc.Quality = 0;
			desc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
			desc.Flags = D3D12_RESOURCE_FLAG_NONE;

			ID3D12Resource* pTexture = NULL;
			HRESULT hr = g_pd3dDevice->CreateCommittedResource(&props, D3D12_HEAP_FLAG_NONE, &desc,
				D3D12_RESOURCE_STATE_COPY_DEST, NULL, IID_PPV_ARGS(&pTexture));

			if (FAILED(hr))
			{
				LOGF(LOGSEVERITY_ERROR, "Could not create texture resource for image '%s'.", a_Path.c_str());
				return false;
			}

			// Create a temporary upload resource to move the data in
			UINT uploadPitch = (a_DescHandle->Width * 4 + D3D12_TEXTURE_DATA_PITCH_ALIGNMENT - 1u) & ~(D3D12_TEXTURE_DATA_PITCH_ALIGNMENT - 1u);
			UINT uploadSize = a_DescHandle->Height * uploadPitch;
			desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
			desc.Alignment = 0;
			desc.Width = uploadSize;
			desc.Height = 1;
			desc.DepthOrArraySize = 1;
			desc.MipLevels = 1;
			desc.Format = DXGI_FORMAT_UNKNOWN;
			desc.SampleDesc.Count = 1;
			desc.SampleDesc.Quality = 0;
			desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
			desc.Flags = D3D12_RESOURCE_FLAG_NONE;

			props.Type = D3D12_HEAP_TYPE_UPLOAD;
			props.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
			props.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;

			ID3D12Resource* uploadBuffer = NULL;
			hr = g_pd3dDevice->CreateCommittedResource(&props, D3D12_HEAP_FLAG_NONE, &desc,
				D3D12_RESOURCE_STATE_GENERIC_READ, NULL, IID_PPV_ARGS(&uploadBuffer));

			if (FAILED(hr))
			{
				LOGF(LOGSEVERITY_ERROR, "Could not create temporary committed resource for image '%s'.", a_Path.c_str());
				return false;
			}

			// Write pixels into the upload resource
			void* mapped = NULL;
			D3D12_RANGE range = { 0, uploadSize };
			hr = uploadBuffer->Map(0, &range, &mapped);
			if (FAILED(hr))
			{
				LOGF(LOGSEVERITY_ERROR, "Could not map resource to upload buffer for image '%s'.", a_Path.c_str());
				return false;
			}

			for (int y = 0; y < a_DescHandle->Height; y++)
			{
				memcpy((void*)((uintptr_t)mapped + y * uploadPitch), texPtr + y * a_DescHandle->Width * 4, a_DescHandle->Width * 4);
			}
			uploadBuffer->Unmap(0, &range);

			// Copy the upload resource content into the real resource
			D3D12_TEXTURE_COPY_LOCATION srcLocation = {};
			srcLocation.pResource = uploadBuffer;
			srcLocation.Type = D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT;
			srcLocation.PlacedFootprint.Footprint.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			srcLocation.PlacedFootprint.Footprint.Width = a_DescHandle->Width;
			srcLocation.PlacedFootprint.Footprint.Height = a_DescHandle->Height;
			srcLocation.PlacedFootprint.Footprint.Depth = 1;
			srcLocation.PlacedFootprint.Footprint.RowPitch = uploadPitch;

			D3D12_TEXTURE_COPY_LOCATION dstLocation = {};
			dstLocation.pResource = pTexture;
			dstLocation.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
			dstLocation.SubresourceIndex = 0;

			D3D12_RESOURCE_BARRIER barrier = {};
			barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
			barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
			barrier.Transition.pResource = pTexture;
			barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
			barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
			barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;

			// Create a temporary command queue to do the copy with
			ID3D12Fence* fence = NULL;
			hr = g_pd3dDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence));
			if (FAILED(hr))
			{
				LOGF(LOGSEVERITY_ERROR, "Could not create fence for image '%s'.", a_Path.c_str());
				return false;
			}

			HANDLE event = CreateEvent(0, 0, 0, 0);
			if (event == nullptr)
			{
				LOGF(LOGSEVERITY_ERROR, "Could not create event for image '%s'.", a_Path.c_str());
				return false;
			}

			D3D12_COMMAND_QUEUE_DESC queueDesc = {};
			queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
			queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
			queueDesc.NodeMask = 1;

			ID3D12CommandQueue* cmdQueue = NULL;
			hr = g_pd3dDevice->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&cmdQueue));
			if (FAILED(hr))
			{
				LOGF(LOGSEVERITY_ERROR, "Could not create command queue for image '%s'.", a_Path.c_str());
				return false;
			}

			ID3D12CommandAllocator* cmdAlloc = NULL;
			hr = g_pd3dDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&cmdAlloc));
			if (FAILED(hr))
			{
				LOGF(LOGSEVERITY_ERROR, "Could not create command allocator for image '%s'.", a_Path.c_str());
				return false;
			}

			ID3D12GraphicsCommandList* cmdList = NULL;
			hr = g_pd3dDevice->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, cmdAlloc, NULL, IID_PPV_ARGS(&cmdList));
			if (FAILED(hr))
			{
				LOGF(LOGSEVERITY_ERROR, "Could not create command list for image '%s'.", a_Path.c_str());
				return false;
			}

			cmdList->CopyTextureRegion(&dstLocation, 0, 0, 0, &srcLocation, NULL);
			cmdList->ResourceBarrier(1, &barrier);

			hr = cmdList->Close();
			if (FAILED(hr))
			{
				LOGF(LOGSEVERITY_ERROR, "Could not close command list for image '%s'.", a_Path.c_str());
				return false;
			}

			// Execute the copy
			cmdQueue->ExecuteCommandLists(1, (ID3D12CommandList* const*)&cmdList);
			hr = cmdQueue->Signal(fence, 1);
			if (FAILED(hr))
			{
				LOGF(LOGSEVERITY_ERROR, "Could not signal fence for image '%s'.", a_Path.c_str());
				return false;
			}

			// Wait for everything to complete
			fence->SetEventOnCompletion(1, event);
			WaitForSingleObject(event, INFINITE);

			// Tear down our temporary command queue and release the upload resource
			cmdList->Release();
			cmdAlloc->Release();
			cmdQueue->Release();
			CloseHandle(event);
			fence->Release();
			uploadBuffer->Release();

			if (!pTexture)
			{
				LOGF(LOGSEVERITY_ERROR, "pTexture is null before creating SRV.");
				return false;
			}

			// Create a shader resource view for the texture
			D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc;
			ZeroMemory(&srvDesc, sizeof(srvDesc));
			srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
			srvDesc.Texture2D.MipLevels = desc.MipLevels;
			srvDesc.Texture2D.MostDetailedMip = 0;
			srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
			g_pd3dDevice->CreateShaderResourceView(pTexture, &srvDesc, a_DescHandle->CpuHandle);

			// Return results
			a_DescHandle->Texture = pTexture;
			a_DescHandle->InUse++;

			return true;
		}
	}
}