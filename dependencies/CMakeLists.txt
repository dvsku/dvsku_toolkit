INCLUDE(FetchContent)

SET(FETCHCONTENT_QUIET OFF)
SET(FETCHCONTENT_UPDATES_DISCONNECTED ON)

###############################################################################
## dv_gui_opengl

FetchContent_Declare(
    dv_gui_opengl
    URL https://github.com/dvsku/dv_gui_opengl/archive/3d706e2009e308508ce5a04d72805464a48af518.zip
)

FetchContent_MakeAvailable(dv_gui_opengl)

SET(DV_GUI_INCLUDES "${dv_gui_opengl_SOURCE_DIR}/include" PARENT_SCOPE)

###############################################################################
## libevp

SET(EVP_TEST OFF)

FetchContent_Declare(
    libevp
    URL https://github.com/dvsku/libevp/archive/refs/heads/main.zip
)

FetchContent_MakeAvailable(libevp)

SET(LIBEVP_INCLUDES "${libevp_SOURCE_DIR}/include" PARENT_SCOPE)