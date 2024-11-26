#pragma once

#include "core/System.h"

#include <string>
#include <vector>
#include <shtypes.h>
#include <thread>
#include <functional>
#include <queue>
#include <mutex>
#include <future>

namespace renegade
{
	namespace core
	{
		class Data;
		class DataStream;
	}

	namespace file
	{
		struct Task
		{
			std::function<bool()> m_Function = nullptr;

			Task(std::function<bool()> a_Function);
		};

		struct PromiseTask : public Task
		{
			std::promise<bool>& m_Promise;
			std::future<bool>& m_Future;

			PromiseTask(std::function<bool()> a_Function, std::promise<bool>& a_Promise, std::future<bool>& a_Future);
		};

		class TaskQueue {
		public:
			void EnqueueTask(std::function<bool()> a_Function, std::promise<bool>& a_Promise, std::future<bool>& a_Future);
			void EnqueueTask(std::function<bool()> a_Function);
			void Run();

		private:
			std::queue<Task> m_Tasks;
			std::queue<PromiseTask> m_PromiseTasks;
			std::mutex m_Mutex;
		};

		class FileLoader : public core::System
		{
		public:
			bool Initialize(int nArgs = 0, ...) override;
			bool Destroy() override;

			void EnqueueTask(std::function<bool()> a_Function, std::promise<bool>& a_Promise, std::future<bool>& a_Future);
			void EnqueueTask(std::function<bool()> a_Function);
		private:
			TaskQueue m_Tasks;
			std::thread m_FileThread;
			void StartThread();
		public:
			static bool LoadFile(const std::string& a_Path, core::DataStream& a_Data);
			static bool SaveFile(const std::string& a_Path, const core::Data& a_Data);
			static bool DoesFileExist(const std::string& a_Path);
			static bool DoesFolderExist(const std::string& a_Path);
			static bool CreateFolder(const std::string& a_Path);
			static bool OpenInExplorer(const std::string& a_Path);
			static std::string GetAppDataPath();
			static bool PickContainer(std::string& a_Path);
			static bool PickFile(std::string& a_Path, const std::vector<COMDLG_FILTERSPEC>& a_Filters);
		};
	}
}