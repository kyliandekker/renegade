#pragma once

#include <thread>

namespace renegade
{
	namespace core
	{
		class System
		{
		public:
			virtual bool Initialize(int a_NumArgs = 0, ...);
			virtual bool Destroy();
			bool Ready() const;
		protected:
			bool m_Ready = false;
		};

		template <typename... Args>
		bool InitializeSystem(System* a_System, int a_NumArgs = 0, Args&&... a_Args)
		{
			// Initialize system.
			if (!a_System->Initialize(a_NumArgs, std::forward<Args>(a_Args)...))
			{
				return false;
			}

			// Wait until the system is ready.
			while (!a_System->Ready())
			{
				// Wait...
				std::this_thread::yield();
			}
			return true;
		}

	}
}