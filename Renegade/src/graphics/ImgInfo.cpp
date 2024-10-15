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
		}

		bool ImgInfo::Initialize(UINT width, UINT height)
		{
			m_Width = width;
			m_Height = height;

			return true;
		}
	}
}