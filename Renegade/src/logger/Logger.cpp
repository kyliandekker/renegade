#include "logger/Logger.h"

#include <cstdarg>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <format>
#include <windows.h>
#include <iostream>

namespace renegade
{
	namespace logger
	{
		Message::Message(const std::string& a_RawMessage, const std::string& a_Location, LogSeverity a_Severity, const std::chrono::system_clock::time_point& a_Time) :
			m_RawMessage(a_RawMessage),
			m_Location(a_Location),
			m_Severity(a_Severity),
			m_Time(a_Time)
		{ }

		const std::string& Message::GetRawMessage() const
		{
			return m_RawMessage;
		}

		const std::string& Message::GetLocation() const
		{
			return m_Location;
		}

		LogSeverity Message::GetSeverity() const
		{
			return m_Severity;
		}

		const std::chrono::system_clock::time_point& Message::GetTime() const
		{
			return m_Time;
		}

		bool Logger::Initialize(int, ...)
		{
			bool success = System::Initialize();
			if (!success)
			{
				return false;
			}

			m_Thread = std::thread(&Logger::MessageQueue, this);
			m_Thread.detach();

			LOG(LOGSEVERITY_SUCCESS, "Logger initialized.");
			return success;
		}

		bool Logger::Destroy()
		{
			std::scoped_lock lock(m_MessagesMutex);
			while (!m_Messages.empty())
			{
				m_Messages.pop();
			}
			m_Ready = false; // Signal thread to finish
			if (m_Thread.joinable())
			{
				m_Thread.join();
			}
			return System::Destroy();
		}

		void Logger::Log(LogSeverity a_Severity, const char* a_Message, const char* a_File, int a_Line)
		{
			PrintMessage(a_Severity, a_Message, a_File, a_Line);
		}

		void Logger::LogF(LogSeverity a_Severity, const char* a_Message, const char* a_File, int a_Line, ...)
		{
			va_list va_format_list;
			va_start(va_format_list, a_Line);

			const size_t buffersize = vsnprintf(NULL, 0, a_Message, va_format_list) + 1;
			char* formatted_message = (char*)malloc(buffersize);
			vsnprintf(formatted_message, buffersize, a_Message, va_format_list);

			PrintMessage(a_Severity, formatted_message, a_File, a_Line);

			free(formatted_message);
		}

		void Logger::PrintMessage(LogSeverity a_Severity, const char* a_Message, const char* a_File, int a_Line)
		{
			struct tm lt;
			time_t t;
			t = time(NULL);
			localtime_s(&lt, &t);

			std::string message = std::format("{0} on line {1}",
				a_File,
				a_Line);

			std::scoped_lock lock(m_MessagesMutex);
			m_Messages.push(Message(a_Message, message, a_Severity, std::chrono::system_clock::now()));
		}

		constexpr auto COLOR_YELLOW = "\033[0;33m";
		constexpr auto COLOR_WHITE = "\033[0;37m";
		constexpr auto COLOR_GREEN = "\033[0;32m";
		constexpr auto COLOR_RED = "\033[31m";
		constexpr auto COLOR_CYAN = "\033[0;36m";
		constexpr auto COLOR_PURPLE = "\033[0;35m";
		constexpr auto COLOR_BLUE = "\033[0;34m";
		constexpr auto COLOR_PINK = "\033[1;35m";

		std::string LOGGER_SEVERITY_COLOR[7] =
		{
			COLOR_CYAN,
			COLOR_YELLOW,
			COLOR_RED,
			COLOR_RED,
			COLOR_GREEN,
			COLOR_GREEN,
			COLOR_PINK,
		};

		std::string LOGGER_SEVERITY_TEXT[7] =
		{
			"INFO",
			"WARNING",
			"ERROR",
			"ASSERT",
			"SUCCESS",
			"INFO SUCCESS",
			"AWESOME",
		};

		FILE* console = nullptr;
		void Logger::MessageQueue()
		{
			// Initialize console first.
#ifdef _DEBUG
			AllocConsole();
			freopen_s(&console, "CONOUT$", "w", stdout);

			HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
			DWORD dwMode = 0;
			GetConsoleMode(hOut, &dwMode);
			dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
			SetConsoleMode(hOut, dwMode);
#endif // _DEBUG
			while (m_Ready)
			{
				// Use scoped lock for queue access and printing to prevent mixing
				std::scoped_lock lock(m_MessagesMutex);

				if (!m_Messages.empty())
				{
					const Message lm = m_Messages.front();
					m_Messages.pop();

					std::string message =
						"[" + LOGGER_SEVERITY_COLOR[lm.GetSeverity()] +
						LOGGER_SEVERITY_TEXT[lm.GetSeverity()] +
						COLOR_WHITE + "] " + lm.GetRawMessage() + " " +
						lm.GetLocation();

					std::cout << message.c_str() << std::endl;
					fflush(stdout);

					OnMessageLogged(lm);
				}
			}
#ifdef _DEBUG
			fclose(console);
#endif
		}
}
}