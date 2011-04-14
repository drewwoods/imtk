#include "imtk.h"
#include "draw.h"

#define SLIDER_SIZE             100

static void draw_progress(int id, float pos, int x, int y);

void imtk_progress(int id, float pos, int x, int y)
{
	draw_progress(id, pos, x, y);
}

static void draw_progress(int id, float pos, int x, int y)
{
	int bar_size = SLIDER_SIZE * pos;

	if(pos < 0.0) pos = 0.0;
	if(pos > 1.0) pos = 1.0;

	/* through */
	glBegin(GL_QUADS);
	glColor4fv(imtk_get_color(IMTK_BASE_COLOR));
	glVertex2f(x - 1, y - 1);
	glVertex2f(x + SLIDER_SIZE + 1, y - 1);
	glVertex2f(x + SLIDER_SIZE + 1, y + 17);
	glVertex2f(x - 1, y + 17);
	glEnd();
	imtk_draw_frame(x - 1, y - 1, SLIDER_SIZE + 2, 17, FRAME_INSET);

	if(pos > 0.0) {
		/* bar */
		glBegin(GL_QUADS);
		glColor4fv(imtk_get_color(IMTK_BASE_COLOR));
		glVertex2f(x, y);
		glVertex2f(x + bar_size, y);
		glVertex2f(x + bar_size, y + 15);
		glVertex2f(x, y + 15);
		glEnd();
		imtk_draw_frame(x, y, bar_size, 15, FRAME_OUTSET);
	}
}
