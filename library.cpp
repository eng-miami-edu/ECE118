#include "clibrary.h"
#include "gpc.h"
#include "library.h"
#include <vector>

using namespace std;

int pen_color = 0x0000FFFF;
double pen_x = 0, pen_y = 0;

void set_pen_color(int c)
{ pen_color = c; }

void make_window(int w, int h, string text)
{ c_InitWindow(w, h, text.c_str());
  c_SetTargetFPS(60); }

bool running()
{ return ! c_WindowShouldClose(); }

void begin()
{ c_BeginDrawing(); }

void end()
{ c_EndDrawing(); }

void close_window()
{ c_CloseWindow(); }

struct shape
{ vector<Vector2> points; };

shape current_shape;

void start_shape()
{ current_shape.points.clear(); }

void add_point_to_shape(int x, int y)
{ Vector2 p;
  p.x = x;
  p.y = y;
  current_shape.points.push_back(p); }

void end_shape()
{ }

void fill_shape()
{ c_make_shape(current_shape.points.data(), current_shape.points.size()); 
  c_fill_shape(pen_color); }

void draw_shape()
{ int last = current_shape.points.size() - 1;
  for (int i = 0; i <= last; i += 1)
  { Vector2 a = current_shape.points[i];
    Vector2 b = current_shape.points[ i == last ? 0 : i ];
    c_draw_line(a.x, a.y, b.x, b.y, pen_color); } }

void move_to(double x, double y)
{ pen_x = x;
  pen_y = y; }

void draw_to(double x, double y)
{ c_draw_line((int)pen_x, (int)pen_y, (int)x, (int)y, pen_color);
  pen_x = x;
  pen_y = y; }
