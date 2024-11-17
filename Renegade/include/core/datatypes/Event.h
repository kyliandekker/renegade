#pragma once

#include <vector>
#include <functional>

namespace renegade
{
	template<typename... Args>
	class Event {
	public:
		void operator+=(const std::function<void(Args...)>& a_Listener)
		{
			auto it = std::find_if(m_Listeners.begin(), m_Listeners.end(),
				[&](const std::function<void(Args...)>& other)
				{
					return a_Listener.target_type() == other.target_type();
				});

			if (it != m_Listeners.end())
			{
				return;
			}
			m_Listeners.push_back(a_Listener);
		}

		// Remove a listener (unsubscribe)
		void operator-=(const std::function<void(Args...)>& a_Listener)
		{
			auto it = std::find_if(m_Listeners.begin(), m_Listeners.end(),
				[&](const std::function<void(Args...)>& other)
				{
					return a_Listener.target_type() == other.target_type();
				});

			if (it != m_Listeners.end())
			{
				m_Listeners.erase(it);
			}
		}

		// Notify all listeners (invoke event)
		void operator()(Args... args) const
		{
			invoke(args...);
		}

		void invoke(Args... args) const
		{
			for (auto& listener : m_Listeners)
			{
				if (listener)
				{
					listener(args...);
				}
			}
		}

		void clear()
		{
			m_Listeners.clear();
		}

	private:
		std::vector<std::function<void(Args...)>> m_Listeners;
	};

	template<typename... Args>
	class SimpleEvent {
	public:
		void operator+=(const std::function<void(Args...)>& a_Listener)
		{
			listener = a_Listener;
		}

		void operator-=(const std::function<void(Args...)>& a_Listener)
		{
			listener = nullptr;
		}

		void operator()(Args... args) const
		{
			invoke(args...);
		}

		void invoke(Args... args) const
		{
			if (listener)
			{
			listener(args...);
			}
		}

		void clear()
		{
			listener = nullptr;
		}

	private:
		std::function<void(Args...)> listener;
	};
}