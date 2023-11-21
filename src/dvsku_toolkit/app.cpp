#include "app.hpp"

#include "glfw/glfw3native.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl2.h"

#include "fonts/font_roboto.hpp"
#include "fonts/font_fontawesome_solid.hpp"

using namespace dvsku_toolkit;

app::app(uint32_t width, uint32_t height) : m_components(m_systems) {
    // setup glfw
    glfwSetErrorCallback(NULL);

    if (!glfwInit()) return;

    // setup window
    glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);		// remove title bar
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);		// window cannot be resized

    // create window
    m_window = glfwCreateWindow(width, height, "dvsku toolkit", NULL, NULL);

    if (m_window == NULL) return;

    // set taskbar icon
    SetClassLongPtr(glfwGetWin32Window(m_window), GCLP_HICON, (LONG_PTR)LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(101)));

    glfwMakeContextCurrent(m_window);
    glfwSwapInterval(1);				// enable vsync

    // set window size and position
    glfwSetWindowMonitor(m_window, NULL, (GetSystemMetrics(SM_CXSCREEN) / 2) - (width / 2),
        (GetSystemMetrics(SM_CYSCREEN) / 2) - (height / 2), width, height, GLFW_DONT_CARE);

    // setup imgui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.IniFilename = NULL;

    ImFontConfig config;
    config.FontDataOwnedByAtlas = false;
    config.GlyphExtraSpacing.x  = 1.0f;

    // Add default font
    io.Fonts->AddFontFromMemoryTTF(ROBOTO_REGULAR, (int)ROBOTO_REGULAR_LEN, 14.0f, &config);
    
    // Add font awesome
    config.MergeMode = true;
    static const ImWchar icon_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
    io.Fonts->AddFontFromMemoryTTF(FONTAWESOME_SOLID, (int)FONTAWESOME_SOLID_LEN, 14.0f, &config, icon_ranges);
    
    // Build fonts
    io.Fonts->Build();

    // set theme to dark
    ImGui::StyleColorsDark();

    // set rendering to glfw
    ImGui_ImplGlfw_InitForOpenGL(m_window, true);
    ImGui_ImplOpenGL2_Init();

    //if (!IsWindows7OrGreater()) return;

    //HRESULT result = CoInitialize(NULL);

    //if ((result != S_OK) && (result != S_FALSE)) return;

    //ITaskbarList3* temp = NULL;
    //result = CoCreateInstance(CLSID_TaskbarList, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&temp));

    //if (!SUCCEEDED(result)) return;

    //result = temp->HrInit();

    //if (!SUCCEEDED(result)) return;

    //m_taskbar = temp;
}

app::~app() {
    ImGui_ImplOpenGL2_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    /*if (m_taskbar == nullptr) return;

    m_taskbar->Release();
    m_taskbar = nullptr;*/

    
    glfwTerminate();
}

void app::run() {
    while (!glfwWindowShouldClose(m_window)) {
        glfwPollEvents();

        //handle_window_move();

        ImGui_ImplOpenGL2_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        render();

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

    glfwDestroyWindow(m_window);
}

void app::render() {
    ImGui::PushStyleColor(ImGuiCol_WindowBg, 0xFF1F1F1F);

    ///////////////////////////////////////////////////////////////////////////
    // BEGIN ROOT WINDOW

    ImGui::SetNextWindowPos({ 0, 0 });
    ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize, ImGuiCond_Always);
    ImGui::Begin("Root##Window", 0, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);

    ///////////////////////////////////////////////////////////////////////////
    // BEGIN TITLEBAR

    ImGui::BeginChild("TitleBar", { 0.0f, 25.0f });

    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 5.0f);
    ImGui::Text("dvsku toolkit");

    ImGui::SameLine();

    ImVec2 avail = ImGui::GetContentRegionAvail();

    ImGui::PushStyleColor(ImGuiCol_Button, 0x00FFFFFF);
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, 0x00FFFFFF);
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, 0x00FFFFFF);

    if (!m_hovered_minimize && !m_active_minimize)
        ImGui::PushStyleColor(ImGuiCol_Text, 0x88FFFFFF);
    else
        ImGui::PushStyleColor(ImGuiCol_Text, 0xFFFFFFFF);

    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + avail.x - 45.0f);
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 3.0f);
    if (ImGui::Button(ICON_FA_MINUS"##MinimizeButton", { 20.0f, 15.0f })) {

    }

    m_hovered_minimize = ImGui::IsItemHovered();
    m_active_minimize  = ImGui::IsItemActive();

    ImGui::SameLine(0.0f, 5.0f);

    if (!m_hovered_close && !m_active_close)
        ImGui::PushStyleColor(ImGuiCol_Text, 0x88FFFFFF);
    else
        ImGui::PushStyleColor(ImGuiCol_Text, 0xFFFFFFFF);

    ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 3.0f);
    if (ImGui::Button(ICON_FA_XMARK"##CloseButton", { 20.0f, 15.0f })) {

    }

    m_hovered_close = ImGui::IsItemHovered();
    m_active_close  = ImGui::IsItemActive();

    ImGui::PopStyleColor(5);

    ImGui::EndChild();

    // END TITLEBAR
    ///////////////////////////////////////////////////////////////////////////

    ///////////////////////////////////////////////////////////////////////////
    // BEGIN VIEW

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 10.0f, 0.0f });

    ImGui::BeginChild("View", { 0.0f , 0.0f }, false, ImGuiWindowFlags_AlwaysUseWindowPadding);

    m_components.view.render();

    ImGui::EndChild();

    ImGui::PopStyleVar();

    // END VIEW
    ///////////////////////////////////////////////////////////////////////////

    ImGui::End();

    // END ROOT
    ///////////////////////////////////////////////////////////////////////////

    ImGui::PopStyleColor();
}
