#include <stdio.h>
#include <assert.h>
#include "imtk.h"
#include "state.h"
#include "draw.h"

#define SLIDER_SIZE		100
#define THUMB_WIDTH		10
#define THUMB_HEIGHT	20

static void draw_slider(int id, float pos, float min, float max, int x, int y);

float imtk_slider(int id, float pos, float min, float max, int x, int y)
{
	int mousex, mousey, thumb_x, thumb_y, over = 0;
	float range = max - min;

	assert(id >= 0);

	imtk_get_mouse(&mousex, &mousey);

	pos = (pos - min) / range;
	if(pos < 0.0) pos = 0.0;
	if(pos > 1.0) pos = 1.0;

	thumb_x = x + SLIDER_SIZE * pos - THUMB_WIDTH / 2;
	thumb_y = y - THUMB_HEIGHT / 2;

	if(imtk_hit_test(thumb_x, thumb_y, THUMB_WIDTH, THUMB_HEIGHT)) {
		imtk_set_hot(id);
		over = 1;
	}

	if(imtk_button_state(IMTK_LEFT_BUTTON)) {
		if(over && imtk_is_hot(id)) {
			if(!imtk_is_active(id)) {
				imtk_set_prev_mouse(mousex, mousey);
			}
			imtk_set_active(id);
		}
	} else {
		if(imtk_is_active(id)) {
			imtk_set_active(-1);
		}
	}

	if(imtk_is_active(id)) {
		int prevx;
		float dx;

		imtk_get_prev_mouse(&prevx, 0);

		dx = (float)(mousex - prevx) / (float)SLIDER_SIZE;
		pos += dx;
		imtk_set_prev_mouse(mousex, mousey);

		if(pos < 0.0) pos = 0.0;
		if(pos > 1.0) pos = 1.0;
	}

	draw_slider(id, pos, min, max, x, y);
	return pos * range + min;
}


static void draw_slider(int id, float pos, float min, float max, int x, int y)
{
	float range = max - min;
	int thumb_x, thumb_y;
	char buf[32];

	thumb_x = x + SLIDER_SIZE * pos - THUMB_WIDTH / 2;
	thumb_y = y - THUMB_HEIGHT / 2;

	/* draw trough */
	imtk_draw_rect(x, y - 2, SLIDER_SIZE, 4, imtk_get_color(IMTK_BASE_COLOR));
	imtk_draw_frame(x, y - 2, SLIDER_SIZE, 4, FRAME_INSET);

	if(imtk_hit_test(thumb_x, thumb_y, THUMB_WIDTH, THUMB_HEIGHT)) {
		glColor4fv(imtk_get_color(IMTK_FOCUS_COLOR));
	} else {
		glColor4fv(imtk_get_color(IMTK_BASE_COLOR));
	}

	/* draw handle */
	imtk_draw_rect(thumb_x, thumb_y, THUMB_WIDTH, THUMB_HEIGHT, 0);
	imtk_draw_frame(thumb_x, thumb_y, THUMB_WIDTH, THUMB_HEIGHT, FRAME_OUTSET);

	/* draw display */
	sprintf(buf, "%.3f", pos * range + min);
	glColor4fv(imtk_get_color(IMTK_TEXT_COLOR));
	imtk_draw_string(x + SLIDER_SIZE + THUMB_WIDTH / 2 + 2, y + 4, buf);
}

