INCLUDE(FetchContent)

SET(FETCHCONTENT_QUIET OFF)
SET(FETCHCONTENT_UPDATES_DISCONNECTED ON)

FetchContent_Declare(
    dv_gui_opengl
    URL https://github.com/dvsku/dv_gui_opengl/archive/95da8ed04189ce94223b4d87f9531a851c48992c.zip
)

FetchContent_MakeAvailable(dv_gui_opengl)

SET(DV_GUI_INCLUDES "${dv_gui_opengl_SOURCE_DIR}/include" PARENT_SCOPE)

SET(EVP_TEST OFF)

FetchContent_Declare(
    libevp
    URL https://github.com/dvsku/libevp/archive/refs/heads/main.zip
)

FetchContent_MakeAvailable(libevp)

SET(LIBEVP_INCLUDES "${libevp_SOURCE_DIR}/include" PARENT_SCOPE)