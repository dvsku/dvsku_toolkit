#pragma once

#include <cstdint>

#define DT_TO_STRING_1(s) #s
#define DT_TO_STRING_2(s) DT_TO_STRING_1(s)

#if _WIN64
#define DT_ENV64
#define DT_ENV_STR "64-bit"
#else
#define DT_ENV32
#define DT_ENV_STR "32-bit"
#endif

#define DT_APP_NAME "dvsku toolkit"
#define DT_WEBSITE  "https://github.com/dvsku/dvsku_toolkit"

#define DT_VERSION_MAJOR 1
#define DT_VERSION_MINOR 3
#define DT_VERSION_REV   0

#define DT_VERSION_PACKED ((uint64_t)DT_VERSION_MAJOR << 48) | (((uint64_t)DT_VERSION_MINOR << 32)) | (((uint64_t)DT_VERSION_REV << 16))

#define DT_VERSION     DT_VERSION_MAJOR, DT_VERSION_MINOR, DT_VERSION_REV, 0
#define DT_VERSION_STR DT_TO_STRING_2(DT_VERSION_MAJOR) "." DT_TO_STRING_2(DT_VERSION_MINOR) "." DT_TO_STRING_2(DT_VERSION_REV) ".0"
