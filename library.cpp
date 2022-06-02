#include "clibrary.h"
#include "gpc.h"
#include "library.h"
#include <vector>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <sys/time.h>

#if _WIN32
  void srandomdev()
  { srand(time(NULL)); }

  int random()
  { return (unsigned short int)(rand()) ^ ((unsigned short int)(rand()) << 15); }
#elif __APPLE__
  
#else

#endif

using namespace std;

int pen_color = 0x000000FF, font_size = 20, pen_width = 1, pen_direction = 0;
double pen_x = 0, pen_y = 0;

void reset()
{ pen_color = 0x000000FF;
  font_size = 20;
  pen_width = 1;
  pen_x = 0;
  pen_y = 0;
  pen_direction = 0; }

int string_to_int(string s)
{ return stoi(s); }

string int_to_string(int i)
{ char s[50];
  sprintf(s, "%d", i);
  return s; }

int make_color_int(int r, int g, int b)
{ return ((r & 0xFF) << 24) | ((g & 0xFF) << 16) | ((b & 0xFF) << 8) | 0xFF; }

int make_color(double r, double g, double b)
{ return make_color_int((int)(r * 255), (int)(g * 255), (int)(b * 255)); }

void set_pen_color(int c)
{ pen_color = c; }

void set_pen_color_int(int r, int g, int b)
{ pen_color = ((r & 0xFF) << 24) | ((g & 0xFF) << 16) | ((b & 0xFF) << 8) | 0xFF; }

void set_pen_color(double r, double g, double b)
{ set_pen_color_int((int)(r * 255), (int)(g * 255), (int)(b * 255)); }

void split_color(int rgb, double & r, double & g, double & b)
{ r = ((rgb >> 24) & 0xFF) / 255.0;
  g = ((rgb >> 16) & 0xFF) / 255.0;
  b = ((rgb >> 8) & 0xFF) / 255.0; }

void split_color_int(int rgb, int & r, int & g, int & b)
{ r = (rgb >> 24) & 0xFF;
  g = (rgb >> 16) & 0xFF;
  b = (rgb >> 8) & 0xFF; }

static const double deg360 = 1.0, deg60 = 1.0 / 6.0, deg180 = 0.5, deg240 = 2.0 / 3.0;

double antihls(double n1, double n2, double h)
{ int times = (int)(h / deg360);
  h -= times * deg360;
  if (h < 0.0) h += deg360;
  if (h < deg60) return n1 + (n2 - n1) * h / deg60;
  if (h < deg180) return n2;
  if (h < deg240) return n1+ (n2 - n1) * (deg240 - h) / deg60;
  return n1; }

void hls_to_rgb(double H, double L, double S, double & R, double & G, double & B)
{ double m1 = L <= 0.5 ? L + L * S : L + S - L * S;
  double m2 = 2 * L -m1;
  R = antihls(m1, m2, H - deg60);
  G = antihls(m1, m2, H + deg180);
  B = antihls(m1, m2, H + deg60); }

void hls_to_rgb(double H, double L, double S, int & R, int & G, int & B)
{ double m1 = L <= 0.5 ? L + L * S : L + S - L * S;
  double m2 = 2 * L -m1;
  R = (int)(255 * antihls(m1, m2, H - deg60));
  G = (int)(255 * antihls(m1, m2, H + deg180));
  B = (int)(255 * antihls(m1, m2, H + deg60)); }

void rgb_to_hls(double R, double G, double B, double & H, double & L, double & S)
{ double bst = R;
  if (G > bst) bst = G;
  if (B > bst) bst = B;
  double dst = R;
  if (G < dst) dst = G;
  if (B < dst) dst = B;
  double twol = bst + dst;
  L = twol / 2.0;
  double delta = bst - dst;
  if (twol == 0.0 || twol == 2.0)
    S = 0.0;
  else if (twol <= 1.0)
    S = delta / twol;
  else
    S = delta / (2.0 - twol);
  double h = 0;
  if (delta == 0.0) h = 0.0;
  else if (R >= G && R >= B) h = (G - B) / delta;
  else if (G >= R && G >= B) h = 2.0 + (B - R) / delta;
  else h = 4.0 + (R - G) / delta;
  if (h < 0.0) h += 6.0;
  H = h * deg60; }

int make_color_hls(double h, double l, double s)
{ if (l < 0) l = 0;
  else if (l > 1) l = 1;
  if (s < 0) s = 0;
  else if (s > 1) s = 1;
  double r, g, b;
  hls_to_rgb(h, l, s, r, g, b);
  return make_color_int((int)(r * 255), (int)(g * 255), (int)(b * 255)); }

void set_pen_color_hls(double h, double l, double s)
{ set_pen_color(make_color_hls(h, l, s)); }

void set_pen_width(int w)
{ pen_width = w; }

