
#ifndef ___clibrary_included
#define ___clibrary_included

#include "raylib.h"
#include "rlgl.h"
#include "gpc.h"

extern "C" void c_InitWindow(int w, int h, const char * text);
extern "C" void c_SetTargetFPS(int fps);
extern "C" int c_WindowShouldClose();
extern "C" void c_BeginDrawing();
extern "C" void c_EndDrawing();
extern "C" void c_CloseWindow();

extern "C" void c_draw_line(int x1, int y1, int x2, int y2, int color);

extern "C" void c_make_shape(Vector2 * pts, int n);
extern "C" void c_fill_shape(int color);

#endif
