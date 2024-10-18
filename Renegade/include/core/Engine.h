#pragma once

#include "core/System.h"

#include <chrono>

#include "graphics/Window.h"
#include "gameplay/EntityComponentSystem.h"
#include "editor/Editor.h"

namespace renegade
{
	namespace core
	{
#ifdef _DEBUG
		class FPSCounter
		{
		public:
			float GetFPS();
			void AddFrame();
		private:
			unsigned int m_Frames = 0;
			float m_TimeAccumulation = 0.0f;
		};
#endif // _DEBUG

		class Engine : public System
		{
		public:
			bool Initialize(int a_NumArgs, ...) override;
			bool Destroy() override;

			graphics::Window& GetWindow();
			gameplay::EntityComponentSystem& GetECS();
#ifdef __EDITOR__
			editor::Editor& GetEditor();
#endif
			float GetFPS();

			float GetDeltaTime() const;
		private:
			graphics::Window m_Window;
			gameplay::EntityComponentSystem m_ECS;

			void UpdateDeltaTime();
#ifdef __EDITOR__
			editor::Editor m_Editor;
#endif

#ifdef _DEBUG
			FPSCounter m_FpsCounter;
#endif
			float m_DeltaTime = 0.0f;

			std::chrono::steady_clock::time_point m_TimeSinceEngineStart = std::chrono::steady_clock::now();
			std::chrono::steady_clock::time_point m_TimeSinceGameStart = std::chrono::steady_clock::now();
			std::chrono::steady_clock::time_point m_LastFrame = std::chrono::steady_clock::now();
		};
		inline extern Engine ENGINE = {};
	}
}