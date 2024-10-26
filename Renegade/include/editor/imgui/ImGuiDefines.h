#pragma once

#ifdef __EDITOR__

#include <string>

namespace renegade
{
	namespace editor
	{
		namespace imgui
		{
			// https://fontawesome.com/icons
			// https://r12a.github.io/app-conversion/
			// &#xf03d;
#define ICON_MIN_FA 0xe005
#define ICON_MAX_16_FA 0xf8ff
#define ICON_MAX_FA 0xf8ff
#define ICON_FA_CAMERA "\xEF\x80\xBD"
#define ICON_FA_LIGHT "\xEF\x83\xAB"
#define ICON_FA_LIGHT_ON "\xEF\x99\xB2"
#define ICON_FA_CUBE "\xEF\x86\xB2"
#define ICON_FA_BOX_COLLIDER "\xEF\x86\xB2"
#define ICON_FA_PLAYER "\xEF\x86\x83"
#define ICON_FA_CHICKEN "\xEF\x9F\xBB"
#define ICON_FA_SPHERE_COLLIDER "\xEF\x84\x91"
#define ICON_FA_MOVE "\xEF\x81\x87"
#define ICON_FA_ROTATE "\xEF\x8B\xB1"
#define ICON_FA_AUDIO "\xEF\x80\xA5"
#define ICON_FA_CHECKMARK_CHECKED "\xEF\x85\x8A"
#define ICON_FA_CHECKMARK "\xEF\x83\x88"
#define ICON_FA_COPY "\xEF\x83\x85"
#define ICON_FA_PASTE "\xEF\x83\xAA"
#define ICON_FA_RENAME "\xEF\x8C\x9C"
#define ICON_FA_DELETE "\xEF\x87\xB8"
#define ICON_FA_PARTICLE_SYSTEM "\xEF\x9D\xA2"
#define ICON_FA_FOLDED_IN "\xEF\x83\x9A"
#define ICON_FA_FOLDED_OUT "\xEF\x83\x97"
#define ICON_FA_ADD "\x2B"
#define ICON_FA_SEARCH "\xEF\x80\x82"
#define ICON_FA_ERASE "\xEF\x84\xAD"
#define ICON_FA_DOWN_TO_BOTTOM "\xEF\x8D\x8A"
#define ICON_FA_FOLDER "\xEF\x81\xBB"
#define ICON_FA_FOLDER_OPEN "\xEF\x81\xBC"
#define ICON_FA_ARROW_TURN_UP "\xEF\x85\x88"
#define ICON_FA_FILE_ARROW_UP "\xEF\x95\xB4"
#define ICON_FA_FOLDER_PLUS "\xef\x99\x9e"
#define ICON_FA_ARROW_RIGHT "\xEF\x81\xA1"
#define ICON_FA_CROSS "\xEF\x80\x8D"
#define ICON_FA_REFRESH "\xEF\x80\xA1"
#define ICON_FA_SAVE "\xEF\x83\x87"

/* WINDOW ICONS */
#define ICON_FA_SCENE "\xEF\x80\x8A"
#define ICON_FA_HIERARCHY "\xEF\x80\xBA"
#define ICON_FA_INSPECTOR "\xEF\x81\x9A"
#define ICON_FA_CONSOLE "\xEE\x88\xB6"
#define ICON_FA_EXPLORER ICON_FA_FOLDER

/* LOGGER */
#define ICON_FA_INFO "\xEF\x81\x9A"
#define ICON_FA_WARNING "\xEF\x81\xAA"
#define ICON_FA_ERROR "\xEF\x81\x97"
#define ICON_FA_ASSERT "\xEF\x9E\xBA"
#define ICON_FA_SUCCESS "\xEF\x81\x98"
#define ICON_FA_INFO_SUCCESS "\xEE\x84\x8D"
#define ICON_FA_AWESOME "\xEF\x93\x87"

/* ASSET ICONS */
#define ICON_FA_ASSET_CFG "\xEF\x80\x93"
#define ICON_FA_ASSET_SCENE "\xEF\x80\x8A"
#define ICON_FA_ASSET_MAT "\xEF\x9C\x9E"
#define ICON_FA_ASSET_TEX "\xEF\x87\x85"
#define ICON_FA_ASSET_SPR "\xEF\x87\x85"
#define ICON_FA_ASSET_FONT "\xEF\xA1\xA6"
#define ICON_FA_ASSET_SND "\xEF\x87\x87"
#define ICON_FA_ASSET_SONG "\xEF\xA2\xB6"
#define ICON_FA_ASSET_VO "\xEF\x87\x87"
#define ICON_FA_ASSET_ANIM "\xEE\x8A\x9C"
#define ICON_FA_ASSET_LOC "\xEF\x8C\x9C"
#define ICON_FA_ASSET_MOD "\xEF\x97\x8B"

/* SCENE WINDOW */
#define ICON_FA_PLAY "\xEF\x81\x8B"
#define ICON_FA_PAUSE "\xEF\x81\x8C"
#define ICON_FA_STOP "\xEF\x81\x8D"

#define IMGUI_INDENT 16.0f

			std::string IMGUI_FORMAT_ID(const std::string& a_Text, const char* a_ID, const std::string& a_IDName);

#define POPUP_WINDOW_ID "###POPUP_"
#define POPUP_WINDOW_BUTTON_ID "###POPUP_DIALOG_SAVE_BUTTON_"
#define CHILD_ID "###CHILD_"
#define BUTTON_ID "###BUTTON_"
#define COMBO_ID "###COMBO_"
#define WINDOW_ID "###WINDOW_"
#define MENU_ITEM_ID "###MENU_ITEM_"
#define TREE_NODE_ID "###TREE_NODE_"
#define MENU_ID "###MENU_"
#define INPUT_ID "###INPUT_"
#define CHECKBOX_ID "###CHECKBOX_"
#define FOLD_ID "###CHECKBOX_"
#define DRAG_ITEM_ID "###DRAG_ITEM_"
#define SELECTABLE_ID "###SELECTABLE_"
#define FOLDOUT_ID "###FOLDOUT_"
		}
	}
}

#endif // __EDITOR__