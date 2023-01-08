#include "tab_decrypt.h"

#include "imgui.h"
#include "misc/cpp/imgui_stdlib.h"

#include "gui.h"

#include "utilities/utilities_color.h"
#include "utilities/utilities_file_dialog.h"
#include "utilities/utilities_imgui.h"

using namespace dvsku::toolkit::utilities;

dvsku::toolkit::views::tab_decrypt::tab_decrypt() : tab_base(), m_crypt("") {}

void dvsku::toolkit::views::tab_decrypt::build() {
	set_frame_background("#FF383838");
	set_frame_padding(0, 4);

	draw_text("Input", 20, 20);
	draw_text_input("Input#Decrypt", &m_input, ImGuiInputTextFlags_ReadOnly, 20, 20, MAIN_WINDOW_WIDTH - 200, 4);

	if (draw_button("Select##DecryptInput", 0, 0, 125, 21)) {
		file_dialog::select_folder(m_input);
	}

	draw_checkbox("Decrypt to folder", &m_decrypt_to_folder, 20, 15, 
		"If checked -> decrypts files and saves them to output folder");

	if (m_decrypt_to_folder) {
		draw_text("Output", 20, 15);
		draw_text_input("Output#Decrypt", &m_output, ImGuiInputTextFlags_ReadOnly, 20, 20, MAIN_WINDOW_WIDTH - 200, 4);

		if (draw_button("Select##DecryptOutput", 0, 0, 125, 21)) {
			file_dialog::select_folder(m_output);
		}
	}

	draw_text("Decryption key", 20, 15);
	draw_text_input("Key#Decrypt", &m_key, ImGuiInputTextFlags_CharsNoBlank, 20, 20, MAIN_WINDOW_WIDTH - 65, 4);

	if (m_cancel)
		strcpy(m_progress_text, "Cancelled");
	else
		sprintf(m_progress_text, "%.2f%c", m_progress, '%');

	if (GUI.is_disabled())
		end_disabled();

	draw_progress_bar(m_progress / 100, m_progress_text, 20, 15, MAIN_WINDOW_WIDTH - 65, 0, "#FF774F2D");

	bool cannot_start = m_input.empty() || (m_decrypt_to_folder && m_output.empty()) || m_key.empty();

	if (cannot_start)
		begin_disabled();

	if (!GUI.is_disabled()) {
		if (draw_button("Decrypt", MAIN_WINDOW_WIDTH / 2 - 125 / 2 - 18, 15, 125, 21)) {
			m_cancel = false;

			m_crypt.set_key(m_key.c_str());
			m_crypt.decrypt_folder_async(m_input, m_output, &m_cancel,
				std::bind(&tab_base::handle_on_start, this),
				std::bind(&tab_base::handle_on_update, this, std::placeholders::_1),
				std::bind(&tab_base::handle_on_finish_crypt, this, std::placeholders::_1),
				std::bind(&tab_base::handle_on_error_crypt, this, std::placeholders::_1)
			);
		}
	}
	else {
		if (draw_button("Cancel##Decrypt", MAIN_WINDOW_WIDTH / 2 - 125 / 2 - 18, 15, 125, 21))
			m_cancel = true;
	}

	if (cannot_start)
		end_disabled();

	if (GUI.is_disabled())
		begin_disabled();

	unset_colors();
	unset_vars();
}