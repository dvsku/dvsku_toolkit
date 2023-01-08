#include "gui.h"

#include <versionhelpers.h>

#include "definitions.h"

#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl2.h"

#include "glfw/glfw3native.h"

#include "utilities/utilities_color.h"
#include "utilities/utilities_file_dialog.h"
#include "utilities/utilities_imgui.h"

#include "lib/libevp/libevp.h"

#pragma comment(lib, "lib/libevp/libevp.lib")
#pragma comment(lib, "lib/libdvsku_crypt/libdvsku_crypt_md.lib")
#pragma comment(lib, "Winmm.lib")

#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
	#pragma comment(lib, "legacy_stdio_definitions")
#endif

using namespace dvsku::toolkit::utilities;
using namespace ImGui;

///////////////////////////////////////////////////////////////////////////////
// CONSTRUCTOR/DESTRUCTOR/GETTERS/SETTERS
///////////////////////////////////////////////////////////////////////////////

dvsku::toolkit::gui::gui() {}

dvsku::toolkit::gui::~gui() {
	cleanup_imgui();
	cleanup_glfw();
	cleanup_taskbar();
}

ImGuiIO& dvsku::toolkit::gui::get_io() {
	return ImGui::GetIO();
}

void dvsku::toolkit::gui::set_disabled(bool value) {
	m_disabled = value;
}

bool dvsku::toolkit::gui::is_disabled() {
	return m_disabled;
}

///////////////////////////////////////////////////////////////////////////////
// SETUP/CLEANUP
///////////////////////////////////////////////////////////////////////////////

void dvsku::toolkit::gui::start(uint32_t width, uint32_t height) {
	if (m_initialized) return;

	setup_glfw();
	setup_imgui();
	setup_taskbar();

	m_initialized = true;

	run();
}

void dvsku::toolkit::gui::setup_glfw() {
	// setup glfw
	glfwSetErrorCallback(NULL);

	if (!glfwInit()) return;

	// setup window
	glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);		// remove title bar
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);		// window cannot be resized

	// create window
	m_window = glfwCreateWindow(MAIN_WINDOW_WIDTH, MAIN_WINDOW_HEIGHT, "dvsku toolkit", NULL, NULL);

	if (m_window == NULL) return;

	// set taskbar icon
	SetClassLongPtr(glfwGetWin32Window(m_window), GCLP_HICON, (LONG_PTR)LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(101)));

	glfwMakeContextCurrent(m_window);
	glfwSwapInterval(1);				// enable vsync

	// set window size and position
	glfwSetWindowMonitor(m_window, NULL, (GetSystemMetrics(SM_CXSCREEN) / 2) - (MAIN_WINDOW_WIDTH / 2),
		(GetSystemMetrics(SM_CYSCREEN) / 2) - (MAIN_WINDOW_HEIGHT / 2), MAIN_WINDOW_WIDTH, MAIN_WINDOW_HEIGHT, GLFW_DONT_CARE);
}

void dvsku::toolkit::gui::setup_imgui() {
	// setup imgui
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.IniFilename = NULL;

	// set theme to dark
	ImGui::StyleColorsDark();

	// set rendering to glfw
	ImGui_ImplGlfw_InitForOpenGL(m_window, true);
	ImGui_ImplOpenGL2_Init();
}

void dvsku::toolkit::gui::setup_taskbar() {
	if (!IsWindows7OrGreater()) return;

	HRESULT result = CoInitialize(NULL);

	if ((result != S_OK) && (result != S_FALSE)) return;

	ITaskbarList3* temp = NULL;
	result = CoCreateInstance(CLSID_TaskbarList, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&temp));

	if (!SUCCEEDED(result)) return;

	result = temp->HrInit();

	if (!SUCCEEDED(result)) return;

	m_taskbar = temp;
}

void dvsku::toolkit::gui::cleanup_glfw() {
	glfwDestroyWindow(m_window);
	glfwTerminate();
}

