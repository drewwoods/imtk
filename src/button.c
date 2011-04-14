#include <assert.h>
#include "imtk.h"
#include "state.h"
#include "draw.h"

static void calc_button_size(const char *label, int *wret, int *hret);
static void draw_button(int id, const char *label, int x, int y);

int imtk_button(int id, const char *label, int x, int y)
{
	int w, h, res = 0;
	int over = 0;

	assert(id >= 0);

	calc_button_size(label, &w, &h);

	if(imtk_hit_test(x, y, w, h)) {
		imtk_set_hot(id);
		over = 1;
	}

	if(imtk_button_state(IMTK_LEFT_BUTTON)) {
		if(over) {
			imtk_set_active(id);
		}
	} else { /* mouse button up */
		if(imtk_is_active(id)) {
			imtk_set_active(-1);
			if(imtk_is_hot(id) && over) {
				res = 1;
			}
		}
	}

	draw_button(id, label, x, y);
	return res;
}

static void draw_button(int id, const char *label, int x, int y)
{
	int width, height;

	calc_button_size(label, &width, &height);

	glBegin(GL_QUADS);
	if(imtk_hit_test(x, y, width, height)) {
		glColor4fv(imtk_get_color(IMTK_FOCUS_COLOR));
	} else {
		glColor4fv(imtk_get_color(IMTK_BASE_COLOR));
	}
	glVertex2f(x, y);
	glVertex2f(x + width, y);
	glVertex2f(x + width, y + height);
	glVertex2f(x, y + height);
	glEnd();

	imtk_draw_frame(x, y, width, height, imtk_is_active(id) ? FRAME_INSET : FRAME_OUTSET);

	glColor4fv(imtk_get_color(IMTK_TEXT_COLOR));
	imtk_draw_string(x + 20, y + 15, label);
}

static void calc_button_size(const char *label, int *wret, int *hret)
{
	int strsz = imtk_string_size(label);
	if(wret) *wret = strsz + 40;
	if(hret) *hret = 20;
}
