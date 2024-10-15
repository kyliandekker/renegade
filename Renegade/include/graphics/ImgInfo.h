#pragma once

#include <d3d12.h>
#include <dxgi1_4.h>
#include <wrl/client.h>

namespace renegade
{
	namespace graphics
	{
		struct ImgInfo
		{
			~ImgInfo();

			bool Initialize(UINT width, UINT height);

			int m_Width, m_Height;

			D3D12_CPU_DESCRIPTOR_HANDLE srv_cpu_handle;
			D3D12_GPU_DESCRIPTOR_HANDLE srv_gpu_handle;
			Microsoft::WRL::ComPtr<ID3D12Resource> m_Texture;
		};
	}
}