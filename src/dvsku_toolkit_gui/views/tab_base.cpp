#include "tab_base.h"

#include "imgui.h"
#include "misc/cpp/imgui_stdlib.h"

using namespace ImGui;

void dvsku::toolkit::views::tab_base::offset_draw(float x, float y) {
	auto cursor = GetCursorScreenPos();
	SetCursorPosX(cursor.x + x);
	SetCursorPosY(cursor.y - y);
}