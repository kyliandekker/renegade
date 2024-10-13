#pragma once

#include <d3d12.h>
#include <dxgi1_4.h>
#include <wrl.h>

namespace renegade
{
	namespace graphics
	{
		struct ImgInfo
		{
			~ImgInfo();

			D3D12_CPU_DESCRIPTOR_HANDLE m_Srv_cpu_handle;
			D3D12_GPU_DESCRIPTOR_HANDLE  m_Srv_gpu_handle;
			int m_Width, m_Height;

			Microsoft::WRL::ComPtr<ID3D12Resource> m_Texture;
		};
	}
}