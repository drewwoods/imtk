#include <assert.h>
#include "imtk.h"
#include "state.h"
#include "draw.h"


#define CHECKBOX_SIZE	14


static void draw_checkbox(int id, const char *label, int x, int y, int state, int over);

int imtk_checkbox(int id, const char *label, int x, int y, int state)
{
	int sz = CHECKBOX_SIZE;
	int over = 0;

	assert(id >= 0);

	if(imtk_hit_test(x, y, sz + imtk_string_size(label) + 5, sz)) {
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
				state = !state;
			}
		}
	}

	draw_checkbox(id, label, x, y, state, over);
	return state;
}

static void draw_checkbox(int id, const char *label, int x, int y, int state, int over)
{
	static const int sz = CHECKBOX_SIZE;

	if(over) {
		glColor4fv(imtk_get_color(IMTK_FOCUS_COLOR));
	} else {
		glColor4fv(imtk_get_color(IMTK_BASE_COLOR));
	}

	imtk_draw_rect(x, y, sz, sz, 0);
	imtk_draw_frame(x, y, sz, sz, FRAME_INSET);

	glColor4fv(imtk_get_color(IMTK_TEXT_COLOR));
	if(state) {
		glPushAttrib(GL_LINE_BIT);
		glLineWidth(2);

		glBegin(GL_LINES);
		glVertex2f(x + 2, y + 2);
		glVertex2f(x + sz - 2, y + sz - 2);
		glVertex2f(x + sz - 2, y + 2);
		glVertex2f(x + 2, y + sz - 2);
		glEnd();

		glPopAttrib();
	}

	imtk_draw_string(x + sz + 5, y + sz - 2, label);
}

