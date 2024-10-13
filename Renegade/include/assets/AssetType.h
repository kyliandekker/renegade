#pragma once

#include <string>

namespace renegade
{
	namespace assets
	{
		enum class AssetType
		{
			Cfg,
			Scene,
			Material,
			Texture,
			Sprite,
			Font,
			Sound,
			Song,
			VO,
			Animation,
			Localization,
			Model,
			Prefab,
		};

		inline std::string AssetTypeToString(AssetType a_AssetType)
		{
			switch (a_AssetType)
			{
				case AssetType::Cfg:
				{
					return "Config File";
				}
				case AssetType::Scene:
				{
					return "Scene File";
				}
				case AssetType::Material:
				{
					return "Material File";
				}
				case AssetType::Texture:
				{
					return "Texture File";
				}
				case AssetType::Sprite:
				{
					return "Sprite File";
				}
				case AssetType::Font:
				{
					return "Font File";
				}
				case AssetType::Sound:
				{
					return "Sound File";
				}
				case AssetType::Song:
				{
					return "Song File";
				}
				case AssetType::VO:
				{
					return "VO File";
				}
				case AssetType::Animation:
				{
					return "Animation File";
				}
				case AssetType::Localization:
				{
					return "Localization File";
				}
				case AssetType::Model:
				{
					return "Model File";
				}
				case AssetType::Prefab:
				{
					return "Prefab File";
				}
				default:
				{
					return "";
				}
			}
			return "";
		}
	}
}