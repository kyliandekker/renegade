#include "file/FileLoader.h"

#include <filesystem>
#include <ShlObj_core.h>

#include "core/datatypes/Data.h"
#include "core/datatypes/DataStream.h"
#include "core/Engine.h"

namespace fs = std::filesystem;

namespace renegade
{
	namespace file
	{
		Task::Task(std::function<bool()> a_Function) : m_Function(a_Function)
		{ }

		PromiseTask::PromiseTask(std::function<bool()> a_Function, std::promise<bool>& a_Promise, std::future<bool>& a_Future) : Task(a_Function), m_Promise(a_Promise), m_Future(a_Future)
		{ }

		void TaskQueue::EnqueueTask(std::function<bool()> a_Function, std::promise<bool>& a_Promise, std::future<bool>& a_Future)
		{
			std::lock_guard<std::mutex> lock(m_Mutex);
			m_PromiseTasks.push(PromiseTask(std::move(a_Function), a_Promise, a_Future));
		}

		void TaskQueue::EnqueueTask(std::function<bool()> a_Function)
		{
			std::lock_guard<std::mutex> lock(m_Mutex);
			m_Tasks.push(Task(std::move(a_Function)));
		}

		void TaskQueue::Run()
		{
			if (!m_PromiseTasks.empty())
			{
				PromiseTask task = std::move(m_PromiseTasks.front());
				m_PromiseTasks.pop();
				bool success = task.m_Function();
				task.m_Promise.set_value(success);
			}
			else if (!m_Tasks.empty())
			{
				Task task = std::move(m_Tasks.front());
				m_Tasks.pop();
				bool success = task.m_Function();
			}
		}

		bool FileLoader::Initialize(int nArgs, ...)
		{
			m_FileThread = std::thread(&FileLoader::StartThread, this);
			bool success = System::Initialize();

			LOG(LOGSEVERITY_SUCCESS, "File loader has been created.");
			return success;
		}

		bool FileLoader::Destroy()
		{
			m_FileThread.join();

			LOG(LOGSEVERITY_SUCCESS, "Destroyed file loader.");
			return System::Destroy();
		}

		void FileLoader::EnqueueTask(std::function<bool()> a_Function, std::promise<bool>& a_Promise, std::future<bool>& a_Future)
		{
			m_Tasks.EnqueueTask(a_Function, a_Promise, a_Future);
		}

		void FileLoader::EnqueueTask(std::function<bool()> a_Function)
		{
			m_Tasks.EnqueueTask(a_Function);
		}

		void FileLoader::StartThread()
		{
			while (m_Ready)
			{
				m_Tasks.Run();
			}
		}

		bool FileLoader::LoadFile(const std::string& a_Path, core::DataStream& a_Data)
		{
			FILE* file = nullptr;
			fopen_s(&file, a_Path.c_str(), "rb");
			if (!file)
			{
				return false;
			}

			fseek(file, 0, SEEK_END);
			size_t fileSize = ftell(file);
			rewind(file);

			a_Data = core::DataStream(fileSize);
			fread(a_Data.data(), fileSize, 1, file);

			fclose(file);

			return true;
		}

		bool FileLoader::SaveFile(const std::string& a_Path, const core::Data& a_Data)
		{
			FILE* file = nullptr;
			fopen_s(&file, a_Path.c_str(), "wb");
			if (!file)
			{
				return false;
			}

			fwrite(a_Data.data(), a_Data.size(), 1, file);

			fclose(file);

			return true;
		}

		bool FileLoader::DoesFileExist(const std::string& a_Path)
		{
			return fs::exists(a_Path);
		}

        bool FileLoader::DoesFolderExist(const std::string& a_Path)
        {
            return fs::exists(a_Path);
        }

		bool FileLoader::CreateFolder(const std::string& a_Path)
		{
			return fs::create_directories(a_Path);
		}

		bool FileLoader::OpenInExplorer(const std::string& a_Path)
		{
			ShellExecuteA(NULL, "open", a_Path.c_str(), NULL, NULL, SW_SHOWDEFAULT);
			return true;
		}

        std::string FileLoader::GetAppDataPath()
        {
			PWSTR path_tmp;
			std::filesystem::path path;
			SHGetKnownFolderPath(FOLDERID_RoamingAppData, 0, nullptr, &path_tmp);
			path = path_tmp;

			std::string appDataPath = std::string(path.generic_string());
			fs::create_directories(appDataPath);

			return appDataPath;
        }

		bool genericFileOpen(std::string& path, const IID rclsid, FILEOPENDIALOGOPTIONS options, const std::vector<COMDLG_FILTERSPEC>& filters = {})
		{
			IFileDialog* pfd = nullptr;
			HRESULT hr = CoCreateInstance(rclsid, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pfd));
			if (FAILED(hr))
			{
				return false; // Failed to create file dialog
			}

			if (!filters.empty())
			{
				hr = pfd->SetFileTypes(static_cast<UINT>(filters.size()), filters.data());
				if (FAILED(hr))
				{
					pfd->Release();
					return false;
				}
			}

			DWORD dwOptions;
			hr = pfd->GetOptions(&dwOptions);
			if (SUCCEEDED(hr))
			{
				pfd->SetOptions(dwOptions | options);
			}

			hr = pfd->Show(core::ENGINE.GetWindow().GetHWnd());
			if (SUCCEEDED(hr))
			{
				IShellItem* psi;
				hr = pfd->GetResult(&psi);
				if (SUCCEEDED(hr))
				{
					LPWSTR pszPath;
					hr = psi->GetDisplayName(SIGDN_DESKTOPABSOLUTEPARSING, &pszPath);
					if (SUCCEEDED(hr))
					{
						const std::wstring wide(pszPath);
						path = std::string(wide.begin(), wide.end());

						CoTaskMemFree(pszPath);
						psi->Release();
						pfd->Release();
						return true;
					}
					psi->Release();
				}
			}

			pfd->Release();
			return false;
		}

		bool FileLoader::PickContainer(std::string& a_Path)
		{
			return genericFileOpen(a_Path, CLSID_FileOpenDialog, FOS_PICKFOLDERS);
		}

		bool FileLoader::PickFile(std::string& a_Path, const std::vector<COMDLG_FILTERSPEC>& a_Filters)
		{
			return genericFileOpen(a_Path, CLSID_FileOpenDialog, 0, a_Filters);
		}
	}
}