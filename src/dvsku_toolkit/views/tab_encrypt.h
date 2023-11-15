#pragma once

#ifndef DVSKU_TOOLKIT_TAB_ENCRYPT_H
#define DVSKU_TOOLKIT_TAB_ENCRYPT_H

#include "tab_base.h"

#include "libdvsku_crypt/libdvsku_crypt.h"

namespace dvsku::toolkit::views {
	class tab_encrypt : public tab_base {
		protected:
			bool m_encrypt_to_folder = false;
			int m_file_filter = FILE_FILTER_NONE;

			dvsku::crypt::libdvsku_crypt m_crypt;

		public:
			tab_encrypt();

			void build() override;
	};
}

#endif