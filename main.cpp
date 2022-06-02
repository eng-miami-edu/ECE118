#include "library.h"

using namespace std;

void make_Istvans_shape();
void make_star();
void make_ugly_shape();

int main()
{ make_window(1000, 700, "Third C++ Test");
  not_really_random();
  run(); }

void textinabox(string s)
{ int w, h;
  set_pen_width(5);
  set_pen_color(color::red);
  draw_point();
  set_pen_width(1);
  set_pen_color(color::black);
  measure_string(s, w, h);
  draw_relative(0, h);
  draw_relative(w, 0);
  draw_relative(0, -h);
  draw_relative(-w, 0);
  draw_string(s); }

const bool do_test_1 = false,
           do_test_2 = false,
           do_test_3 = true,
           do_test_4 = false, 
           do_test_5 = false;

void mymain()
{ printf("%d %d\n", get_calendar_date(), get_clock_time());

  if (do_test_1)
  { int w, h;
    get_window_size(w, h);
    printf("%d x %d\n", w, h);
    make_ugly_shape();
    set_pen_color(color::yellow);
    fill_shape();
    set_pen_color(color::red);
    draw_shape();

    move_to(500, 100);
    draw_to(500, 200);
    draw_to(400, 200);
    draw_to(400, 100);
    draw_to(500, 100);

    move_to(420, 180);
    set_heading_degrees(0);
    set_pen_width(3);
    draw_distance(60);
    turn_right_by_degrees(120);
    draw_distance(60);
    turn_right_by_degrees(120);
    draw_distance(60);
    turn_right_by_degrees(120);
    draw_string("Triangle");

    set_pen_width(50);
    set_pen_color(color::orange);
    move_to(450, 400);
    draw_to(650, 200);
    draw_to(850, 400);
    draw_to(650, 600);
    draw_to(450, 400);

    fill_rectangle(25, 425, 350, 150, color::sodium_d);
    printf("colour = %08X\n", get_pixel_color(28, 428));
    set_pen_color(color::blue);
    draw_point(100, 500);
    draw_point(300, 500);
    set_pen_color(color::green);
    draw_point(200, 500);

    char text[100];
    sprintf(text, "%d, %d, %d", 1, 12, 123);
    set_pen_color(color::blue);
    move_to(600, 100);
    draw_string(text); }

  if (do_test_2)
  { set_font(font::arial);
    move_to(100, 100);
    set_font_size(72);
    textinabox("abcdefghijklmnopqrstuvwxyz");
    move_to(100, 200);
    set_font_size(48);
    textinabox("abcdefghijklmnopqrstuvwxyz");
    move_to(100, 300);
    set_font_size(36);
    textinabox("abcdefghijklmnopqrstuvwxyz");
    move_to(100, 400);
    set_font_size(32);
    textinabox("abcdefghijklmnopqrstuvwxyz");
    move_to(100, 500);
    set_font_size(30);
    textinabox("abcdefghijklmnopqrstuvwxyz");
    move_to(100, 600);
    set_font_size(10);
    textinabox("abcdefghijklmnopqrstuvwxyz");
    move_to(100, 650);
    draw_string("Greetings, ", true);
    draw_string("Human.");
    move_to(500, 400);
    draw_string(123);
    move_to(500, 450);
    draw_string(pi);
    move_to(500, 500);
    draw_string('{'); }

  if (do_test_3)
  { move_to(200, 200);
    set_pen_width(20);
    set_pen_color(color::black);
    string s = "The cat sat on the mat";
    for (int i = 0; i < s.length(); i += 1)
    { int w, h;
      pause(0.2);
      string str = string("") + s[i];
      draw_string(str);
      measure_string(str, w, h);
      move_relative(w, 0); }
  
    set_pen_color(color::red);
    set_pen_width(3);
    double alpha = 0.0;
    while (true)
    { pause(0.01);
      move_to(400 + 200 * sin(alpha) + 50 * sin(alpha * 15),
              400 + 200 * cos(alpha) + 50 * cos(alpha * 15));
      draw_point();
      alpha += 0.01; } }

  if (do_test_4)
  { set_window_position(0, 0);
    set_pen_width(1);
    for (int x = 0; x < 1000; x += 1)
    { double h = (double)x / 1000.0;
      set_pen_color_hls(h, 0.5, 1);
      move_to(x, 600);
      draw_to(x, 700); }
    picture p = load_picture("rabbit.png");
    draw_picture(p, 50, 50);
    draw_picture(p, 300, 50, get_picture_width(p) / 2, get_picture_height(p) / 2);
    finished_with_picture(p);
    p = load_picture("tigger.png");
    draw_picture(p, 50, 320);
    finished_with_picture(p);

    set_pen_width(4);
    move_to(300, 200);
    draw_string("Click the mouse about");
    move_to(300, 230);
    draw_string("or type something ", true);
    int x, y;
    bool firstclick = true, anything = false;
    sound s = load_sound("sound.wav");
    set_pen_width_color(5, color::violet);
    while (true)
    { char e = wait_for_mouse_clicked_or_key_pressed_or_time(x, y, 2.5);
      if (! anything)
      { set_window_title("We are interacting!");
        anything = true;
        play_sound(s); }
      if (e == 'M')
      { if (firstclick)
         { move_to(x, y);
           firstclick = false; }
        else
          draw_to(x, y); }
      else if (e == 'K')
        draw_string((char) x, true); } }

  if (do_test_5)
  { set_font(f_times_new_roman_bold);
    for (int i = 0; i < 16; i += 1)
    { int ypos = i * 30;
      for (int j = 0; j < 16; j += 1)
      { int xpos = j * 30;
        char s[2];
        s[0] = i * 16 + j;
        s[1] = 0;
        move_to(xpos, ypos);
        draw_string(s); } } }
 }

void make_Istvans_shape()
{ start_shape();
  note_position(100, 100);
  note_position(300, 100);
  note_position(200, 200);
  note_position(300, 300);
  note_position(100, 300);
  end_shape(); }

void make_star()
{ start_shape();
  note_position(200, 80);
  note_position(240, 160);
  note_position(320, 160);
  note_position(240, 220);
  note_position(280, 320);
  note_position(200, 260);
  note_position(120, 320);
  note_position(160, 220);
  note_position(80, 160);
  note_position(160, 160);
  end_shape(); }

void make_ugly_shape()
{ start_shape();
  note_position(40, 40);
  note_position(140, 20);
  note_position(180, 80);
  note_position(100, 140);
  note_position(120, 160);
  note_position(180, 140);
  note_position(140, 200);
  note_position(160, 220);
  note_position(240, 140);
  note_position(220, 80);
  note_position(300, 100);
  note_position(280, 160);
  note_position(220, 200);
  note_position(200, 260);
  note_position(80, 240);
  note_position(40, 280);
  note_position(80, 180);
  note_position(40, 160);
  note_position(120, 80);
  end_shape(); }