void set_pen_width_color(int w, int c)
{ pen_width = w;
  pen_color = c; }
  
int window_w = 0, window_h = 0;

void make_window(int w, int h, string text)
{ window_w = w;
  window_h = h;
  c_InitWindow(w, h, text.c_str());
  c_SetTargetFPS(30);
  reset();
  c_init_image(w, h); }

bool running()
{ return ! c_WindowShouldClose(); }

void begin()
{ c_BeginDrawing(); }

void end()
{ c_EndDrawing(); }

void close_window()
{ c_CloseWindow(); }

void set_window_position(int x, int y)
{ if (x < 4) x = 4;
  if (y < 4) y = 4;
  c_set_window_position(x, y); }

void get_window_position(int & x, int & y)
{ c_get_window_position(& x, & y); }

void get_window_size(int & w, int & h)
{ c_get_window_size(& w, & h); }

void set_window_title(string s)
{ c_set_window_title(s.c_str()); }

void set_window_size(int w, int h)
{ c_set_window_size(w, h); }

int get_dpi()
{ return c_get_dpi(); }

void pause(double t)
{ usleep((int)(t * 1000000.0)); }

struct shape
{ vector<Vector2> points; };

shape current_shape;

void start_shape()
{ current_shape.points.clear(); }

void note_position(double x, double y)
{ Vector2 p;
  p.x = (int)x;
  p.y = (int)y;
  current_shape.points.push_back(p); }

void note_position()
{ Vector2 p;
  p.x = (int)pen_x;
  p.y = (int)pen_y;
  current_shape.points.push_back(p); }

void end_shape()
{ }

void fill_shape()
{ c_make_shape(current_shape.points.data(), current_shape.points.size()); 
  c_fill_shape(pen_color); }

void draw_shape()
{ int last = current_shape.points.size() - 1;
  for (int i = 0; i < last; i += 1)
  { Vector2 a = current_shape.points[i];
    Vector2 b = current_shape.points[i + 1];
    c_draw_line(a.x, a.y, b.x, b.y, pen_color, pen_width); }
  Vector2 a = current_shape.points[last];
  Vector2 b = current_shape.points[0];
  c_draw_line(a.x, a.y, b.x, b.y, pen_color, pen_width); }

void set_the_heading(double x0, double y0, double x1, double y1)
{ if (x0 == x1)
    if (y0 >= y1)
      pen_direction = 0;
    else
      pen_direction = 648000;
  else if (y0 == y1)
    if (x0 >= x1)
      pen_direction = 972000;
    else
      pen_direction = 324000;
  else
    pen_direction = (int)(atan2(x1 - x0, y0 - y1) * mul_r_to_d * 36000.0); }

void move_to(double x, double y)
{ set_the_heading(pen_x, pen_y, x, y);
  pen_x = x;
  pen_y = y; }

void draw_to(double x, double y)
{ c_draw_line((int)pen_x, (int)pen_y, (int)x, (int)y, pen_color, pen_width);
  set_the_heading(pen_x, pen_y, x, y);
  pen_x = x;
  pen_y = y; }

void set_heading_degrees(double d)
{ if (d >= 0)
    pen_direction = (int)(d * 3600.0 + 0.5);
  else
    pen_direction = (int)(d * 3600.0 - 0.5); }

double get_heading_degrees()
{ return pen_direction / 3600.0; }

void set_heading_radians(double d)
{ set_heading_degrees(d * mul_r_to_d); }

double get_heading_radians()
{ return pen_direction / 3600.0 * mul_d_to_r; }

double distance_to(double x, double y)
{ return sqrt((x - pen_x) * (x - pen_x) + (y - pen_y) * (y - pen_y)); }

double direction_to_in_degrees(double x, double y)
{ return atan2(x - pen_x, pen_y - y) * mul_r_to_d; }

double direction_from_to_in_degrees(double x0, double y0, double x1, double y1)
{ return atan2(x1 - x0, y0 - y1) * mul_r_to_d; }

double direction_to_in_radians(double x, double y)
{ return atan2(x - pen_x, pen_y - y); }

double direction_from_to_in_radians(double x0, double y0, double x1, double y1)
{ return atan2(x1 - x0, y0 - y1); }

void turn_left_by_degrees(double d)
{ if (d >= 0)
  { pen_direction -= (int)(d * 3600.0 + 0.5);
    if (pen_direction < 0)
      pen_direction += 1296000; }
  else
  { pen_direction -= (int)(d * 3600.0 - 0.5);
    if (pen_direction >= 1296000)
      pen_direction -= 1296000; } }

