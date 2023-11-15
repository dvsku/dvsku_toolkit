#include "tab_pack.h"

#include "imgui.h"
#include "misc/cpp/imgui_stdlib.h"

#include "gui.h"

#include "utilities/utilities_color.h"
#include "utilities/utilities_file_dialog.h"
#include "utilities/utilities_imgui.h"

using namespace dvsku::toolkit::utilities;

dvsku::toolkit::views::tab_pack::tab_pack() : tab_base() {}

void dvsku::toolkit::views::tab_pack::build() {
	set_frame_background("#FF383838");
	set_frame_padding(0, 4);

	draw_text("Input", 20, 20);
	draw_text_input("Input#Pack", &m_input, ImGuiInputTextFlags_ReadOnly, 20, 20, MAIN_WINDOW_WIDTH - 200, 4);

	if (draw_button("Select##PackInput", 0, 0, 125, 21)) {
		file_dialog::select_folder(m_input);
	}

	draw_text("Output", 20, 15);
	draw_text_input("Output#Pack",&m_output, ImGuiInputTextFlags_ReadOnly, 20, 20, MAIN_WINDOW_WIDTH - 200, 4);

	if (draw_button("Select##PackOutput", 0, 0, 125, 21)) {
		file_dialog::save_file(m_output);
	}

	draw_checkbox("Encrypt", &m_encrypt, 20, 15);

	if (m_encrypt) {
		draw_text("Encryption key", 20, 15);
		draw_text_input("Key#Pack", &m_key, ImGuiInputTextFlags_CharsNoBlank, 20, 20, MAIN_WINDOW_WIDTH - 65, 4);
	}

	draw_text("Pack type", 20, 15);

	draw_radio_button(20, 20, "any", &m_file_filter, (int)(FILE_FILTER_NONE), "Pack all files from input");
	draw_radio_button(0, 0, "client", &m_file_filter, (int)(FILE_FILTER_CLIENT), "Pack only client files");
	draw_radio_button(0, 0, "server", &m_file_filter, (int)(FILE_FILTER_SERVER), "Pack only server files");

	if (m_cancel)
		strcpy(m_progress_text, "Cancelled");
	else
		sprintf(m_progress_text, "%.2f%c", m_progress, '%');

	if (GUI.is_disabled())
		end_disabled();

	draw_progress_bar(m_progress / 100, m_progress_text, 20, 15, MAIN_WINDOW_WIDTH - 65, 0, "#FF774F2D");

	bool cannot_start = m_input.empty() || m_output.empty() || (m_encrypt && m_key.empty());

	if (cannot_start)
		begin_disabled();

	if (!GUI.is_disabled()) {
		if (draw_button("Pack", MAIN_WINDOW_WIDTH / 2 - 125 / 2 - 18, 15, 125, 21)) {
			m_cancel = false;
			dvsku::evp::pack_async(m_input, m_output, m_encrypt, m_key,
				(dvsku::filesys::utilities::file_filter)m_file_filter, &m_cancel,
				std::bind(&tab_base::handle_on_start, this),
				std::bind(&tab_base::handle_on_update, this, std::placeholders::_1),
				std::bind(&tab_base::handle_on_finish_evp, this, std::placeholders::_1),
				std::bind(&tab_base::handle_on_error_evp, this, std::placeholders::_1)
			);
		}
	}
	else {
		if(draw_button("Cancel##Pack", MAIN_WINDOW_WIDTH / 2 - 125 / 2 - 18, 15, 125, 21))
			m_cancel = true;
	}

	if (cannot_start)
		end_disabled();

	if (GUI.is_disabled())
		begin_disabled();

	unset_colors();
	unset_vars();
}