#pragma once

#ifndef DVSKU_TOOLKIT_TAB_BASE_H
#define DVSKU_TOOLKIT_TAB_BASE_H

#include <string>

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

		protected:
			void offset_draw(float x, float y);
	};
}

#endif