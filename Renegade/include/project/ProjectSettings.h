#pragma once

#include <vector>
#include <locale>
#include <string>

namespace renegade
{
	namespace project
	{
		enum Language
		{
			English,
			Dutch,
		};

		inline std::string LanguageToString(Language a_Language)
		{
			switch (a_Language)
			{
				case English:
				{
					return "English";
				}
				case Dutch:
				{
					return "Nederlands";
				}
			}
			return "Unknown";
		}

		class ProjectSettings
		{
		public:
			bool Load(const std::string& a_Path);
			bool Save(const std::string& a_Path) const;

			const std::vector<Language>& GetLanguages() const;
			void AddLanguage(Language a_Language);
			void RemoveLanguage(Language a_Language);
		private:
			std::vector<Language> m_Languages;
		};
	}
}