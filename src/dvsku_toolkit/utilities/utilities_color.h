#pragma once

#ifndef DVSKU_TOOLKIT_UTILITIES_COLOR_H
#define DVSKU_TOOLKIT_UTILITIES_COLOR_H

#include <string>

#define ARGB2UINT(argb) dvsku::toolkit::utilities::color_argb_to_uint(argb)

namespace dvsku::toolkit::utilities {
	static unsigned int color_argb_to_uint(const std::string& argb) {
		if (argb.size() != 9) return 0;
		
		if (argb[0] == '#')
			return std::stoul(argb.substr(1, 8), nullptr, 16);
		else
			return std::stoul(argb, nullptr, 16);
	}
}

#endif