#ifdef __EDITOR__

#pragma once

#include "editor/ExplorerResource.h"

namespace renegade
{
	namespace editor
	{
		class SceneExplorerResource : public ExplorerResource
		{
		public:
			void SetWindowTitle();
			void SetDirty() override;
			void ResetDirty() override;
			bool Save();
			bool Load();
		};
	}
}

#endif // __EDITOR__