#pragma once

#include <d3d12.h>
#include <dxgi1_4.h>
#include <wrl/client.h>
#include <string>

namespace renegade
{
	namespace graphics
	{
		// TODO: Make this safer with getters and setters.
		class DescHandle
		{
		public:
			int InUse = 0;
			int Index = -1;
			std::string Path;
			D3D12_CPU_DESCRIPTOR_HANDLE CpuHandle;
			D3D12_GPU_DESCRIPTOR_HANDLE GpuHandle;
			Microsoft::WRL::ComPtr<ID3D12Resource> Texture = nullptr;
			int
				Width = 0,
				Height = 0;

			void Release();
			bool Invalid() const;
		};
	}
}