void turn_right_by_degrees(double d)
{ if (d >= 0)
  { pen_direction += (int)(d * 3600.0 + 0.5);
    if (pen_direction >= 1296000)
      pen_direction -= 1296000; }
  else
  { pen_direction += (int)(d * 3600.0 - 0.5);
    if (pen_direction < 0)
      pen_direction += 1296000; } }

void turn_left_by_radians(double d)
{ turn_left_by_degrees(d * mul_r_to_d); }

void turn_right_by_radians(double d)
{ turn_right_by_degrees(d * mul_r_to_d); }

void get_position(int & x, int & y)
{ x = (int)(pen_x + 0.5);
  y = (int)(pen_y + 0.5); }

void get_position(double & x, double & y)
{ x = pen_x;
  y = pen_y; }

double get_x_position()
{ return pen_x; }

double get_y_position()
{ return pen_y; }

void set_pixel_color(double x, double y, int c)
{ c_set_pixel_color((int)(x+0.5), (int)(y+0.5), c); }

int get_pixel_color(double x, double y)
{ return c_get_pixel_color((int)(x+0.5), (int)(y+0.5)); }

void move_distance(double d)
{ if (pen_direction == 0)
    pen_y -= d;
  else if (pen_direction == 324000)
    pen_x += d;
  else if (pen_direction == 648000)
    pen_y += d;
  else if (pen_direction == 972000)
    pen_x -= d;
  else
  { double dx = d * sin(pen_direction);
    double dy = -d * cos(pen_direction);
    pen_x += dx;
    pen_y += dy; } }

void draw_distance(double d)
{ double dx = 0.0, dy = 0.0;
  if (pen_direction == 0)
    dy = - d;
  else if (pen_direction == 324000)
    dx = d;
  else if (pen_direction == 648000)
    dy = d;
  else if (pen_direction == 972000)
    dx = -d;
  else
  { double a = pen_direction / 3600.0 * mul_d_to_r;
    dx = d * sin(a);
    dy = -d * cos(a); }
  c_draw_line((int)pen_x, (int)pen_y, (int)(pen_x + dx), (int)(pen_y + dy), pen_color, pen_width);
  pen_x += dx;
  pen_y += dy; }

void move_relative(double dx, double dy)
{ set_the_heading(pen_x, pen_y, pen_x + dx, pen_y + dy);
  pen_x += dx;
  pen_y += dy; }

void draw_relative(double dx, double dy)
{ set_the_heading(pen_x, pen_y, pen_x + dx, pen_y + dy);
  c_draw_line((int)pen_x, (int)pen_y, (int)(pen_x + dx), (int)(pen_y + dy), pen_color, pen_width);
  pen_x += dx;
  pen_y += dy; }

void draw_point(double x, double y)
{ c_draw_circle((int)x, (int)y, pen_width, pen_color);
  pen_x = x;
  pen_y = y; }

void draw_point()
{ c_draw_circle((int)pen_x, (int)pen_y, pen_width, pen_color); }

void fill_rectangle(double x, double y, double w, double h)
{ c_fill_rectangle((int)x, (int)y, (int)(w+0.5), (int)(h+0.5), pen_color); }

void fill_rectangle(double x, double y, double w, double h, int c)
{ c_fill_rectangle((int)x, (int)y, (int)(w+0.5), (int)(h+0.5), c); }

void clear()
{ fill_rectangle(0, 0, window_w, window_h, color::white); }

void set_font(int n)
{ c_set_font(n); }

void set_font_size(int n)
{ font_size = n; }

int get_font_size()
{ return font_size; }

void draw_string(string s, bool update, int base)
{ if (base == 0 && ! update)
  { c_DrawText(s.c_str(), (int)pen_x, (int)pen_y, font_size, pen_color);
    return; }
  int w, h;
  c_MeasureText(s.c_str(), &w, &h, font_size);
  if (base == 0)
    c_DrawText(s.c_str(), (int)pen_x, (int)pen_y, font_size, pen_color);
  else if (base == 1)
    c_DrawText(s.c_str(), (int)pen_x, (int)pen_y - h, font_size, pen_color);
  else if (base == 2)
    c_DrawText(s.c_str(), (int)pen_x - w, (int)pen_y, font_size, pen_color);
  else if (base == 3)
    c_DrawText(s.c_str(), (int)pen_x - w, (int)pen_y - h, font_size, pen_color);
  else if (base == 4)
    c_DrawText(s.c_str(), (int)pen_x - w / 2, (int)pen_y - h / 2, font_size, pen_color);
  if (update)
    pen_x += w; }
  
  void draw_string(int n, bool update, int base)
  { char s[20];
    snprintf(s, 19, "%d", n);
    draw_string(s, update, base); }

  void draw_string(double d, bool update, int base)
  { char s[100];
    snprintf(s, 99, "%f", d);
    draw_string(s, update, base); }

  void draw_string(char c, bool update, int base)
  { char s[2];
    s[0] = c;
    s[1] = 0;
    draw_string(s, update, base); }

