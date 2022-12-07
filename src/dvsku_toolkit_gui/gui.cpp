#include "gui.h"

#include "helpers/color.h"
#include "helpers/file_dialog.h"

dvsku::toolkit::gui::gui(ImGuiIO& io, GLFWwindow* window) : m_io(io), m_window(window) {
	m_root_win_size = ImVec2(MAIN_WINDOW_WIDTH, MAIN_WINDOW_HEIGHT);
}

void dvsku::toolkit::gui::handle_window_move() {
	if (!m_can_drag) return;
	if (!m_io.MouseDown[0]) return;
	
	tagPOINT point;
	GetCursorPos(&point);

	glfwSetWindowPos(m_window, point.x - m_io.MouseClickedPos->x, point.y - m_io.MouseClickedPos->y);
}

void dvsku::toolkit::gui::build_gui() {
	build_title_bar();
	build_content_window();
}

void dvsku::toolkit::gui::build_tabs() {
	PushStyleVar(ImGuiStyleVar_TabRounding, 0.0f);
	
	PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(5, 5));
	ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
	if (BeginTabBar("Tabs", tab_bar_flags)) {
		PopStyleVar();

		PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(5, 5));
		if (BeginTabItem("Pack EVP")) {
			PopStyleVar();

			build_tab_pack();
			EndTabItem();
		}
		else {
			PopStyleVar();
		}

		PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(5, 5));
		if (BeginTabItem("Unpack EVP")) {
			PopStyleVar();

			build_tab_unpack();
			EndTabItem();
		}
		else {
			PopStyleVar();
		}

		PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(5, 5));
		if (BeginTabItem("Encrypt")) {
			PopStyleVar();

			build_tab_encrypt();
			EndTabItem();
		}
		else {
			PopStyleVar();
		}

		PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(5, 5));
		if (BeginTabItem("Decrypt")) {
			PopStyleVar();

			build_tab_decrypt();
			EndTabItem();
		}
		else {
			PopStyleVar();
		}

		EndTabBar();
	}

	PopStyleVar();
}

