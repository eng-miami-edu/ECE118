#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include "raylib.h"
#include "rlgl.h"
#include "gpc.h"
#include "fonts.h"

#define srandom srand
#define random rand

Texture texture;
Image image;
Font thefont, fonts[num_fonts];

struct Color int_to_Color(int c)
{ struct Color x;
  x.r = (c >> 24) & 0xFF;
  x.g = (c >> 16) & 0xFF;
  x.b = (c >> 8) & 0xFF;
  x.a = c & 0xFF; 
  return x; }

int Color_to_int(struct Color c)
{ return (c.r << 24) | (c.g << 16) | (c.b << 8) | 0xFF; }

void c_DrawText(const char * s, int x, int y, int fs, int c)
{ struct Color color = int_to_Color(c);
  ImageDrawTextEx(& image, thefont, s, (Vector2){x, y}, fs, 2, color); }

void c_MeasureText(const char * s, int * w, int * h, int fs)
{ int total = 2 * strlen(s), i, fsize = fs;
  if (fsize < 32) fsize = 32;
  for (i = 0; 1; i += 1)
  { char c = s[i], str[2];
    if (c == 0) break;
    GlyphInfo g = GetGlyphInfo(thefont, c);
    total += g.advanceX; }
  * w = (int)(total * fsize / 32.0);
  * h = fsize; }

void c_fill_triangle_fb(int xt, int yt, int x2, int y2, int x3, int y3, struct Color c)
{ int y;
  if (yt == y2)
    return;
  double is1 = (x2 - xt) / (double)(y2 - yt);
  double is2 = (x3 - xt) / (double)(y3 - yt);
  double cx1 = xt, cx2 = xt;
  for (y = yt; y <= y2; y += 1)
  { ImageDrawLine(& image, (int)cx1, y, (int)cx2, y, c);
    cx1 += is1;
    cx2 += is2; } }

void c_fill_triangle_ft(int x1, int y1, int x2, int y2, int xb, int yb, struct Color c)
{ int y;
  if (yb == y2)
    return;
  double is1 = (xb - x1) / (double)(yb - y1);
  double is2 = (xb - x2) / (double)(yb - y2);
  double cx1 = xb, cx2 = xb;
  for (y = yb; y > y1; y -= 1)
  { ImageDrawLine(& image, (int)cx1, y, (int)cx2, y, c);
    cx1 -= is1;
    cx2 -= is2; } }

void swapif(int * a, int * b, int * c, int * d)
{ if (* b > * d)
  { int t = * a;
    * a = * c;
    * c = t;
    t = * b;
    * b = * d;
    * d = t; } }

void c_fill_triangle(int x1, int y1, int x2, int y2, int x3, int y3, int color)
{ struct Color c = int_to_Color(color);
  swapif(& x1, & y1, & x2, & y2);
  swapif(& x2, & y2, & x3, & y3);
  swapif(& x1, & y1, & x2, & y2);
  if (y2 == y3)
    c_fill_triangle_fb(x1, y1, x2, y2, x3, y3, c);
  else if (y1 == y2)
    c_fill_triangle_ft(x1, y1, x2, y2, x3, y3, c);
  else
  { char t[100];
    double x4 = x1 + ((y2 - y1) / (double)(y3 - y1)) * (x3 - x1);
    double y4 = y2;
    c_fill_triangle_fb(x1, y1, x2, y2, x4, y4, c);
    c_fill_triangle_ft(x2, y2, x4, y4, x3, y3, c); } }

gpc_tristrip * generate_tristrip(const int len, Vector2 poly[])
{ gpc_vertex_list * vlist = (gpc_vertex_list *)malloc(sizeof(gpc_vertex_list));
  vlist->num_vertices = len;
  vlist->vertex = (gpc_vertex *)malloc(len * sizeof(gpc_vertex));
  int i;
  for (i = 0; i < len; i += 1)
  { vlist->vertex[i].x = poly[i].x;
    vlist->vertex[i].y = poly[i].y; }
  gpc_polygon * polygon = (gpc_polygon*)malloc(sizeof(gpc_polygon));
  polygon->num_contours = 1;
  polygon->hole = 0;
  polygon->contour = vlist;
  gpc_tristrip * tristrip = (gpc_tristrip*)malloc(sizeof(gpc_tristrip));
  gpc_polygon_to_tristrip(polygon, tristrip);
  free(vlist->vertex);
  free(vlist);
  free(polygon);
  return tristrip; }

