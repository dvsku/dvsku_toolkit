#include "tab_encrypt.h"

#include "imgui.h"
#include "misc/cpp/imgui_stdlib.h"

#include "gui.h"

#include "utilities/utilities_color.h"
#include "utilities/utilities_file_dialog.h"

#include "lib/libdvsku_crypt/libdvsku_crypt.h"

using namespace dvsku::toolkit::utilities;
using namespace ImGui;

dvsku::toolkit::views::tab_encrypt::tab_encrypt() : tab_base(), m_crypt("") {}

void dvsku::toolkit::views::tab_encrypt::build() {
	PushStyleColor(ImGuiCol_FrameBg, ARGB2UINT("#FF383838"));
	PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 4));

	offset_draw(20, 20);
	Text("Input");

	BeginDisabled(true);

	SetNextItemWidth(MAIN_WINDOW_WIDTH - 70 - 130);
	offset_draw(20, 20);
	PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(5, 4));
	InputText("", &m_input, ImGuiInputTextFlags_ReadOnly);
	PopStyleVar();

	EndDisabled();

	SameLine();
	if (Button("Select##EncryptInput", ImVec2(125, 21))) {
		file_dialog::select_folder(m_input);
	}

	offset_draw(20, 15);
	Checkbox("Encrypt to folder", &m_encrypt_to_folder);
	if (IsItemHovered())
		SetTooltip("If checked -> encrypts files and saves them to output folder");

	if (m_encrypt_to_folder) {
		offset_draw(20, 15);
		Text("Output");

		BeginDisabled(true);

		SetNextItemWidth(MAIN_WINDOW_WIDTH - 70 - 130);
		offset_draw(20, 20);
		PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(5, 4));
		InputText("", &m_output, ImGuiInputTextFlags_ReadOnly);
		PopStyleVar();

		EndDisabled();

		SameLine();
		if (Button("Select##EncryptOutput", ImVec2(125, 21))) {
			file_dialog::select_folder(m_output);
		}
	}

	offset_draw(20, 15);
	Text("Pack type");

	offset_draw(20, 20);
	RadioButton("any", &m_file_filter, (int)(FILE_FILTER_NONE));
	if (IsItemHovered())
		SetTooltip("Encrypt all files from input");
	SameLine();

	RadioButton("client", &m_file_filter, (int)(FILE_FILTER_CLIENT));
	if (IsItemHovered())
		SetTooltip("Encrypt only client files");
	SameLine();

	RadioButton("server", &m_file_filter, (int)(FILE_FILTER_SERVER));
	if (IsItemHovered())
		SetTooltip("Encrypt only server files");

	offset_draw(20, 15);
	Text("Encryption key");

	offset_draw(20, 20);
	SetNextItemWidth(MAIN_WINDOW_WIDTH - 65);
	PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(5, 4));
	InputText(" ", &m_key, ImGuiInputTextFlags_CharsNoBlank);
	PopStyleVar();

	offset_draw(20, 15);
	SetNextItemWidth(MAIN_WINDOW_WIDTH - 65);

	if (m_cancel)
		strcpy(m_progress_text, "Cancelled");
	else
		sprintf(m_progress_text, "%.2f%c", m_progress, '%');

	if (GUI.is_disabled())
		EndDisabled();

	PushStyleColor(ImGuiCol_PlotHistogram, ARGB2UINT("#FF774F2D"));
	ProgressBar(m_progress / 100, ImVec2(0.0f, 0.0f), m_progress_text);
	PopStyleColor();

	offset_draw(MAIN_WINDOW_WIDTH / 2 - 125 / 2 - 18, 15);

	bool cannot_start = m_input.empty() || (m_encrypt_to_folder && m_output.empty()) || m_key.empty();

	if (cannot_start)
		BeginDisabled();

	if (!GUI.is_disabled()) {
		if (Button("Encrypt", ImVec2(125, 21))) {
			m_cancel = false;

			m_crypt.set_key(m_key.c_str());
			m_crypt.encrypt_folder_async(m_input, m_output,
				(dvsku::filesys::utilities::file_filter)m_file_filter, &m_cancel,
				std::bind(&tab_base::handle_on_start, this),
				std::bind(&tab_base::handle_on_update, this, std::placeholders::_1),
				std::bind(&tab_base::handle_on_finish_crypt, this, std::placeholders::_1),
				std::bind(&tab_base::handle_on_error_crypt, this, std::placeholders::_1)
			);
		}
	}
	else {
		if (Button("Cancel##Encrypt", ImVec2(125, 21)))
			m_cancel = true;
	}

	if (cannot_start)
		EndDisabled();

	if (GUI.is_disabled())
		BeginDisabled();

	PopStyleColor();
	PopStyleVar();
}