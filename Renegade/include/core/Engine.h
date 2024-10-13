#pragma once

#include "core/System.h"

#include "graphics/Window.h"
#include "editor/Editor.h"

namespace renegade
{
	namespace core
	{
		class Engine : public System
		{
		public:
			bool Initialize(int a_NumArgs, ...) override;
			bool Destroy() override;

			graphics::Window& GetWindow();

#ifdef __EDITOR__
			editor::Editor& GetEditor();
#endif
		private:
			graphics::Window m_Window;
#ifdef __EDITOR__
			editor::Editor m_Editor;
#endif
		};
		inline extern Engine ENGINE = {};
	}
}