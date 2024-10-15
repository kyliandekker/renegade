#include "graphics/DescHandle.h"

namespace renegade
{
	namespace graphics
	{
		void DescHandle::Release()
		{
			InUse--;
			if (InUse == 0)
			{
				Width = 0;
				Height = 0;
				Path.clear();

				if (Texture)
				{
					Texture.Get()->Release();  // ComPtr handles release
				}
				Texture = nullptr;
			}
		}

        bool DescHandle::Invalid() const
        {
            return Index == -1;
        }
	}
}