void dvsku::toolkit::gui::build_tab_pack() {
	PushStyleColor(ImGuiCol_FrameBg, ARGB2UINT("#FF383838"));
	PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 4));
	
	offset_draw(20, 20);
	Text("Input");
	
	BeginDisabled(true);
	
	SetNextItemWidth(MAIN_WINDOW_WIDTH - 70 - 130);
	offset_draw(20, 20);
	PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(5, 4));
	InputText("", m_pack_input, MAX_PATH, ImGuiInputTextFlags_ReadOnly);
	PopStyleVar();

	EndDisabled();
	
	SameLine();
	if (Button("Select##PackInput", ImVec2(125, 21))) {
		file_dialog::select_folder(m_pack_input);
	}

	offset_draw(20, 15);
	Text("Output");

	BeginDisabled(true);

	SetNextItemWidth(MAIN_WINDOW_WIDTH - 70 - 130);
	offset_draw(20, 20);
	PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(5, 4));
	InputText("", m_pack_output, MAX_PATH, ImGuiInputTextFlags_ReadOnly);
	PopStyleVar();
	
	EndDisabled();

	SameLine();
	if (Button("Select##PackOutput", ImVec2(125, 21))) {	
		file_dialog::save_file(m_pack_output);
	}

	offset_draw(20, 15);
	Checkbox("Encrypt", &m_pack_encrypt);

	if (m_pack_encrypt) {

		offset_draw(20, 15);
		Text("Encryption key");

		offset_draw(20, 20);
		SetNextItemWidth(MAIN_WINDOW_WIDTH - 65);
		PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(5, 4));
		InputText(" ", m_pack_key, 129, ImGuiInputTextFlags_CharsNoBlank);
		PopStyleVar();
	}

	offset_draw(20, 15);
	Text("Pack type");
	
	offset_draw(20, 20);
	RadioButton("any", &m_pack_type, evp_type::any); 
	if (IsItemHovered())
		SetTooltip("Pack all files from input");
	SameLine();

	RadioButton("client", &m_pack_type, evp_type::client); 
	if (IsItemHovered())
		SetTooltip("Pack only client files");
	SameLine();

	RadioButton("server", &m_pack_type, evp_type::server);
	if (IsItemHovered())
		SetTooltip("Pack only server files");

	offset_draw(20, 15);
	SetNextItemWidth(MAIN_WINDOW_WIDTH - 65);
	
	if (m_cancel)
		strcpy(m_progress_text, "Cancelled");
	else
		sprintf(m_progress_text, "%.2f%", m_pack_progress);
	
	if (m_disabled)
		EndDisabled();

	PushStyleColor(ImGuiCol_PlotHistogram, ARGB2UINT("#FF774F2D"));
	ProgressBar(m_pack_progress / 100, ImVec2(0.0f, 0.0f), m_progress_text);
	PopStyleColor();

	offset_draw(MAIN_WINDOW_WIDTH / 2 - 125/2 - 18, 15);

	if (!m_disabled) {
		if (Button("Pack", ImVec2(125, 21))) {
			m_cancel = false;
			dvsku::evp::pack_async(m_pack_input, m_pack_output, false, "", 
				dvsku::evp::file_filter::none, &m_cancel,
				[this]() {
					m_disabled = true;
					m_pack_progress = 0.0f;
				},
				[this](float progress) {
					m_pack_progress += progress;
				},
				[this](dvsku::evp::evp_status status) {
					if (status == dvsku::evp::evp_status::ok)
						m_pack_progress = 100.0f;
					else if (status == dvsku::evp::evp_status::cancelled)
						m_pack_progress = 0.0f;

					m_disabled = false;
				},
				[this](dvsku::evp::evp_result result) {
					m_disabled = false;
				}
			);
		}
	}
	else {
		if (Button("Cancel##Pack", ImVec2(125, 21))) {
			m_cancel = true;
;		}
	}
	
	if (m_disabled)
		BeginDisabled();

	PopStyleColor();
	PopStyleVar();
}

void dvsku::toolkit::gui::build_tab_unpack() {
	PushStyleColor(ImGuiCol_FrameBg, ARGB2UINT("#FF383838"));
	PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 4));

	offset_draw(20, 20);
	Text("Input");

	BeginDisabled(true);

	SetNextItemWidth(MAIN_WINDOW_WIDTH - 70 - 130);
	offset_draw(20, 20);
	PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(5, 4));
	InputText("", m_unpack_input, MAX_PATH, ImGuiInputTextFlags_ReadOnly);
	PopStyleVar();

	EndDisabled();

	SameLine();
	if (Button("Select##UnpackInput", ImVec2(125, 21))) {
		file_dialog::open_file(m_unpack_input);
	}

	offset_draw(20, 15);
	Text("Output");

	BeginDisabled(true);

	SetNextItemWidth(MAIN_WINDOW_WIDTH - 70 - 130);
	offset_draw(20, 20);
	PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(5, 4));
	InputText("", m_unpack_output, MAX_PATH, ImGuiInputTextFlags_ReadOnly);
	PopStyleVar();

	EndDisabled();

	SameLine();
	if (Button("Select##UnpackOutput", ImVec2(125, 21))) {
		file_dialog::select_folder(m_unpack_output);
	}

	offset_draw(20, 15);
	Checkbox("Decrypt", &m_unpack_decrypt);

	if (m_unpack_decrypt) {

		offset_draw(20, 15);
		Text("Decryption key");

		offset_draw(20, 20);
		SetNextItemWidth(MAIN_WINDOW_WIDTH - 65);
		PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(5, 4));
		InputText(" ", m_unpack_key, 129, ImGuiInputTextFlags_CharsNoBlank);
		PopStyleVar();
	}

	offset_draw(20, 15);
	SetNextItemWidth(MAIN_WINDOW_WIDTH - 65);

	char buf[12];

	if (m_cancel)
		strcpy(m_progress_text, "Cancelled");
	else
		sprintf(m_progress_text, "%.2f%", m_unpack_progress);

	if (m_disabled)
		EndDisabled();

	PushStyleColor(ImGuiCol_PlotHistogram, ARGB2UINT("#FF774F2D"));
	ProgressBar(m_unpack_progress / 100, ImVec2(0.0f, 0.0f), m_progress_text);
	PopStyleColor();

	offset_draw(MAIN_WINDOW_WIDTH / 2 - 125 / 2 - 18, 15);

	if (!m_disabled) {
		if (Button("Unack", ImVec2(125, 21))) {
			m_cancel = false;
			dvsku::evp::unpack_async(m_unpack_input, m_unpack_output, false, "", 
				dvsku::evp::file_filter::none, &m_cancel,
				[this]() {
					m_disabled = true;
					m_unpack_progress = 0.0f;
				},
				[this](float progress) {
					m_unpack_progress += progress;
				},
				[this](dvsku::evp::evp_status status) {
					if (status == dvsku::evp::evp_status::ok)
						m_unpack_progress = 100.0f;
					else if (status == dvsku::evp::evp_status::cancelled)
						m_unpack_progress = 0.0f;

					m_disabled = false;
				},
				[this](dvsku::evp::evp_result result) {
					m_disabled = false;
				}
			);
		}
	}
	else {
		if (Button("Cancel##Unpack", ImVec2(125, 21))) {
			m_cancel = true;
		}
	}

	if (m_disabled)
		BeginDisabled();

	PopStyleColor();
	PopStyleVar();
}

