#include <string.h>
#include <assert.h>
#include "draw.h"
#include "imtk.h"

#define COLOR_MASK	0xff

/* default colors, can be changed with imtk_set_color */
static float colors[][4] = {
	{0.0, 0.0, 0.0, 1.0},		/* text color */
	{0.75, 0.75, 0.75, 1.0},	/* top color */
	{0.56, 0.56, 0.56, 1.0},	/* bot color */
	{0.9, 0.9, 0.9, 1.0},		/* lit bevel */
	{0.3, 0.3, 0.3, 1.0},		/* shadowed bevel */
	{0.8, 0.25, 0.18, 1.0},		/* cursor color */
	{0.68, 0.85, 1.3, 1.0},		/* selection color */
	{0.75, 0.1, 0.095, 1.0}		/* check color */
};

static float focus_factor = 1.15;
static float press_factor = 0.8;
static float alpha = 1.0;
static float bevel = 1.0;

void imtk_set_color(unsigned int col, float r, float g, float b, float a)
{
	int idx = col & COLOR_MASK;
	assert(idx >= 0 && idx < sizeof colors / sizeof *colors);

	colors[idx][0] = r;
	colors[idx][1] = g;
	colors[idx][2] = b;
	colors[idx][3] = a;
}

float *imtk_get_color(unsigned int col)
{
	static float ret[4];
	int idx = col & COLOR_MASK;

	memcpy(ret, colors + idx, sizeof ret);
	if(col & IMTK_FOCUS_BIT) {
		ret[0] *= focus_factor;
		ret[1] *= focus_factor;
		ret[2] *= focus_factor;
	}
	if(col & IMTK_PRESS_BIT) {
		ret[0] *= press_factor;
		ret[1] *= press_factor;
		ret[2] *= press_factor;
	}
	if(col & IMTK_SEL_BIT) {
		ret[0] *= colors[IMTK_SELECTION_COLOR][0];
		ret[1] *= colors[IMTK_SELECTION_COLOR][1];
		ret[2] *= colors[IMTK_SELECTION_COLOR][2];
	}
	ret[3] *= alpha;
	return ret;
}

void imtk_set_alpha(float a)
{
	alpha = a;
}

float imtk_get_alpha(void)
{
	return alpha;
}

void imtk_set_bevel_width(float b)
{
	bevel = b;
}

float imtk_get_bevel_width(void)
{
	return bevel;
}

void imtk_set_focus_factor(float fact)
{
	focus_factor = fact;
}

float imtk_get_focus_factor(void)
{
	return focus_factor;
}

void imtk_set_press_factor(float fact)
{
	press_factor = fact;
}

float imtk_get_press_factor(void)
{
	return press_factor;
}

void imtk_draw_rect(int x, int y, int w, int h, float *ctop, float *cbot)
{
	glBegin(GL_QUADS);
	if(ctop) {
		glColor4fv(ctop);
	}
	glVertex2f(x, y);
	glVertex2f(x + w, y);

	if(cbot) {
		glColor4fv(cbot);
	}
	glVertex2f(x + w, y + h);
	glVertex2f(x, y + h);
	glEnd();
}

void imtk_draw_frame(int x, int y, int w, int h, int style)
{
	float tcol[4], bcol[4];
	float b = imtk_get_bevel_width();

	if(!b) {
		return;
	}

	x -= b;
	y -= b;
	w += b * 2;
	h += b * 2;

	switch(style) {
	case FRAME_INSET:
		memcpy(tcol, imtk_get_color(IMTK_BEVEL_SHAD_COLOR), sizeof tcol);
		memcpy(bcol, imtk_get_color(IMTK_BEVEL_LIT_COLOR), sizeof bcol);
		break;

	case FRAME_OUTSET:
	default:
		memcpy(tcol, imtk_get_color(IMTK_BEVEL_LIT_COLOR), sizeof tcol);
		memcpy(bcol, imtk_get_color(IMTK_BEVEL_SHAD_COLOR), sizeof bcol);
	}

	glBegin(GL_QUADS);
	glColor4fv(tcol);
	glVertex2f(x, y);
	glVertex2f(x + b, y + b);
	glVertex2f(x + w - b, y + b);
	glVertex2f(x + w, y);

	glVertex2f(x + b, y + b);
	glVertex2f(x, y);
	glVertex2f(x, y + h);
	glVertex2f(x + b, y + h - b);

	glColor4fv(bcol);
	glVertex2f(x + b, y + h - b);
	glVertex2f(x + w - b, y + h - b);
	glVertex2f(x + w, y + h);
	glVertex2f(x, y + h);

	glVertex2f(x + w - b, y + b);
	glVertex2f(x + w, y);
	glVertex2f(x + w, y + h);
	glVertex2f(x + w - b, y + h - b);
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
