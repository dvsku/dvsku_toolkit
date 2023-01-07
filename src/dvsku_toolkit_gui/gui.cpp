#include "gui.h"

#include <shobjidl.h>
#include <versionhelpers.h>

#include "definitions.h"

#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl2.h"

#include "glfw/glfw3native.h"

#include "helpers/color.h"
#include "helpers/file_dialog.h"

#include "lib/libevp/libevp.h"

#pragma comment(lib, "lib/libevp/libevp.lib")
#pragma comment(lib, "lib/libdvsku_crypt/libdvsku_crypt_md.lib")

#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
	#pragma comment(lib, "legacy_stdio_definitions")
#endif

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
	PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));

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

///////////////////////////////////////////////////////////////////////////////
// HANDLERS
///////////////////////////////////////////////////////////////////////////////

void dvsku::toolkit::gui::handle_window_move() {
	if (!m_can_drag) return;

	ImGuiIO& io = get_io();

	if (!io.MouseDown[0]) return;
	
	tagPOINT point;
	GetCursorPos(&point);

	glfwSetWindowPos(m_window, point.x - (int)io.MouseClickedPos->x, point.y - (int)io.MouseClickedPos->y);
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