#pragma once

#ifndef DVSKU_TOOLKIT_CORE_H
#define DVSKU_TOOLKIT_CORE_H

#include "imgui.h"
#include <GLFW/glfw3.h>
#include <windows.h>

#include "definitions.h"
#include "lib/libevp/libevp.h"

#include "views/tab_pack.h"
#include "views/tab_unpack.h"
#include "views/tab_encrypt.h"
#include "views/tab_decrypt.h"

#pragma comment(lib, "lib/libevp/libevp.lib")
#pragma comment(lib, "lib/libdvsku_crypt/libdvsku_crypt.lib")

using namespace ImGui;

namespace dvsku::toolkit {
	class gui {
		protected:
			bool m_disabled = false;

			bool m_can_drag = false;

			bool m_close_hover	= false;
			bool m_close_active = false;

			dvsku::toolkit::views::tab_pack m_pack;
			dvsku::toolkit::views::tab_unpack m_unpack;
			dvsku::toolkit::views::tab_encrypt m_encrypt;
			dvsku::toolkit::views::tab_decrypt m_decrypt;
		
			ImVec2 m_root_win_size;

			ImGuiIO& m_io;
			GLFWwindow* m_window;
		public:
			gui(ImGuiIO& io, GLFWwindow* window);

			void build_gui();
			void handle_window_move();

		protected:
			void build_tabs();

			void build_title_bar();
			void build_content_window();
	};
}

#endif