#pragma once

#include <rapidjson/document.h>
#include <string>

namespace renegade
{
	namespace gameplay
	{
		class Component
		{
		public:
			virtual ~Component() = default;

			virtual std::string GetPropertyName() const = 0;
			virtual void Serialize(rapidjson::Document& a_Document) const = 0;
			virtual void Deserialize(const rapidjson::Document& a_Document) = 0;
		};
	}
}