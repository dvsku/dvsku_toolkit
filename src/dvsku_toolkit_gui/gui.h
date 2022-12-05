#pragma once

#ifndef DVSKU_TOOLKIT_CORE_H
#define DVSKU_TOOLKIT_CORE_H

#include "imgui.h"
#include <GLFW/glfw3.h>
#include "definitions.h"
#include <windows.h>

using namespace ImGui;

namespace dvsku::toolkit {
	class gui {
		protected:
			enum evp_type : int {
				any = 0,
				client = 1,
				server = 2
			};

			bool m_can_drag = false;

			bool m_close_hover	= false;
			bool m_close_active = false;

			char m_pack_input[FILE_PATH_MAX]		= "";
			char m_pack_output[FILE_PATH_MAX]		= "";
			char m_unpack_input[FILE_PATH_MAX]		= "";
			char m_unpack_output[FILE_PATH_MAX]		= "";
			char m_encrypt_input[FILE_PATH_MAX]		= "";
			char m_encrypt_output[FILE_PATH_MAX]	= "";
			char m_decrypt_input[FILE_PATH_MAX]		= "";
			char m_decrypt_output[FILE_PATH_MAX]	= "";

			int m_pack_type = evp_type::any;
			
			bool m_pack_encrypt		= false;
			bool m_unpack_decrypt	= false;

			char m_pack_key[129]	= "";
			char m_unpack_key[129]	= "";
			char m_encrypt_key[129] = "";
			char m_decrypt_key[129] = "";
		
			ImVec2 m_root_win_size;

			ImGuiIO& m_io;
			GLFWwindow* m_window;
		public:
			gui(ImGuiIO& io, GLFWwindow* window);

			void build_gui();
			void handle_window_move();

		protected:
			void build_tabs();
			void build_tab_pack();
			void build_tab_unpack();
			void build_tab_encrypt();
			void build_tab_decrypt();

			void build_title_bar();
			void build_content_window();

			void offset_draw(float x, float y);
	};
}

#endif