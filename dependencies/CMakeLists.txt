INCLUDE(FetchContent)

SET(FETCHCONTENT_QUIET OFF)
SET(FETCHCONTENT_UPDATES_DISCONNECTED ON)

FetchContent_Declare(
    dv_gui_opengl
    GIT_REPOSITORY https://github.com/dvsku/dv_gui_opengl.git
    GIT_TAG master
)

FetchContent_MakeAvailable(dv_gui_opengl)

SET(DV_GUI_INCLUDES "${dv_gui_opengl_SOURCE_DIR}/include" PARENT_SCOPE)
