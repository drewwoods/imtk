#include "imtk.h"
#include "draw.h"

#define PROGR_SIZE             100
#define PROGR_HEIGHT			15

static void draw_progress(int id, float pos, int x, int y);

void imtk_progress(int id, float pos, int x, int y)
{
	draw_progress(id, pos, x, y);
}

static void draw_progress(int id, float pos, int x, int y)
{
	int bar_size = PROGR_SIZE * pos;
	float b = imtk_get_bevel_width();

	if(pos < 0.0) pos = 0.0;
	if(pos > 1.0) pos = 1.0;

	/* through */
	imtk_draw_rect(x - b, y - b, PROGR_SIZE + b * 2, PROGR_HEIGHT + b * 2, imtk_get_color(IMTK_BASE_COLOR));
	imtk_draw_frame(x - b, y - b, PROGR_SIZE + b * 2, PROGR_HEIGHT + b * 2, FRAME_INSET);

	if(pos > 0.0) {
		/* bar */
		imtk_draw_rect(x, y, bar_size, PROGR_HEIGHT, imtk_get_color(IMTK_BASE_COLOR));
		imtk_draw_frame(x, y, bar_size, PROGR_HEIGHT, FRAME_OUTSET);
	}
}