void measure_string(string s, int & w, int & h)
{ c_MeasureText(s.c_str(), & w, & h, font_size); }

void * load_picture(string fname)
{ return c_load_picture(fname.c_str()); }

void draw_picture(void * i, int x, int y, int w, int h)
{ c_draw_picture(i, x, y, w, h); }

int get_picture_width(void * i)
{ return c_get_picture_width(i); }

int get_picture_height(void * i)
{ return c_get_picture_height(i); }

void finished_with_picture(void * i)
{ c_finished_with_picture(i); }

bool check_wav_file(string s)
{ return c_check_wav_file(s.c_str()); }

sound load_sound(string fname)
{ return c_load_sound(fname.c_str()); }

void play_sound(sound s)
{ c_play_sound(s); }

void finished_with_sound(sound s)
{ c_finished_with_sound(s); }

void draw_image()
{ c_draw_image(); }

bool was_mouse_clicked(int & x, int & y)
{ return c_was_mouse_clicked(& x, & y); }

int get_key_pressed()
{ return c_get_key_pressed(); }

char wait_for_mouse_clicked_or_key_pressed(int & x, int & y)
{ while(true)
  { if (was_mouse_clicked(x, y))
      return 'M';
    char c = get_key_pressed();
    if (c)
    { x = c;
      return 'K'; }
    pause(0.01); } }

char wait_for_mouse_clicked_or_key_pressed_or_time(int & x, int & y, double t)
{ struct timeval tv;
  gettimeofday(& tv, NULL);
  double waitfor = tv.tv_sec + tv.tv_usec / 1000000.0 + t;
  while(true)
  { if (was_mouse_clicked(x, y))
      return 'M';
    char c = get_key_pressed();
    if (c)
    { x = c;
      return 'K'; }
    pause(0.01);
    gettimeofday(& tv, NULL);
    double now = tv.tv_sec + tv.tv_usec / 1000000.0;
    if (now >= waitfor)
      return 'T'; } }

int get_clock_time()
{ time_t t = time(NULL);
  struct tm * tim = localtime(&t);
  return tim->tm_hour * 10000 + tim->tm_min * 100 + tim->tm_sec; }

int get_calendar_date()
{ time_t t = time(NULL);
  struct tm * tim = localtime(&t);
  return (tim->tm_year + 1900) * 10000 + (tim->tm_mon + 1) * 100 + tim->tm_mday; }

void print(string s)
{ printf("%s", s.c_str()); }

void print(const char * s)
{ printf("%s", s); }

void print(int n)
{ printf("%d", n); }

void print(char c)
{ printf("%c", c); }

void print(double d)
{ printf("%14g", d); }

void new_line()
{ printf("\n"); }

string read_something()
{ string r = "";
  int c = 0;
  while (true)
  { c = fgetc(stdin);
    if (c == EOF || c > ' ' && c != ',')
      break; }
  while (true)
  { if (c <= ' ' || c == ',')
      break;
    r += (char)c;
    c = fgetc(stdin); }
  ungetc(c, stdin);
  return r; }

string read_string()
{ return read_something(); }

string read_line()
{ string r = "";
  while (true)
  { int c = fgetc(stdin);
    if (c == '\n' || c == EOF)
      break;
    r += (char)c; }
  return r; }

int read_int()
{ return atoi(read_something().c_str()); }

double read_double()
{ return atof(read_something().c_str()); }

char read_char()
{ return fgetc(stdin); }

bool read_bool()
{ string s = read_something();
  if (s == "1") return true;
  if (s == "0") return false;
  if (strcasecmp(s.c_str(), "true") == 0) return true;
  if (strcasecmp(s.c_str(), "false") == 0) return false;
  if (strcasecmp(s.c_str(), "t") == 0) return true;
  if (strcasecmp(s.c_str(), "f") == 0) return false;
  if (strcasecmp(s.c_str(), "yes") == 0) return true;
  if (strcasecmp(s.c_str(), "no") == 0) return false;
  if (strcasecmp(s.c_str(), "y") == 0) return true;
  if (strcasecmp(s.c_str(), "n") == 0) return false;
  return false; }

void experiment()
{ c_experiment(); }

void end_experiment()
{ c_end_experiment(); }

void mymain();

void * threadfn(void * a)
{ mymain();
  return NULL; }

bool unrandom = false;

void not_really_random()
{ unrandom = true; }

int random_in_range(int a, int b)
{ return random() % (b - a + 1) + a; }

void run()
{ if (! unrandom)
    srandomdev();
  pthread_t thrid;
  pthread_create(& thrid, NULL, threadfn, NULL);
  while (running())
  { c_interactions();
    begin();
    draw_image();
    end(); }
  close_window(); }