void drawTriStrip(gpc_tristrip * tristrip, int c)
{ int i, j;
  for(i = 0; i < tristrip->num_strips; i+=1)
    for(j = 0; j < tristrip->strip[i].num_vertices-2; j += 1)
    { gpc_vertex * v = tristrip->strip[i].vertex;
      c_fill_triangle((int)v[j].x, (int)v[j].y, (int)v[j+1].x, (int)v[j+1].y, (int)v[j+2].x, (int)v[j+2].y, c); } }

gpc_tristrip * current_tristrip = NULL;

void c_make_shape(Vector2 * pts, int n)
{ if (current_tristrip != NULL)
    gpc_free_tristrip(current_tristrip);
  current_tristrip = generate_tristrip(n, pts); }

void c_fill_shape(int color)
{ drawTriStrip(current_tristrip, color); }

void c_draw_circle(int x1, int y1, int r, int c)
{ struct Color color = int_to_Color(c);
  int rsq = r * r;
  int maxx = r;
  int x, y;
  for (y = 0; y <= r; y += 1)
  { int ysq = y * y;
    for (x = maxx; x >= 0; x -= 1)
      if (x * x + ysq <= rsq)
      { ImageDrawLine(& image, x1 - x, y1 + y, x1 + x, y1 + y, color);
        ImageDrawLine(& image, x1 - x, y1 - y, x1 + x, y1 - y, color);
        maxx = x;
        break; } } }

void c_draw_line(int x1, int y1, int x2, int y2, int c, int w)
{ struct Color color = int_to_Color(c);
  if (w < 2)
  { ImageDrawLine(& image, x1, y1, x2, y2, color);
    return; }
  double dx = x2 - x1, dy = y2 - y1;
  double len2 = sqrt(dx * dx + dy * dy) * 2;
  double dwx = -dy * w / len2, dwy = dx * w / len2;
  double p1x = x1 + dwx, p1y = y1 + dwy;
  double p2x = p1x + dx, p2y = p1y + dy;
  double p4x = x1 - dwx, p4y = y1 - dwy;
  double p3x = p4x + dx, p3y = p4y + dy;
  c_fill_triangle((int)p1x, (int)p1y, (int)p2x, (int)p2y, (int)p3x, (int)p3y, c);
  c_fill_triangle((int)p4x, (int)p4y, (int)p1x, (int)p1y, (int)p3x, (int)p3y, c);
  c_draw_circle((int)x1, (int)y1, (int)(w * 0.5 - 0.5), c);
  c_draw_circle((int)x2, (int)y2, (int)(w * 0.5 - 0.5), c); }

void c_fill_rectangle(int x, int y, int w, int h, int c)
{ struct Color color = int_to_Color(c);
  ImageDrawRectangle(& image, x, y, w, h, color); }

void c_set_pixel_color(int x, int y, int c)
{ struct Color color = int_to_Color(c);
  ImageDrawPixel(& image, x, y, color); }

int c_get_pixel_color(int x, int y)
{ Color c = GetImageColor(image,x, y);
  return Color_to_int(c); }

void c_InitWindow(int w, int h, const char * text)
{ SetTraceLogLevel(LOG_ERROR);
  InitWindow(w, h, text);
#if _WIN32
  fonts[f_times_new_roman] = LoadFont("C:\\Windows\\Fonts\\times.ttf");
  fonts[f_times_new_roman_italic] = LoadFont("C:\\Windows\\Fonts\\timesi.ttf");
  fonts[f_times_new_roman_bold] = LoadFont("C:\\Windows\\Fonts\\timesbd.ttf");
  fonts[f_arial] = LoadFont("C:\\Windows\\Fonts\\arial.ttf");
  fonts[f_courier_new] = LoadFont("C:\\Windows\\Fonts\\cour.ttf");
  fonts[f_courier_new_bold] = LoadFont("C:\\Windows\\Fonts\\courbd.ttf");
#elif __APPLE__
  fonts[f_times_new_roman] = LoadFont("/System/Library/Fonts/Supplemental/Times New Roman.ttf");
  fonts[f_times_new_roman_italic] = LoadFont("/System/Library/Fonts/Supplemental/Times New Roman Italic.ttf");
  fonts[f_times_new_roman_bold] = LoadFont("/System/Library/Fonts/Supplemental/Times New Roman Bold.ttf");
  fonts[f_arial] = LoadFont("/System/Library/Fonts/Supplemental/Arial Unicode.ttf");
  fonts[f_courier_new] = LoadFont("/System/Library/Fonts/Supplemental/Courier New.ttf");
  fonts[f_courier_new_bold] = LoadFont("/System/Library/Fonts/Supplemental/Courier New Bold.ttf");
#endif
  thefont = fonts[f_times_new_roman]; }

