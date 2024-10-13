#include "graphics/ImgInfo.h"
#include "core/Engine.h"

namespace renegade
{
	namespace graphics
	{
		ImgInfo::~ImgInfo()
		{
			if (m_Texture)
			{
				m_Texture.Reset();  // ComPtr handles release
			}
			m_Texture = nullptr;
			core::ENGINE.GetWindow().GetDX12Window().descriptor_index--;
		}
	}
}