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
	}
}