void dvsku::toolkit::gui::cleanup_imgui() {
	ImGui_ImplOpenGL2_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void dvsku::toolkit::gui::cleanup_taskbar() {
	if (m_taskbar == nullptr) return;
	
	m_taskbar->Release();
	m_taskbar = nullptr;
}

///////////////////////////////////////////////////////////////////////////////
// MAIN LOOP
///////////////////////////////////////////////////////////////////////////////

void dvsku::toolkit::gui::run() {
	while (!glfwWindowShouldClose(m_window)) {
		create_new_frame();
		build_gui();
		render_new_frame();
	}
}

void dvsku::toolkit::gui::create_new_frame() {
	glfwPollEvents();

	handle_window_move();

	ImGui_ImplOpenGL2_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void dvsku::toolkit::gui::render_new_frame() {
	ImGui::Render();

	int display_w, display_h;
	glfwGetFramebufferSize(m_window, &display_w, &display_h);
	glViewport(0, 0, display_w, display_h);

	glClearColor(0.45f * 1.00f, 0.55f * 1.00f, 0.60f * 1.00f, 1.00f);
	glClear(GL_COLOR_BUFFER_BIT);

	ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());

	glfwMakeContextCurrent(m_window);
	glfwSwapBuffers(m_window);
}

///////////////////////////////////////////////////////////////////////////////
// GUI
///////////////////////////////////////////////////////////////////////////////

void dvsku::toolkit::gui::build_gui() {
	build_title_bar();
	build_content_window();
}

void dvsku::toolkit::gui::build_tabs() {
	if (start_tab_bar("Tabs", ImGuiTabBarFlags_None, 5, 5)) {
		if (start_tab_item("Pack EVP", 0, 5, 5)) {
			m_pack.build();
			end_tab_item();
		}
		if (start_tab_item("Unpack EVP", 0, 5, 5)) {
			m_unpack.build();
			end_tab_item();
		}
		if (start_tab_item("Encrypt", 0, 5, 5)) {
			m_encrypt.build();
			end_tab_item();
		}
		if (start_tab_item("Decrypt", 0, 5, 5)) {
			m_decrypt.build();
			end_tab_item();
		}
		end_tab_bar();
	}
}

void dvsku::toolkit::gui::build_title_bar() {
	begin_window("TitleBar", ImGuiWindowFlags_NoTitleBar | 
		ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBringToFrontOnFocus, 
		0, 0, MAIN_WINDOW_WIDTH, 25, "#FF1A1A1A", 0, 0);

	m_can_drag = is_window_hovered();

	draw_text("dvsku toolkit", 15, (25 - CalcTextSize("dvsku toolkit").y) * 0.5f, false);

	set_color(ImGuiCol_Button, "#00FFFFFF");
	set_color(ImGuiCol_ButtonHovered, "#00FFFFFF");
	set_color(ImGuiCol_ButtonActive, "#00FFFFFF");

	if (!m_minimize_hover && !m_minimize_active)
		set_color(ImGuiCol_Text, "#88FFFFFF");
	else
		set_color(ImGuiCol_Text, "#FFFFFFFF");

	if (draw_button("-", MAIN_WINDOW_WIDTH - 55, 0, 25, 25, false)) {
		glfwIconifyWindow(m_window);
	}

	m_minimize_hover = is_element_hovered();
	m_minimize_active = is_element_active();

	unset_colors(1);

	if (!m_close_hover && !m_close_active)
		set_color(ImGuiCol_Text, "#88FFFFFF");
	else
		set_color(ImGuiCol_Text, "#FFFFFFFF");

	if (draw_button("X", MAIN_WINDOW_WIDTH - 30, 0, 25, 25, false)) {
		glfwSetWindowShouldClose(m_window, 1);
	}

	m_close_hover = is_element_hovered();
	m_close_active = is_element_active();

	unset_colors(4);

	end_window();

	unset_colors(1);
	unset_vars(1);
}

void dvsku::toolkit::gui::build_content_window() {
	begin_window("ContentWindow", ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize, 0, 25,
		MAIN_WINDOW_WIDTH, MAIN_WINDOW_HEIGHT - 25, "#FF1F1F1F", 15, 10);

	if (m_disabled)
		begin_disabled();

	build_tabs();

	if (m_disabled)
		end_disabled();

	end_window();
}

///////////////////////////////////////////////////////////////////////////////
// HANDLERS
///////////////////////////////////////////////////////////////////////////////

void dvsku::toolkit::gui::handle_window_move() {
	if (!m_can_drag) return;

	ImGuiIO& io = get_io();

	if (!io.MouseDown[0]) return;
	
	tagPOINT point;
	GetCursorPos(&point);

	glfwSetWindowPos(m_window, point.x - (int)ceil(io.MouseClickedPos->x), point.y - (int)ceil(io.MouseClickedPos->y));
}

///////////////////////////////////////////////////////////////////////////////
// TASKBAR
///////////////////////////////////////////////////////////////////////////////

void dvsku::toolkit::gui::set_taskbar_status(TBPFLAG flags) {
	if (m_window == nullptr) return;

	HWND handle = glfwGetWin32Window(m_window);

	if (handle == NULL) return;

	m_taskbar->SetProgressState(handle, flags);
}

void dvsku::toolkit::gui::set_taskbar_progress(float value) {
	if (m_window == nullptr) return;

	HWND handle = glfwGetWin32Window(m_window);

	if (handle == NULL) return;

	m_taskbar->SetProgressValue(handle, (ULONGLONG)value, 100);
}