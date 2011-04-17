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

#ifdef __cplusplus
}
#endif

#endif	/* IMTK_H_ */
