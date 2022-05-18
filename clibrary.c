#include <stdlib.h>
#include <stdio.h>
#include "raylib.h"
#include "rlgl.h"
#include "gpc.h"
 
struct Color int_to_Color(int c)
{ struct Color x;
  x.r = (c >> 24) & 0xFF;
  x.g = (c >> 16) & 0xFF;
  x.b = (c >> 8) & 0xFF;
  x.a = c & 0xFF; 
  return x; }

gpc_tristrip * generate_tristrip(const int len, Vector2 poly[]) {
    gpc_vertex_list * vlist = (gpc_vertex_list *)malloc(sizeof(gpc_vertex_list));
    vlist->num_vertices = len;
    vlist->vertex = (gpc_vertex *)malloc(len * sizeof(gpc_vertex));
    int i;
    for (i = 0; i < len; i += 1) {
        vlist->vertex[i].x = poly[i].x;
        vlist->vertex[i].y = poly[i].y;
    }

    gpc_polygon * polygon = (gpc_polygon*)malloc(sizeof(gpc_polygon));
    polygon->num_contours = 1;
    polygon->hole = 0;
    polygon->contour = vlist;

    gpc_tristrip * tristrip = (gpc_tristrip*)malloc(sizeof(gpc_tristrip));
    
    gpc_polygon_to_tristrip(polygon, tristrip);
    printf("num_strips = %d\n", tristrip->num_strips);
    for(i = 0; i < tristrip->num_strips; i+=1) {
        int j;
        printf("  num_vertices = %d\n", tristrip->strip[i].num_vertices);
        for(j = 0; j < tristrip->strip[i].num_vertices; j += 1) {
            printf("    %f, %f\n", tristrip->strip[i].vertex[j].x, tristrip->strip[i].vertex[j].y);
        }
    }

    free(vlist->vertex);
    free(vlist);
    free(polygon);
    return tristrip;
}

void drawTriStrip(gpc_tristrip * tristrip, int c)
{ int i, j;
  struct Color color = int_to_Color(c);
  for(i = 0; i < tristrip->num_strips; i+=1)
    for(j = 0; j < tristrip->strip[i].num_vertices-2; j += 1)
      if (j % 2)
        DrawTriangle((Vector2){tristrip->strip[i].vertex[j  ].x, tristrip->strip[i].vertex[j  ].y},
                     (Vector2){tristrip->strip[i].vertex[j+1].x, tristrip->strip[i].vertex[j+1].y}, 
                     (Vector2){tristrip->strip[i].vertex[j+2].x, tristrip->strip[i].vertex[j+2].y}, color);
      else
        DrawTriangle((Vector2){tristrip->strip[i].vertex[j+1].x, tristrip->strip[i].vertex[j+1].y},
                     (Vector2){tristrip->strip[i].vertex[j  ].x, tristrip->strip[i].vertex[j  ].y}, 
                     (Vector2){tristrip->strip[i].vertex[j+2].x, tristrip->strip[i].vertex[j+2].y}, color); }

gpc_tristrip * current_tristrip = NULL;

void c_make_shape(Vector2 * pts, int n)
{ if (current_tristrip != NULL)
    gpc_free_tristrip(current_tristrip);
  current_tristrip = generate_tristrip(n, pts); }

void c_fill_shape(int color)
{ drawTriStrip(current_tristrip, color); }

void c_draw_line(int x1, int y1, int x2, int y2, int c)
{ struct Color color = int_to_Color(c);
  printf("**** c_draw_line, %d %d %d %d %X\n", x1, y1, x2, y2, c);
  DrawLine(x1, y1, x2, y2, color); }

void c_InitWindow(int w, int h, const char * text)
{ InitWindow(w, h, text); }

void c_SetTargetFPS(int fps)
{ SetTargetFPS(fps); }

int c_WindowShouldClose()
{ return WindowShouldClose(); }

void c_BeginDrawing()
{ BeginDrawing(); }

void c_EndDrawing()
{ EndDrawing();
  ClearBackground(RAYWHITE); }

void c_CloseWindow()
{ CloseWindow(); }
