#pragma once

#include "core/System.h"

#include <chrono>

#include "graphics/Window.h"
#include "gameplay/EntityComponentSystem.h"
#include "editor/Editor.h"
#include "file/FileLoader.h"

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
			gameplay::EntityComponentSystem& GetECS();
			file::FileLoader& GetFileLoader();
#ifdef __EDITOR__
			editor::Editor& GetEditor();
#endif
		private:
			graphics::Window m_Window;
			gameplay::EntityComponentSystem m_ECS;
			file::FileLoader m_FileLoader;
#ifdef __EDITOR__
			editor::Editor m_Editor;
#endif

			float m_DeltaTime = 0.0f;

			std::chrono::steady_clock::time_point m_TimeSinceEngineStart = std::chrono::steady_clock::now();
			std::chrono::steady_clock::time_point m_TimeSinceGameStart = std::chrono::steady_clock::now();
			std::chrono::steady_clock::time_point m_LastFrame = std::chrono::steady_clock::now();
		};
		inline extern Engine ENGINE = {};
	}
}