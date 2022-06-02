
#ifndef ___library_included
#define ___library_included

#include <string>
#include <cmath>
#include <cstdio>
#include <iostream>
#include <iomanip>
#include "fonts.h"

using namespace std;

const double pi = acos(-1.0);
const double mul_d_to_r = pi / 180.0;
const double mul_r_to_d = 180.0 / pi;
const double twopi = 2 * pi;

typedef void * picture;
typedef void * sound;

struct color
{ static const int 
    black=0x000000FF,
    white=0xFFFFFFFF,
    grey=0xC0C0C0FF,
    light_grey=0xE0E0E0FF,
    dark_grey=0x909090FF,
    brown=0x8B4513FF,
    red=0xFF0000FF,
    green=0x00FF00FF,
    blue=0x0000FFFF,
    light_blue=0x90CEFFFF,
    yellow=0xFFFF00FF,
    magenta=0xFF00FFFF,
    cyan=0x00FFFFFF,
    pink=0xFFB6C1FF,
    orange=0xFF9400FF,
    dark_red=0xC00000FF,
    light_green=0x80FF80FF,
    light_red=0xFF8080FF,
    dark_green=0x008000FF,
    dark_blue=0x000090FF,
    indigo=0x483D8BFF,
    violet=0x700090FF,
    mauve=0x8A2BE2FF,
    purple=0xB71585FF,
    lime_green=0x9ACD32FF,
    sodium_d=0xFFD700FF; };

struct font
{ static const int
    times_new_roman = f_times_new_roman,
    times_new_roman_italic = f_times_new_roman_italic,
    times_new_roman_bold = f_times_new_roman_bold,
    arial = f_arial,
    courier_new = f_courier_new,
    courier_new_bold = f_courier_new_bold; };

struct anchor
{ static const int
    north_west = 0,
    south_west = 1,
    north_east = 2,
    south_east = 3,
    center = 4; };

void make_window(int w, int h, string text = "Graphics");
bool running();
void begin();
void run();
void end();
void close_window();
void set_window_position(int x, int y);
void get_window_position(int & x, int & y);
void set_window_title(string s);
void get_window_size(int & w, int & h);
void set_window_size(int w, int h);
void clear();

void pause(double t);

int make_color_int(int r, int g, int b);
int make_color(double r, double g, double b);
void split_color(int rgb, double & r, double & g, double & b);
void split_color_int(int rgb, int & r, int & g, int & b);
double antihls(double n1, double n2, double h);
void hls_to_rgb(double H, double L, double S, double & R, double & G, double & B);
void hls_to_rgb(double H, double L, double S, int & R, int & G, int & B);
int make_color_hls(double h, double l, double s);
void rgb_to_hls(double R, double G, double B, double & H, double & L, double & S);
void set_pen_color(int c);
void set_pen_color_int(int r, int g, int b);
void set_pen_color(double r, double g, double b);
void set_pen_color_hls(double h, double l, double s);
void set_pen_width(int w);
void set_pen_width_color(int w, int c);
void move_to(double x, double y);
void draw_to(double x, double y);
void get_position(int & x, int & y);
void get_position(double & x, double & y);
double get_x_position();
double get_y_position();
void set_pixel_color(double x, double y, int c);
int get_pixel_color(double x, double y);
void set_heading_radians(double d);
void set_heading_degrees(double d);
double get_heading_radians();
double get_heading_degrees();
double distance_to(double x, double y);
double direction_to_in_degrees(double x, double y);
double direction_from_to_in_degrees(double x0, double y0, double x1, double y1);
double direction_to_in_radians(double x, double y);
double direction_from_to_in_radians(double x0, double y0, double x1, double y1);
void turn_right_by_degrees(double d);
void turn_left_by_degrees(double d);
void turn_right_by_radians(double d);
void turn_left_by_radians(double d);
void move_distance(double d);
void draw_distance(double d);
void move_relative(double x, double y);
void draw_relative(double x, double y);
void draw_point(double x, double y);
void draw_point();
void fill_rectangle(double x, double y, double w, double h);
void fill_rectangle(double x, double y, double w, double h, int c);
picture load_picture(string fname);
void draw_picture(picture p, int x, int y, int w = -1, int h = -1);
int get_picture_width(picture p);
int get_picture_height(picture p);
void finished_with_picture(picture p);
bool check_wav_file(string fname);
sound load_sound(string fname);
void play_sound(sound s);
void finished_with_sound(sound s);

void start_shape();
void note_position(double x, double y);
void note_position();
void end_shape();
void draw_shape();
void fill_shape();

void set_font(int n);
void set_font_size(int n);
int get_font_size();
void draw_string(string s, bool update = false, int base = 0);
void draw_string(int n, bool update = false, int base = 0);
void draw_string(double d, bool update = false, int base = 0);
void draw_string(char c, bool update = false, int base = 0);
void measure_string(string s, int & w, int & h);
int string_to_int(string s);
string int_to_string(int i);

bool was_mouse_clicked(int & x, int & y);
int get_key_pressed();
char wait_for_mouse_clicked_or_key_pressed(int & x, int & y);
char wait_for_mouse_clicked_or_key_pressed_or_time(int & x, int & y, double t);

int get_clock_time();
int get_calendar_date();
void not_really_random();
int random_in_range(int a, int b);
void print(string s);
void print(const char * s);
void print(int n);
void print(char c);
void print(double d);
void new_line();
string read_string();
string read_line();
int read_int();
double read_double();
char read_char();
bool read_bool();

void experiment();
void end_experiment();

#endif
