
#ifndef ___library_included
#define ___library_included

#include <string>

struct color
{ static const int 
    blue =    0x0000FFFF,
    red =     0xFF0000FF,
    green =   0x00FF00FF,
    yellow =  0xFFFF00FF,
    magenta = 0xFF00FFFF,
    cyan =    0x00FFFFFF,
    black =   0x000000FF; };

using namespace std;

void make_window(int w, int h, string text = "Graphics");
bool running();
void begin();
void end();
void close_window();

void set_pen_color(int c);
void move_to(double x, double y);
void draw_to(double x, double y);

void start_shape();
void add_point_to_shape(int x, int y);
void end_shape();
void draw_shape();
void fill_shape();

#endif
