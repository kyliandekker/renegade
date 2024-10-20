#pragma once

#ifdef __EDITOR__

#include "editor/ExplorerResource.h"
#include "core/System.h"
#include "core/datatypes/Event.h"

namespace renegade
{
	namespace editor
	{
		namespace imgui
		{
			class EditorSelectable;
		}

		class AssetDatabase : public core::System
		{
		public:
			bool Initialize(int a_NumArgs = 0, ...) override;
			bool Destroy() override;

			void Rescan();
			void CheckAssetDatabase();

			ExplorerResource& GetRoot();

			SimpleEvent<> m_OnScanCompleted;
			SimpleEvent<> m_OnBeforeScan;
		private:
			bool m_Rescan;
			bool Scan();
			ExplorerResource m_Root;
		};
	}
}

#endif // __EDITOR__