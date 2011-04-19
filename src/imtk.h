#ifndef IMTK_H_
#define IMTK_H_

#include <stdlib.h>


#define IMUID			(__LINE__ << 10)
#define IMUID_IDX(i)	((__LINE__ << 10) + ((i) << 1))


/* key/button state enum */
enum {
	IMTK_UP,
	IMTK_DOWN
};

enum {
	IMTK_LEFT_BUTTON,
	IMTK_MIDDLE_BUTTON,
	IMTK_RIGHT_BUTTON
};

enum {
	IMTK_TEXT_COLOR,
	IMTK_TOP_COLOR,
	IMTK_BOTTOM_COLOR,
	IMTK_BEVEL_LIT_COLOR,
	IMTK_BEVEL_SHAD_COLOR,
	IMTK_CURSOR_COLOR,
	IMTK_SELECTION_COLOR,
	IMTK_CHECK_COLOR
};

#define IMTK_FOCUS_BIT		0x100
#define IMTK_PRESS_BIT		0x200

#define IMTK_BASE_COLOR		IMTK_BOTTOM_COLOR
#define IMTK_FOCUS_COLOR	(IMTK_TOP_COLOR | IMTK_FOCUS_BIT)


#ifdef __cplusplus
extern "C" {
#endif


void imtk_inp_key(int key, int state);
void imtk_inp_mouse(int bn, int state);
void imtk_inp_motion(int x, int y);

void imtk_set_viewport(int x, int y);
void imtk_get_viewport(int *width, int *height);

void imtk_post_redisplay(void);

void imtk_begin(void);
void imtk_end(void);

void imtk_label(const char *str, int x, int y);
int imtk_button(int id, const char *label, int x, int y);
int imtk_checkbox(int id, const char *label, int x, int y, int state);
void imtk_textbox(int id, char *textbuf, size_t buf_sz, int x, int y);
float imtk_slider(int id, float pos, float min, float max, int x, int y);
void imtk_progress(int id, float pos, int x, int y);
int imtk_listbox(int id, const char *list, int sel, int x, int y);
/*
int imtk_combobox(int id, char *textbuf, size_t buf_sz, const char *list, int sel, int x, int y);
*/

/* helper functions to create and destroy item lists for listboxes and comboboxes */
char *imtk_create_list(const char *first, ...);
void imtk_free_list(char *list);

/* defined in draw.c */
void imtk_set_color(unsigned int col, float r, float g, float b, float a);
float *imtk_get_color(unsigned int col);
void imtk_set_alpha(float a);
float imtk_get_alpha(void);
void imtk_set_bevel_width(float b);
float imtk_get_bevel_width(void);
void imtk_set_focus_factor(float fact);
float imtk_get_focus_factor(void);
void imtk_set_press_factor(float fact);
float imtk_get_press_factor(void);

#ifdef __cplusplus
}
#endif

#endif	/* IMTK_H_ */
