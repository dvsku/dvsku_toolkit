#include "tab_pack.h"

#include "imgui.h"
#include "misc/cpp/imgui_stdlib.h"

#include "lib/libevp/libevp.h"

#include "utilities/utilities_color.h"
#include "utilities/utilities_file_dialog.h"

#include "gui.h"

using namespace dvsku::toolkit::utilities;
using namespace ImGui;

dvsku::toolkit::views::tab_unpack::tab_unpack() {}

void dvsku::toolkit::views::tab_unpack::build() {
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
	if (Button("Select##UnpackInput", ImVec2(125, 21))) {
		file_dialog::open_file(m_input);
	}

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
	if (Button("Select##UnpackOutput", ImVec2(125, 21))) {
		file_dialog::select_folder(m_output);
	}

	offset_draw(20, 15);
	Checkbox("Decrypt", &m_decrypt);

	if (m_decrypt) {
		offset_draw(20, 15);
		Text("Decryption key");

		offset_draw(20, 20);
		SetNextItemWidth(MAIN_WINDOW_WIDTH - 65);
		PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(5, 4));
		InputText(" ", &m_key, ImGuiInputTextFlags_CharsNoBlank);
		PopStyleVar();
	}

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

	bool cannot_start = m_input.empty() || m_output.empty() || (m_decrypt && m_key.empty());

	if (cannot_start)
		BeginDisabled();



	if (!dvsku::toolkit::gui::instance().is_disabled()) {
		if (Button("Unpack", ImVec2(125, 21))) {
			m_cancel = false;
			dvsku::evp::unpack_async(m_input, m_output, m_decrypt, m_key, &m_cancel,
				[this]() {
					GUI.set_disabled(true);
					m_progress = 0.0f;
				},
				[this](float progress) {
					m_progress += progress;
				},
					[this](dvsku::evp::evp_status status) {
					if (status == dvsku::evp::evp_status::ok) {
						m_progress = 100.0f;
						m_input = "";
						m_output = "";
						m_key = "";
					}
					else if (status == dvsku::evp::evp_status::cancelled)
						m_progress = 0.0f;

					GUI.set_disabled(false);
				},
					[this](dvsku::evp::evp_result result) {
					GUI.set_disabled(false);
				}
				);
		}
	}
	else {
		if (Button("Cancel##Unpack", ImVec2(125, 21)))
			m_cancel = true;
	}

	if (cannot_start)
		EndDisabled();

	if (GUI.is_disabled())
		BeginDisabled();

	PopStyleColor();
	PopStyleVar();
}