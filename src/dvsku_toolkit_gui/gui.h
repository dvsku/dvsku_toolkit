#pragma once

#ifndef DVSKU_TOOLKIT_CORE_H
#define DVSKU_TOOLKIT_CORE_H

#include <windows.h>

#define STRICT_TYPED_ITEMIDS
#include <shlobj.h>

#include "definitions.h"

#define GLFW_EXPOSE_NATIVE_WIN32

#include "imgui.h"
#include "glfw/glfw3.h"

#include "views/tab_pack.h"
#include "views/tab_unpack.h"
#include "views/tab_encrypt.h"
#include "views/tab_decrypt.h"

#define GUI dvsku::toolkit::gui::instance()

namespace dvsku::toolkit {
	class gui {
		protected:
			GLFWwindow* m_window = nullptr;
			bool m_initialized = false;

			ITaskbarList3* m_taskbar = nullptr;

			bool m_disabled = false;

			bool m_can_drag = false;

			bool m_minimize_hover = false;
			bool m_minimize_active = false;
			bool m_close_hover	= false;
			bool m_close_active = false;

			dvsku::toolkit::views::tab_pack m_pack;
			dvsku::toolkit::views::tab_unpack m_unpack;
			dvsku::toolkit::views::tab_encrypt m_encrypt;
			dvsku::toolkit::views::tab_decrypt m_decrypt;
		
		private:
			gui();
			~gui();

			gui(gui const&) = delete;
			void operator=(gui const&) = delete;

		public:
			static gui& instance() {
				static gui instance;
				return instance;
			}

			void start(uint32_t width, uint32_t height);

			///////////////////////////////////////////////////////////////////
			// GETTERS/SETTERS
			///////////////////////////////////////////////////////////////////

			void set_disabled(bool value);

			bool is_disabled();

			///////////////////////////////////////////////////////////////////
			// TASKBAR
			///////////////////////////////////////////////////////////////////

			void set_taskbar_status(TBPFLAG flags);
			void set_taskbar_progress(float value);

		protected:

			///////////////////////////////////////////////////////////////////
			// GETTERS/SETTERS
			///////////////////////////////////////////////////////////////////

			ImGuiIO& get_io();

			///////////////////////////////////////////////////////////////////
			// SETUP/CLEANUP
			///////////////////////////////////////////////////////////////////

			void setup_glfw();
			void setup_imgui();
			void setup_taskbar();

			void cleanup_glfw();
			void cleanup_imgui();
			void cleanup_taskbar();

			///////////////////////////////////////////////////////////////////
			// MAIN LOOP
			///////////////////////////////////////////////////////////////////

			void run();

			void create_new_frame();
			void render_new_frame();

			///////////////////////////////////////////////////////////////////
			// GUI
			///////////////////////////////////////////////////////////////////

			void build_gui();

			void build_title_bar();
			void build_content_window();

			void build_tabs();

			///////////////////////////////////////////////////////////////////
			// HANDLERS
			///////////////////////////////////////////////////////////////////

			void handle_window_move();
	};
}

#endif