void dvsku::toolkit::gui::build_tab_encrypt() {
	PushStyleColor(ImGuiCol_FrameBg, ARGB2UINT("#FF383838"));
	PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 4));

	offset_draw(20, 20);
	Text("Input");

	BeginDisabled(true);

	SetNextItemWidth(MAIN_WINDOW_WIDTH - 70 - 130);
	offset_draw(20, 20);
	PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(5, 4));
	InputText("", m_encrypt_input, MAX_PATH, ImGuiInputTextFlags_ReadOnly);
	PopStyleVar();

	EndDisabled();

	SameLine();
	if (Button("Select##EncryptInput", ImVec2(125, 21))) {
		file_dialog::select_folder(m_encrypt_input);
	}

	offset_draw(20, 15);
	Text("Output");

	BeginDisabled(true);

	SetNextItemWidth(MAIN_WINDOW_WIDTH - 70 - 130);
	offset_draw(20, 20);
	PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(5, 4));
	InputText("", m_encrypt_output, MAX_PATH, ImGuiInputTextFlags_ReadOnly);
	PopStyleVar();

	EndDisabled();

	SameLine();
	if (Button("Select##EncryptOutput", ImVec2(125, 21))) {
		file_dialog::select_folder(m_encrypt_output);
	}

	offset_draw(20, 15);
	Text("Encryption key");

	offset_draw(20, 20);
	SetNextItemWidth(MAIN_WINDOW_WIDTH - 65);
	PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(5, 4));
	InputText(" ", m_encrypt_key, 129, ImGuiInputTextFlags_CharsNoBlank);
	PopStyleVar();

	offset_draw(20, 15);
	SetNextItemWidth(MAIN_WINDOW_WIDTH - 65);
	ProgressBar(0, ImVec2(0.0f, 0.0f));

	offset_draw(MAIN_WINDOW_WIDTH / 2 - 125 / 2 - 18, 15);
	Button("Encrypt", ImVec2(125, 21));

	PopStyleColor();
	PopStyleVar();
}

