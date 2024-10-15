#pragma once

#include "editor/ExplorerResource.h"
#include "core/System.h"

#ifdef __EDITOR__

namespace renegade
{
	namespace editor
	{
		class EditorSelectable;

		class AssetDatabase : public core::System
		{
		public:
			bool Initialize(int a_NumArgs = 0, ...) override;
			bool Destroy() override;

			bool Scan();
			ExplorerResource m_Root;
			EditorSelectable* m_EditorSelectable;
		};
	}
}

#endif // __EDITOR__