#include "library.h"

using namespace std;

void make_Istvans_shape();
void make_star();
void make_ugly_shape();

int main()
{ make_window(800, 600, "First C++ Test");
  make_ugly_shape();
  while (running())
  { begin();

    set_pen_color(color::yellow);
    fill_shape();
    set_pen_color(color::red);
    draw_shape();

    // move_to(500, 100);
    // draw_to(500, 200);
    // draw_to(400, 200);
    // draw_to(400, 100);
    // draw_to(500, 100);

    end(); }
  close_window(); }

void make_Istvans_shape()
{ start_shape();
  add_point_to_shape(100, 100);
  add_point_to_shape(300, 100);
  add_point_to_shape(200, 200);
  add_point_to_shape(300, 300);
  add_point_to_shape(100, 300);
  end_shape(); }

void make_star()
{ start_shape();
  add_point_to_shape(200, 80);
  add_point_to_shape(240, 160);
  add_point_to_shape(320, 160);
  add_point_to_shape(240, 220);
  add_point_to_shape(280, 320);
  add_point_to_shape(200, 260);
  add_point_to_shape(120, 320);
  add_point_to_shape(160, 220);
  add_point_to_shape(80, 160);
  add_point_to_shape(160, 160);
  end_shape(); }

void make_ugly_shape()
{ start_shape();
  add_point_to_shape(40, 40);
  add_point_to_shape(140, 20);
  add_point_to_shape(180, 80);
  add_point_to_shape(100, 140);
  add_point_to_shape(120, 160);
  add_point_to_shape(180, 140);
  add_point_to_shape(140, 200);
  add_point_to_shape(160, 220);
  add_point_to_shape(240, 140);
  add_point_to_shape(220, 80);
  add_point_to_shape(300, 100);
  add_point_to_shape(280, 160);
  add_point_to_shape(220, 200);
  add_point_to_shape(200, 260);
  add_point_to_shape(80, 240);
  add_point_to_shape(40, 280);
  add_point_to_shape(80, 180);
  add_point_to_shape(40, 160);
  add_point_to_shape(120, 80);
  end_shape(); }
