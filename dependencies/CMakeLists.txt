INCLUDE(FetchContent)

SET(FETCHCONTENT_QUIET OFF)
SET(FETCHCONTENT_UPDATES_DISCONNECTED ON)

###############################################################################
## libgui

FetchContent_Declare(
    libgui
    URL https://github.com/dvsku/libgui/archive/ba73daa88bccbc5b281aa7d634ce2e1c67b1380f.zip
)

FetchContent_MakeAvailable(libgui)

###############################################################################
## libevp

FetchContent_Declare(
    libevp
    URL https://github.com/dvsku/libevp/archive/refs/tags/v1.1.1.zip
)

FetchContent_MakeAvailable(libevp)