int newwpx = -1, newwpy = -1;

void c_set_window_position(int x, int y)
{ newwpx = x;
  newwpy = y; }

void c_get_window_position(int * x, int * y)
{ Vector2 v = GetWindowPosition();
  * x = (int)v.x;
  * y = (int)v.y; }

void c_get_window_size(int * w, int * h)
{ * w = GetScreenWidth();
  * h = GetScreenHeight(); }

const char * newtitle = NULL;

void c_set_window_title(const char * s)
{ newtitle = s; }

void c_set_window_size(int w, int h)
{ SetWindowSize(w, h); }

int c_get_dpi()
{ Vector2 v = GetWindowScaleDPI();
  if (v.x > v.y)
    return (int)v.x;
  return (int)v.y; }

void c_set_font(int n)
{ if (n >= 0 && n < num_fonts)
    thefont = fonts[n]; }

void c_SetTargetFPS(int fps)
{ SetTargetFPS(fps); }

int c_WindowShouldClose()
{ return WindowShouldClose(); }

void c_BeginDrawing()
{ BeginDrawing(); }

void c_EndDrawing()
{ EndDrawing();
  ClearBackground(WHITE); }

void c_CloseWindow()
{ UnloadImage(image);
  UnloadTexture(texture); 
  CloseWindow(); }

void c_init_image(int w, int h)
{ image = GenImageColor(w, h, WHITE); }
  
void c_draw_image()
{ texture = LoadTextureFromImage(image);
  DrawTexture(texture, 0, 0, WHITE); }

void * c_load_picture(const char * fname)
{ Image * i =  (Image *)malloc(sizeof(Image));
  * i = LoadImage(fname);
  return i; }

int c_get_picture_width(void * i)
{ return ((Image *)i)->width; }

int c_get_picture_height(void * i)
{ return ((Image *)i)->height; }

void c_finished_with_picture(void * i)
{ UnloadImage(* (Image *)i);
  free(i); }

int loadedad = 0;

int c_check_wav_file(const char * fname)
{ FILE * f = fopen(fname, "rb");
  const char * x = "WAV file defective: ";
  if (f == NULL)
  { printf("%sNot readable\n", x);
    return 0; }
  fseek(f, 0, SEEK_SET);
  int flen = ftell(f);
  char ch[100];
  int n, len;
  fread(ch, 4, 1, f);
  if (ch[0] != 'R' || ch[1] != 'I' || ch[2] != 'F' || ch[3] != 'F')
  { printf("%s'RIFF' missing\n", x);
    fclose(f);
    return 0; }
  fread(& n, 4, 1, f);
  if (n + 8 != flen && flen != 0)
  { printf("%sfile length wrong %d %d\n", x, n, flen);
    fclose(f);
    return 0; }
  fread(ch, 4, 1, f);
  if (ch[0] != 'W' || ch[1] != 'A' || ch[2] != 'V' || ch[3] != 'E')
  { printf("%s'WAVE' missing\n", x);
    fclose(f);
    return 0; }
  fread(ch, 4, 1, f);
  if (ch[0] != 'f' || ch[1] != 'm' || ch[2] != 't')
  { printf("%s'fmt' missing\n", x);
    fclose(f);
    return 0; }
  fread(& n, 4, 1, f);
  if (n != 16)
  { printf("%slength of format not 16\n", x);
    fclose(f);
    return 0; }
  fread(& n, 2, 1, f);
  if (n != 1)
  { printf("%stype is not PCM\n", x);
    fclose(f);
    return 0; }
  fread(& n, 2, 1, f);  // number of channels
  fread(& n, 4, 1, f);  // sample rate, e.g. 44100
  fread(& n, 4, 1, f);
  fread(& n, 2, 1, f);
  fread(& n, 2, 1, f);
  fread(ch, 4, 1, f);
  if (ch[0] != 'd' || ch[1] != 'a' || ch[2] != 't' || ch[3] != 'a')
  { printf("%s'data' missing\n", x);
    fclose(f);
    return 0; }
  fclose(f);
  return 1; }

