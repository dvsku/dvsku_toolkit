INCLUDE(FetchContent)

SET(FETCHCONTENT_QUIET OFF)
SET(FETCHCONTENT_UPDATES_DISCONNECTED ON)

###############################################################################
## libgui

FetchContent_Declare(
    libgui
    URL https://github.com/dvsku/libgui/archive/refs/heads/master.zip
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