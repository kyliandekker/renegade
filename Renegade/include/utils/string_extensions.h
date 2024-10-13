#pragma once

#include <string>

namespace renegade
{
	namespace string_extensions
	{
		std::string ReplaceSlashes(std::string& path);
		std::string GetExtensionFromPath(const std::string& a_Path, bool a_IncludeDot = false);
		std::string GetFileWithoutExtension(const std::string& a_Path);
		std::string GetFileName(const std::string& a_Path);
		int Strpos(const char* a_Haystack, const char* a_Needle, int a_Nth);
		std::string GetRoot(const std::string& a_Path);
		bool EndsWith(std::string const& a_Value, std::string const& a_Ending);
		bool StartsWith(std::string const& value, std::string const& starting);
		std::string StringToUpper(std::string a_StrToConvert);
		std::string StringToLower(std::string a_StrToConvert);
		std::string GetPath(const std::string& a_Path);
	}
}