#pragma once

#ifndef DVSKU_TOOLKIT_TAB_UNPACK_H
#define DVSKU_TOOLKIT_TAB_UNPACK_H

#include "tab_base.h"

namespace dvsku::toolkit::views {
	class tab_unpack : public tab_base {
		protected:
			bool m_decrypt = false;

		public:
			tab_unpack();

			void build() override;
	};
}

#endif