#pragma once

#include <mutex>
#include "core/datatypes/Event.h"

namespace renegade
{
	namespace core
	{
		inline std::mutex m_EntityMutex;
	}
}