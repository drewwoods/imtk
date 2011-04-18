#ifndef DRAW_H_
#define DRAW_H_

#ifndef __APPLE__
#include <GL/glut.h>
#else
#include <GLUT/glut.h>
#endif
#include "imtk.h"

enum {
	FRAME_OUTSET,
	FRAME_INSET
};

/*void imtk_set_color(int col, float r, float g, float b, float a); in imtk.h */
float *imtk_get_color(int col);
void imtk_draw_rect(int x, int y, int w, int h, float *color_rgba);
void imtk_draw_frame(int x, int y, int w, int h, int style);
void imtk_draw_string(int x, int y, const char *str);
int imtk_string_size(const char *str);

#endif	/* DRAW_H_ */
