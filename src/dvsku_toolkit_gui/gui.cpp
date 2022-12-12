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

			m_pack.build(&m_disabled);
			EndTabItem();
		}
		else {
			PopStyleVar();
		}

		PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(5, 5));
		if (BeginTabItem("Unpack EVP")) {
			PopStyleVar();

			m_unpack.build(&m_disabled);
			EndTabItem();
		}
		else {
			PopStyleVar();
		}

		PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(5, 5));
		if (BeginTabItem("Encrypt")) {
			PopStyleVar();

			m_encrypt.build(&m_disabled);
			EndTabItem();
		}
		else {
			PopStyleVar();
		}

		PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(5, 5));
		if (BeginTabItem("Decrypt")) {
			PopStyleVar();

			m_decrypt.build(&m_disabled);
			EndTabItem();
		}
		else {
			PopStyleVar();
		}

		EndTabBar();
	}

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
