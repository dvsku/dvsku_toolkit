INCLUDE(FetchContent)

SET(FETCHCONTENT_QUIET OFF)
SET(FETCHCONTENT_UPDATES_DISCONNECTED ON)

###############################################################################
## libgui

FetchContent_Declare(
    libgui
    URL https://github.com/dvsku/libgui/archive/65b5c4d51f3428bbe292b77cb5f6e4be482800a6.zip
)

FetchContent_MakeAvailable(libgui)

###############################################################################
## libevp

SET(EVP_TEST OFF)

FetchContent_Declare(
    libevp
    URL https://github.com/dvsku/libevp/archive/refs/heads/main.zip
)

FetchContent_MakeAvailable(libevp)

SET(LIBEVP_INCLUDES "${libevp_SOURCE_DIR}/include" PARENT_SCOPE)