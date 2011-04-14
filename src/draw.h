#ifndef DRAW_H_
#define DRAW_H_

#ifndef __APPLE__
#include <GL/glut.h>
#else
#include <GLUT/glut.h>
#endif

enum {
	IMTK_TEXT_COLOR,
	IMTK_BASE_COLOR,
	IMTK_FOCUS_COLOR,
	IMTK_BEVEL_LIT_COLOR,
	IMTK_BEVEL_SHAD_COLOR
};


enum {
	FRAME_OUTSET,
	FRAME_INSET
};

void imtk_set_color(int col, float r, float g, float b, float a);
float *imtk_get_color(int col);
void imtk_draw_frame(int x, int y, int w, int h, int style);
void imtk_draw_string(int x, int y, const char *str);
int imtk_string_size(const char *str);

#endif	/* DRAW_H_ */
