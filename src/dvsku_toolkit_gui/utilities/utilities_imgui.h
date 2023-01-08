#pragma once

#ifndef DVSKU_TOOLKIT_UTILITIES_IMGUI_H
#define DVSKU_TOOLKIT_UTILITIES_IMGUI_H

#include <windows.h>

#include "imgui.h"
#include "misc/cpp/imgui_stdlib.h"

#include "utilities_color.h"

namespace dvsku::toolkit::utilities {

	static void set_draw_position(float x, float y) {
		ImGui::SetCursorPosX(x);
		ImGui::SetCursorPosY(y);
	}

	static void set_draw_position_offset(float x, float y) {
		auto cursor = ImGui::GetCursorScreenPos();
		ImGui::SetCursorPosX(cursor.x + x);
		ImGui::SetCursorPosY(cursor.y - y);
	}

	static void set_color(ImGuiCol color, const char* value) {
		ImGui::PushStyleColor(color, ARGB2UINT(value));
	}

	static void set_var(ImGuiStyleVar var, float value) {
		ImGui::PushStyleVar(var, value);
	}

	static void unset_colors(int count = 1) {
		ImGui::PopStyleColor(count);
	}

	static void unset_vars(int count = 1) {
		ImGui::PopStyleVar(count);
	}

	static bool is_window_hovered() {
		return ImGui::IsWindowHovered();
	}

	static bool is_element_hovered() {
		return ImGui::IsItemHovered();
	}

	static bool is_element_active() {
		return ImGui::IsItemActive();
	}

	static void set_window_background(const char* color) {
		ImGui::PushStyleColor(ImGuiCol_WindowBg, ARGB2UINT(color));
	}

	static void set_window_padding(float x, float y) {
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(x, y));
	}

	static void set_frame_background(const char* color) {
		ImGui::PushStyleColor(ImGuiCol_FrameBg, ARGB2UINT(color));
	}

	static void set_frame_padding(float x, float y) {
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(x, y));
	}

	static void set_tab_rounding(float value) {
		ImGui::PushStyleVar(ImGuiStyleVar_TabRounding, value);
	}

	static void begin_window(const char* name, ImGuiWindowFlags flags = 0, 
		float x = 0, float y = 0, float w = 50, float h = 50, 
		const char* background = nullptr, float padding_x = 0, float padding_y = 0) 
	{
		ImGui::SetNextWindowPos(ImVec2(x, y));
		ImGui::SetNextWindowSize(ImVec2(w, h));
		
		if(background != nullptr)
			set_window_background(background);
		
		set_window_padding(padding_x, padding_y);

		ImGui::Begin(name, 0, flags);

		ImGui::PopStyleVar(1);

		if (background != nullptr)
			ImGui::PopStyleColor(1);
	}

	static void end_window() {
		ImGui::End();
	}

	static void begin_disabled(bool value = true) {
		ImGui::BeginDisabled(value);
	}

	static void end_disabled() {
		ImGui::EndDisabled();
	}

	static void draw_text(const char* text, float x = 0, float y = 0, bool relative = true) {
		if (x == 0 && y == 0)
			ImGui::SameLine();
		else
			if (relative)
				set_draw_position_offset(x, y);
			else
				set_draw_position(x, y);

		ImGui::Text(text);
	}

	static void draw_text_input(const char* label, std::string* binding, ImGuiInputTextFlags flags = 0, 
		float x = 0, float y = 0, float w = 0, float h = 0) 
	{
		if (x == 0 && y == 0)
			ImGui::SameLine();
		else
			set_draw_position_offset(x, y);

		ImGui::SetNextItemWidth(w);

		if(h > 0)
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(5, h));

		if ((flags & ImGuiInputTextFlags_ReadOnly) != 0)
			begin_disabled();

		ImGui::InputText(label, binding, flags);

		if ((flags & ImGuiInputTextFlags_ReadOnly) != 0)
			end_disabled();

		if (h > 0)
			ImGui::PopStyleVar();
	}

	static bool draw_button(const char* label, float x = 0, float y = 0, float w = 0, float h = 0, bool relative = true) {
		if (x == 0 && y == 0)
			ImGui::SameLine();
		else
			if (relative)
				set_draw_position_offset(x, y);
			else
				set_draw_position(x, y);

		return ImGui::Button(label, ImVec2(w, h));
	}

	static bool draw_checkbox(const char* label, bool* binding, float x = 0, float y = 0, 
		const char* tooltip = nullptr) 
	{
		if (x == 0 && y == 0)
			ImGui::SameLine();
		else
			set_draw_position_offset(x, y);
		
		bool result = ImGui::Checkbox(label, binding);
		if (ImGui::IsItemHovered() && tooltip != nullptr)
			ImGui::SetTooltip(tooltip);

		return result;
	}

	static bool draw_radio_button(float x, float y, const char* label, int* binding, int value, 
		const char* tooltip = nullptr) 
	{
		if (x == 0 && y == 0)
			ImGui::SameLine();
		else
			set_draw_position_offset(x, y);

		bool result = ImGui::RadioButton(label, binding, value);
		if (ImGui::IsItemHovered() && tooltip != nullptr)
			ImGui::SetTooltip(tooltip);

		return result;
	}

	static void draw_progress_bar(float value, const char* text = nullptr,
		float x = 0, float y = 0, float w = 0, float h = 0, const char* color = nullptr) 
	{
		if (x == 0 && y == 0)
			ImGui::SameLine();
		else
			set_draw_position_offset(x, y);

		ImGui::SetNextItemWidth(w);

		if (h > 0)
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, h));

		if (color != nullptr)
			ImGui::PushStyleColor(ImGuiCol_PlotHistogram, ARGB2UINT(color));

		ImGui::ProgressBar(value, ImVec2(0.0f, 0.0f), text);

		if (color != nullptr)
			ImGui::PopStyleVar();

		if (h > 0)
			ImGui::PopStyleVar();
	}

	static bool start_tab_bar(const char* id, ImGuiTabBarFlags flags, float padding_x = 0, float padding_y = 0) {
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(padding_x, padding_y));
		bool result = ImGui::BeginTabBar(id, flags);
		ImGui::PopStyleVar();
		return result;
	}

	static void end_tab_bar() {
		ImGui::EndTabBar();
	}

	static bool start_tab_item(const char* id, ImGuiTabItemFlags flags = 0, float padding_x = 0, float padding_y = 0) {
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(padding_x, padding_y));
		bool result = ImGui::BeginTabItem(id, 0, flags);
		ImGui::PopStyleVar();
		return result;
	}

	static void end_tab_item() {
		ImGui::EndTabItem();
	}
}

#endif