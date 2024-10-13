#include "core/System.h"

namespace renegade
{
	namespace core
	{
		bool System::Initialize(int, ...)
		{
			m_Ready = true;
			return true;
		}

		bool System::Destroy()
		{
			m_Ready = false;
			return true;
		}

		bool System::Ready() const
		{
			return m_Ready;
		}
	}
}