void dvsku::toolkit::gui::build_tab_decrypt() {
	PushStyleColor(ImGuiCol_FrameBg, ARGB2UINT("#FF383838"));
	PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 4));

	offset_draw(20, 20);
	Text("Input");

	BeginDisabled(true);

	SetNextItemWidth(MAIN_WINDOW_WIDTH - 70 - 130);
	offset_draw(20, 20);
	PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(5, 4));
	InputText("", m_decrypt_input, MAX_PATH, ImGuiInputTextFlags_ReadOnly);
	PopStyleVar();

	EndDisabled();

	SameLine();
	if (Button("Select##DecryptInput", ImVec2(125, 21))) {
		file_dialog::select_folder(m_decrypt_input);
	}

	offset_draw(20, 15);
	Text("Output");

	BeginDisabled(true);

	SetNextItemWidth(MAIN_WINDOW_WIDTH - 70 - 130);
	offset_draw(20, 20);
	PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(5, 4));
	InputText("", m_decrypt_output, MAX_PATH, ImGuiInputTextFlags_ReadOnly);
	PopStyleVar();

	EndDisabled();

	SameLine();
	if (Button("Select##DecryptOutput", ImVec2(125, 21))) {
		file_dialog::select_folder(m_decrypt_output);
	}

	offset_draw(20, 15);
	Text("Decryption key");

	offset_draw(20, 20);
	SetNextItemWidth(MAIN_WINDOW_WIDTH - 65);
	PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(5, 4));
	InputText(" ", m_decrypt_key, 129, ImGuiInputTextFlags_CharsNoBlank);
	PopStyleVar();

	offset_draw(20, 15);
	SetNextItemWidth(MAIN_WINDOW_WIDTH - 65);
	ProgressBar(0, ImVec2(0.0f, 0.0f));

	offset_draw(MAIN_WINDOW_WIDTH / 2 - 125 / 2 - 18, 15);
	Button("Decrypt", ImVec2(125, 21));

	PopStyleColor();
	PopStyleVar();
}

void dvsku::toolkit::gui::build_title_bar() {
	SetNextWindowPos(ImVec2(0, 0));
	SetNextWindowSize(ImVec2(MAIN_WINDOW_WIDTH, 25));

	PushStyleColor(ImGuiCol_WindowBg, ARGB2UINT("#FF1A1A1A"));
	PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0,0));
	
	Begin("TitleBar", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBringToFrontOnFocus);
	m_can_drag = IsWindowHovered();

	auto text_size = CalcTextSize("dvsku toolkit");

	SetCursorPosX(15);
	SetCursorPosY((25 - text_size.y) * 0.5f);

	Text("dvsku toolkit");
	
	PushStyleColor(ImGuiCol_Button, ARGB2UINT("#00FFFFFF"));
	PushStyleColor(ImGuiCol_ButtonHovered, ARGB2UINT("#00FFFFFF"));
	PushStyleColor(ImGuiCol_ButtonActive, ARGB2UINT("#00FFFFFF"));

	if (!m_close_hover && !m_close_active)
		PushStyleColor(ImGuiCol_Text, ARGB2UINT("#88FFFFFF"));
	else 
		PushStyleColor(ImGuiCol_Text, ARGB2UINT("#FFFFFFFF"));

	SetCursorPosX(MAIN_WINDOW_WIDTH - 25);
	SetCursorPosY(0);

	if (Button("X", ImVec2(25, 25))) {
		glfwSetWindowShouldClose(m_window, 1);
	}

	m_close_hover = IsItemHovered();
	m_close_active = IsItemActive();

	PopStyleColor(4);

	End();

	PopStyleColor();
	PopStyleVar(1);
}

void dvsku::toolkit::gui::build_content_window() {
	SetNextWindowPos(ImVec2(0, 25));
	SetNextWindowSize(ImVec2(MAIN_WINDOW_WIDTH, MAIN_WINDOW_HEIGHT - 25));

	PushStyleColor(ImGuiCol_WindowBg, ARGB2UINT("#FF1F1F1F"));
	PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(15, 10));

	Begin("ContentWindow", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);
	PopStyleVar(1);

	if (m_disabled)
		BeginDisabled();

	build_tabs();

	if (m_disabled)
		EndDisabled();

	End();

	PopStyleColor();
}

void dvsku::toolkit::gui::offset_draw(float x, float y) {
	auto cursor = GetCursorScreenPos();
	SetCursorPosX(cursor.x + x);
	SetCursorPosY(cursor.y - y);
}
