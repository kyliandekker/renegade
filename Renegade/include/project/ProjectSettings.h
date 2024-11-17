#pragma once

#include <vector>
#include <locale>
#include <string>

namespace renegade
{
	namespace project
	{
		class ProjectSettings
		{
		public:
			bool Load(const std::string& a_Path);
			bool Save(const std::string& a_Path) const;

			void AddLanguage(int a_Language);
			void RemoveLanguage(int a_Language);
		private:
			std::vector<int> m_Languages;
		};
	}
}