#include <string.h>
#include <assert.h>
#include "draw.h"
#include "imtk.h"

/* default colors, can be changed with imtk_set_color */
static float colors[][4] = {
	{0.0, 0.0, 0.0, 1.0},		/* text color */
	{0.7, 0.7, 0.7, 1.0},		/* base color */
	{0.85, 0.85, 0.85, 1.0},	/* focus color */
	{1.0, 1.0, 1.0, 1.0},		/* lit bevel */
	{0.3, 0.3, 0.3, 1.0}		/* shadowed bevel */
};

void imtk_set_color(int col, float r, float g, float b, float a)
{
	assert(col >= 0 && col < sizeof colors / sizeof *colors);

	colors[col][0] = r;
	colors[col][1] = g;
	colors[col][2] = b;
	colors[col][3] = a;
}

float *imtk_get_color(int col)
{
	return colors[col];
}

void imtk_draw_rect(int x, int y, int w, int h, float *color_rgba)
{
	glBegin(GL_QUADS);
	if(color_rgba) {
		glColor4fv(color_rgba);
	}
	glVertex2f(x, y);
	glVertex2f(x + w, y);
	glVertex2f(x + w, y + h);
	glVertex2f(x, y + h);
	glEnd();
}

void imtk_draw_frame(int x, int y, int w, int h, int style)
{
	float tcol[4], bcol[4];

	switch(style) {
	case FRAME_INSET:
		memcpy(tcol, colors[IMTK_BEVEL_SHAD_COLOR], sizeof tcol);
		memcpy(bcol, colors[IMTK_BEVEL_LIT_COLOR], sizeof bcol);
		break;

	case FRAME_OUTSET:
	default:
		memcpy(tcol, colors[IMTK_BEVEL_LIT_COLOR], sizeof tcol);
		memcpy(bcol, colors[IMTK_BEVEL_SHAD_COLOR], sizeof bcol);
	}

	glBegin(GL_LINES);
	glColor4fv(tcol);
	glVertex2f(x, y + h);
	glVertex2f(x, y);
	glVertex2f(x, y);
	glVertex2f(x + w, y);
	glColor4fv(bcol);
	glVertex2f(x + w, y);
	glVertex2f(x + w, y + h);
	glVertex2f(x + w, y + h);
	glVertex2f(x, y + h);
	glEnd();

}

void imtk_draw_string(int x, int y, const char *str)
{
	glRasterPos2i(x, y);
	while(*str) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *str++);
	}
}

int imtk_string_size(const char *str)
{
	return glutBitmapLength(GLUT_BITMAP_HELVETICA_12, (const unsigned char*)str);
}
