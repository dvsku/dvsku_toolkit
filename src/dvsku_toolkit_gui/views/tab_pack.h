#pragma once

#ifndef DVSKU_TOOLKIT_TAB_PACK_H
#define DVSKU_TOOLKIT_TAB_PACK_H

#include "tab_base.h"

#include "lib/libevp/libevp.h"

namespace dvsku::toolkit::views {
	class tab_pack : public tab_base {
		protected:
			int m_file_filter = FILE_FILTER_NONE;

			bool m_encrypt = false;

		public:
			tab_pack();

			void build() override;
	};
}

#endif
