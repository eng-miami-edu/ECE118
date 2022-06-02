
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
extern "C" void c_set_window_position(int x, int y);
extern "C" void c_set_window_size(int w, int h);
extern "C" void c_get_window_position(int * x, int * y);
extern "C" void c_get_window_size(int * w, int * h);
extern "C" void c_set_window_title(const char * s);
extern "C" int c_get_dpi();

extern "C" void c_draw_line(int x1, int y1, int x2, int y2, int color, int width);
extern "C" void c_set_font(int n);
extern "C" void c_DrawText(const char * s, int x, int y, int fs, int color);
extern "C" void c_MeasureText(const char * s, int * w, int * h, int fs);
extern "C" void c_draw_circle(int x1, int y1, int r, int color);
extern "C" void c_fill_rectangle(int x, int y, int w, int h, int color);
extern "C" void c_set_pixel_color(int x, int y, int c);
extern "C" int c_get_pixel_color(int x, int y);
extern "C" void c_make_shape(Vector2 * pts, int n);
extern "C" void c_fill_shape(int color);
extern "C" void * c_load_picture(const char * fname);
extern "C" void c_draw_picture(void * i, int x, int y, int w, int h);
extern "C" int c_get_picture_width(void * i);
extern "C" int c_get_picture_height(void * i);
extern "C" void c_finished_with_picture(void * i);
extern "C" int c_check_wav_file(const char * fname);
extern "C" void * c_load_sound(const char * fname);
extern "C" void c_play_sound(void * s);
extern "C" void c_finished_with_sound(void * i);

extern "C" void c_init_image(int w, int h);
extern "C" void c_draw_image();
extern "C" void c_interactions();
extern "C" int c_was_mouse_clicked(int * x, int * y);
extern "C" int c_get_key_pressed();

extern "C" void c_experiment();
extern "C" void c_end_experiment();

#endif
