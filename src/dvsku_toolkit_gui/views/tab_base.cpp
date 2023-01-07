#include "tab_base.h"

#include "imgui.h"
#include "misc/cpp/imgui_stdlib.h"

#include "gui.h"

using namespace ImGui;

void dvsku::toolkit::views::tab_base::offset_draw(float x, float y) {
	auto cursor = GetCursorScreenPos();
	SetCursorPosX(cursor.x + x);
	SetCursorPosY(cursor.y - y);
}

///////////////////////////////////////////////////////////////////////////////
// HANDLERS
///////////////////////////////////////////////////////////////////////////////

void dvsku::toolkit::views::tab_base::handle_on_start() {
	GUI.set_disabled(true);
	GUI.set_taskbar_status(TBPF_NORMAL);
	GUI.set_taskbar_progress(0.0f);
	m_progress = 0.0f;
}

void dvsku::toolkit::views::tab_base::handle_on_finish_evp(dvsku::evp::evp_status status) {
	if (status == dvsku::evp::evp_status::ok) {
		m_progress = 100.0f;
		m_input = "";
		m_output = "";
		m_key = "";
	}
	else if (status == dvsku::evp::evp_status::cancelled)
		m_progress = 0.0f;

	GUI.set_disabled(false);
	GUI.set_taskbar_status(TBPF_NOPROGRESS);
	GUI.set_taskbar_progress(0.0f);
}

void dvsku::toolkit::views::tab_base::handle_on_finish_crypt(crypt_result result) {
	if (result == CRYPT_OK) {
		m_progress = 100.0f;
		m_input = "";
		m_output = "";
		m_key = "";
	}
	else if (result == CRYPT_CANCELLED)
		m_progress = 0.0f;

	GUI.set_disabled(false); 
	GUI.set_taskbar_status(TBPF_NOPROGRESS);
	GUI.set_taskbar_progress(0.0f);
}

void dvsku::toolkit::views::tab_base::handle_on_update(float progress) {
	m_progress += progress;
	GUI.set_taskbar_progress(m_progress);
}

void dvsku::toolkit::views::tab_base::handle_on_error_evp(dvsku::evp::evp_result status) {
	GUI.set_disabled(false);
	GUI.set_taskbar_status(TBPF_ERROR);
}

void dvsku::toolkit::views::tab_base::handle_on_error_crypt(crypt_result result) {
	GUI.set_disabled(false);
	GUI.set_taskbar_status(TBPF_ERROR);
}