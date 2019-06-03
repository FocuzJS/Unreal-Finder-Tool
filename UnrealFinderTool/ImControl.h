#pragma once
#include "ImGUI/imgui.h"
#include <vector>

#define ENABLE_DISABLE_WIDGET(uiCode, disabledBool) if (disabledBool) { ui::PushItemFlag(ImGuiItemFlags_Disabled, true);ui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f); } uiCode; if (disabledBool) { ImGui::PopItemFlag();ImGui::PopStyleVar(); }
#define ENABLE_DISABLE_WIDGET_IF(uiCode, disabledBool, body) if (disabledBool) { ui::PushItemFlag(ImGuiItemFlags_Disabled, true); ui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f); } if(uiCode)body if (disabledBool) { ImGui::PopItemFlag();ImGui::PopStyleVar(); }

// => Main Options Section
inline bool process_id_disabled = false;
inline int process_id = 83252;

inline bool use_kernal_disabled = false;
inline bool use_kernal;

inline bool g_objects_disabled = false;
inline bool g_names_disabled = false;
// => Main Options Section

// => Tabs
inline int cur_tap_id = 0;
// => Tabs

// => GObjects, GNames, Class
inline bool g_objects_find_disabled = false;
inline uintptr_t g_objects_address;
inline char g_objects_buf[17] = "270E4F60000";// { 0 };
inline std::vector<std::string> g_obj_listbox_items;
inline int g_obj_listbox_item_current = 0;

inline bool g_names_find_disabled = false;
inline uintptr_t g_names_address;
inline char g_names_buf[17] = "270E2680080";// { 0 };
inline std::vector<std::string> g_names_listbox_items;
inline int g_names_listbox_item_current = 0;

inline bool class_find_disabled = false;
inline bool class_find_input_disabled = false;
inline char class_find_buf[30] = { 0 };
inline std::vector<std::string> class_listbox_items;
inline int class_listbox_item_current = 0;
// => GObjects, GNames, Class

// => Instance Logger
inline bool il_start_disabled = false;
inline int il_objects_count = 0;
inline int il_names_count = 0;
inline std::string il_state = "Ready ..!!";
// => Instance Logger

// => Sdk Generator
inline bool sg_start_disabled = false;
inline bool sg_finished = false;

inline int sg_objects_count = 0;
inline int sg_names_count = 0;
inline int sg_packages_count = 0;
inline int sg_packages_done_count = 0;

inline bool sg_type_disabled = false;
inline std::vector<std::string> sg_type_items = { "Internal", "External" };
inline int sg_type_item_current = 0;

inline bool sg_game_name_disabled = false;
inline char sg_game_name_buf[30] = { 0 };

inline bool sg_game_version_disabled = false;
inline int sg_game_version[3] = { 1, 0, 0 };

inline std::string sg_state = "Ready ..!!";

inline std::vector<std::string> sg_packages_items;
inline int sg_packages_item_current = 0;
// => Sdk Generator

// => Help Functions
static void DisabledAll()
{
	process_id_disabled = true;
	use_kernal_disabled = true;

	g_objects_disabled = true;
	g_names_disabled = true;
	class_find_disabled = true;
	class_find_input_disabled = true;

	il_start_disabled = true;
	sg_start_disabled = true;
	sg_type_disabled = true;
	sg_game_name_disabled = true;
	sg_game_version_disabled = true;
}

static void EnabledAll()
{
	g_objects_disabled = false;
	g_names_disabled = false;
	class_find_disabled = false;
	class_find_input_disabled = false;

	il_start_disabled = false;
	sg_start_disabled = false;

	sg_type_disabled = false;
	sg_game_name_disabled = false;
	sg_game_version_disabled = false;
}

static bool VectorGetter(void* vec, const int idx, const char** out_text)
{
	auto& vector = *static_cast<std::vector<std::string>*>(vec);
	if (idx < 0 || idx >= static_cast<int>(vector.size())) { return false; }
	*out_text = vector.at(idx).c_str();
	return true;
};

static void HelpMarker(const char* desc)
{
	ui::TextDisabled("(?)");
	if (ui::IsItemHovered())
	{
		ui::BeginTooltip();
		ui::PushTextWrapPos(ui::GetFontSize() * 35.0f);
		ui::TextUnformatted(desc);
		ui::PopTextWrapPos();
		ui::EndTooltip();
	}
}

static void WarningPopup(const std::string& key, const std::string& message, const std::function<void()> okCallBack = nullptr)
{
	// If Not Valid Process, this Popup will show
	if (ui::BeginPopupModal((std::string("Warning##") + key).c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove))
	{
		ui::Text("%s", message.c_str());
		ui::Separator();

		if (ui::Button("Ok", ImVec2(200, 0)))
		{
			if (okCallBack)
				okCallBack();
			ui::CloseCurrentPopup();
		}
		ui::SetItemDefaultFocus();
		ui::EndPopup();
	}
}
