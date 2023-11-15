#pragma once

#ifndef DVSKU_TOOLKIT_TAB_DECRYPT_H
#define DVSKU_TOOLKIT_TAB_DECRYPT_H

#include "tab_base.h"

#include "libdvsku_crypt/libdvsku_crypt.h"

namespace dvsku::toolkit::views {
	class tab_decrypt : public tab_base {
		protected:
			bool m_decrypt_to_folder = false;

			dvsku::crypt::libdvsku_crypt m_crypt;

		public:
			tab_decrypt();

			void build() override;
	};
}

#endif