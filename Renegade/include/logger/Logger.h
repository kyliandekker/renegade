#pragma once

#include "core/System.h"

#include <assert.h>
#include <queue>
#include <thread>
#include <string>
#include <mutex>

#include "core/datatypes/Event.h"

namespace renegade
{
	typedef enum LogSeverity {
		LOGSEVERITY_INFO,
		LOGSEVERITY_WARNING,
		LOGSEVERITY_ERROR,
		LOGSEVERITY_ASSERT,
		LOGSEVERITY_SUCCESS,
		LOGSEVERITY_INFO_SUCCESS,
		LOGSEVERITY_AWESOME,
	} LogSeverity;

	namespace logger
	{
#define ASSERT_ON_ASSERT false

		class Message
		{
		public:
			Message(const std::string& a_RawMessage, const std::string& a_Location, LogSeverity a_Severity, const std::chrono::system_clock::time_point& a_Time);

			const std::string& GetRawMessage() const;
			const std::string& GetLocation() const;
			LogSeverity GetSeverity() const;
			const std::chrono::system_clock::time_point& GetTime() const;
		private:
			std::string m_RawMessage;
			std::string m_Location;
			LogSeverity m_Severity;
			std::chrono::system_clock::time_point m_Time;
		};

		class Logger : public core::System
		{
		public:
			bool Initialize(int a_NumArgs = 0, ...) override;
			bool Destroy() override;

			void Log(LogSeverity a_Severity, const char* a_Message, const char* a_File, int a_Line);
			void LogF(LogSeverity a_Severity, const char* a_Message, const char* a_File, int a_Line, ...);
			void PrintMessage(LogSeverity a_Severity, const char* a_Message, const char* a_File, int a_Line);
			void MessageQueue();

			Event<const Message&> OnMessageLogged;
		private:
			std::queue<Message> m_Messages;
			std::thread m_Thread;
			std::mutex m_MessagesMutex;
		};
		inline extern Logger LOGGER = {};
	}
}

#define LOGF(a_Severity, a_Message, ...)\
do{\
	if (ASSERT_ON_ASSERT && a_Severity == LogSeverity::LOGSEVERITY_ASSERT)\
		assert(false);\
	logger::LOGGER.LogF(a_Severity, a_Message, __FILE__, __LINE__, __VA_ARGS__);\
} while (0)

#define LOG(a_Message, ...)\
do{\
	logger::LOGGER.LogF(logger::LOGSEVERITY_INFO, a_Message, __FILE__, __LINE__, __VA_ARGS__);\
} while (0)
