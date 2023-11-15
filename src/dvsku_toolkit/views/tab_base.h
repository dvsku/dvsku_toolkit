#pragma once

#ifndef DVSKU_TOOLKIT_TAB_BASE_H
#define DVSKU_TOOLKIT_TAB_BASE_H

#include <string>

#include "libevp/libevp.h"
#include "libdvsku_crypt/libdvsku_crypt.h"

namespace dvsku::toolkit::views {
	class tab_base {
		protected:
			bool m_cancel = false;

			std::string m_input = "";
			std::string m_output = "";

			std::string m_key = "";

			float m_progress = 0.0f;
			char m_progress_text[12] = "";

		public:
			virtual void build() = 0;

			///////////////////////////////////////////////////////////////////
			// HANDLERS
			///////////////////////////////////////////////////////////////////

			void handle_on_start();

			void handle_on_finish_evp(dvsku::evp::evp_status status);
			void handle_on_finish_crypt(crypt_result result);

			void handle_on_update(float progress);

			void handle_on_error_evp(dvsku::evp::evp_result status);
			void handle_on_error_crypt(crypt_result result);

		protected:
			void offset_draw(float x, float y);

	};
}

#endif