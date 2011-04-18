#include <string.h>
#include <ctype.h>
#include <assert.h>
#include "imtk.h"
#include "state.h"
#include "draw.h"

#define TEXTBOX_SIZE	100

static void draw_textbox(int id, const char *text, int x, int y);


void imtk_textbox(int id, char *textbuf, size_t buf_sz, int x, int y)
{
	int len, over = 0;

	assert(id >= 0);

	if(imtk_hit_test(x, y, TEXTBOX_SIZE, 20)) {
		imtk_set_hot(id);
		over = 1;
	}

	if(imtk_button_state(IMTK_LEFT_BUTTON)) {
		if(over) {
			imtk_set_active(id);
		}
	} else {
		if(imtk_is_active(id)) {
			imtk_set_active(-1);
			if(imtk_is_hot(id) && over) {
				imtk_set_focus(id);
			}
		}
	}

	if(imtk_has_focus(id)) {
		int key;
		len = strlen(textbuf);

		while((key = imtk_get_key()) != -1) {
			if(isprint(key)) {
				if(len < buf_sz) {
					textbuf[len++] = (char)key;
				}
			} else {
				switch(key) {
				case '\b':
					if(len > 0) {
						textbuf[--len] = 0;
					}
					break;

				default:
					break;
				}
			}
		}
	}

	draw_textbox(id, textbuf, x, y);
}


static void draw_textbox(int id, const char *text, int x, int y)
{
	int strsz = imtk_string_size(text);

	if(imtk_hit_test(x, y, TEXTBOX_SIZE, 20)) {
		glColor4fv(imtk_get_color(IMTK_FOCUS_COLOR));
	} else {
		glColor4fv(imtk_get_color(IMTK_BASE_COLOR));
	}

	imtk_draw_rect(x, y, TEXTBOX_SIZE, 20, 0);

	if(imtk_has_focus(id)) {
		glBegin(GL_LINES);
		glColor4fv(imtk_get_color(IMTK_CURSOR_COLOR));
		glVertex2f(x + strsz + 3, y + 2);
		glVertex2f(x + strsz + 3, y + 18);
		glVertex2f(x + strsz + 4, y + 2);
		glVertex2f(x + strsz + 4, y + 18);
		glEnd();
	}

	glColor4fv(imtk_get_color(IMTK_TEXT_COLOR));
	imtk_draw_string(x + 2, y + 15, text);

	imtk_draw_frame(x, y, TEXTBOX_SIZE, 20, FRAME_INSET);
}