int ends_with(const char * big, const char * little)
{ int lb = strlen(big), ll = strlen(little);
  int sp = lb - ll;
  if (sp < 0)
    return 0;
  if (strcasecmp(big + sp, little) == 0)
    return 1;
  return 0; }

void * c_load_sound(const char * fname)
{ if (ends_with(fname, ".wav"))
    c_check_wav_file(fname);
  if (! loadedad)
  { InitAudioDevice();
    loadedad = 1; }
  Sound * s =  (Sound *)malloc(sizeof(Sound));
  * s = LoadSound(fname);
  return s; }

void c_play_sound(void * s)
{ PlaySound(*(Sound *)s); }

void c_finished_with_sound(void * i)
{ UnloadSound(* (Sound *)i);
  free(i); }

void c_draw_picture(void * i, int x, int y, int w, int h)
{ Image * im = (Image *)i;
  if (w == -1)
    w = im->width;
  if (h == -1)
    h = im->height;
  ImageDraw(& image, 
            * im,
            (Rectangle){0, 0, im->width, im->height},
            (Rectangle){x, y, w, h},
            WHITE); }

int mousedown = 0, mouseclicked = 0, mousedownx = 0, mousedowny = 0, mouseclickx = 0, mouseclicky = 0;

int c_was_mouse_clicked(int * x, int * y)
{ if (! mouseclicked)
    return 0;
  mouseclicked = false;
  * x = mouseclickx;
  * y = mouseclicky;
  return 1; }

int c_get_key_pressed()
{ return GetCharPressed(); }

void c_interactions()
{ if (newtitle != NULL)
  { SetWindowTitle(newtitle);
    newtitle = NULL; }
  if (newwpx > 0)
  { SetWindowPosition(newwpx, newwpy);
    newwpx = -1; }
  if (mousedown)
  { if (IsMouseButtonUp(MOUSE_BUTTON_LEFT))
    { mousedown = false;
      int x = GetMouseX(), y = GetMouseY();
      if (x == mousedownx && y == mousedowny)
      { mouseclickx = x;
        mouseclicky = y;
        mouseclicked = 1; } } }
  else if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
  { mousedown = true;
    mousedownx = GetMouseX();
    mousedowny = GetMouseY();
    mouseclicked = false; } }

/*
void c_experiment()
{ }

void c_end_experiment()
{ }
*/

const int max_samples = 512;
const int max_samples_per_update = 4096;
double frequency = 440.0;
double audioFrequency = 440.0;
double oldFrequency = 1.0;
double idx = 0.0;
AudioStream stream;

void AudioInputCallback(void * buffer, unsigned int frames)
{ audioFrequency = frequency + (audioFrequency - frequency) * 0.95;
  double incr = audioFrequency / 44100.0;
  short int * d = (short int *)buffer;
  int i;
  for (i = 0; i < frames; i += 1)
  { d[i] = (short int)(32000.0 * sin(2 * PI * idx));
    idx += incr;
    if (idx > 1.0)
      idx -= 1.0; } }

void c_experiment()
{ if (! loadedad)
  { InitAudioDevice();
    loadedad = 1; }
  SetAudioStreamBufferSizeDefault(max_samples_per_update);
  stream = LoadAudioStream(44100, 16, 1);
  // SetAudioStreamCallback(stream, AudioInputCallback);
  short int * data = (short int *)malloc(max_samples * sizeof(short int));
  short int * writebuf = (short int *)malloc(max_samples_per_update * sizeof(short int));
  PlayAudioStream(stream); }

void c_end_experiment()
{ UnloadAudioStream(stream);
  CloseAudioDevice(); }


