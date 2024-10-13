#include "utils/string_extensions.h"

#include <algorithm>

namespace renegade
{
	namespace string_extensions
	{
		std::string ReplaceSlashes(std::string& path)
		{
			std::replace(path.begin(), path.end(), '\\', '/');
			return path;
		}

		std::string GetExtensionFromPath(const std::string& a_Path, bool a_IncludeDot)
		{
			return a_Path.substr(a_Path.find_last_of(".") + (a_IncludeDot ? 0 : 1));
		}

		std::string GetFileWithoutExtension(const std::string& a_Path)
		{
			return a_Path.substr(0, a_Path.find_last_of("."));
		}

		std::string GetFileName(const std::string& a_Path)
		{
			return a_Path.substr(a_Path.find_last_of("/") + 1);
		}

		int Strpos(const char* a_Haystack, const char* a_Needle, int a_Nth)
		{
			const char* res = a_Haystack;
			for (int i = 1; i <= a_Nth; i++)
			{
				res = strstr(res, a_Needle);
				if (!res)
				{
					return -1;
				}
				else if (i != a_Nth)
				{
					res++;
				}
			}
			return static_cast<int>(res - a_Haystack);
		}

		std::string GetRoot(const std::string& a_Path)
		{
			return a_Path.substr(Strpos(a_Path.c_str(), "/", 2) + 1);
		}

		bool EndsWith(std::string const& a_Value, std::string const& a_Ending)
		{
			if (a_Ending.size() > a_Value.size())
			{
				return false;
			}
			return std::equal(a_Ending.rbegin(), a_Ending.rend(), a_Value.rbegin());
		}

		bool StartsWith(std::string const& a_Value, std::string const& a_Starting)
		{
			return (a_Value.rfind(a_Starting, 0) == 0);
		}

		std::string StringToUpper(std::string a_StrToConvert)
		{
			std::transform(a_StrToConvert.begin(), a_StrToConvert.end(), a_StrToConvert.begin(), ::toupper);

			return a_StrToConvert;
		}

		std::string StringToLower(std::string a_StrToConvert)
		{
			std::transform(a_StrToConvert.begin(), a_StrToConvert.end(), a_StrToConvert.begin(), ::tolower);

			return a_StrToConvert;
		}

		std::string GetPath(const std::string& a_Path)
		{
			std::string f = a_Path.substr(0, a_Path.find_last_of("\\"));
			return f.substr(0, a_Path.find_last_of("/"));
		}
	}
}