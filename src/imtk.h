#ifndef IMTK_H_
#define IMTK_H_

#define IMUID			(__LINE__ << 10)
#define IMUID_IDX(i)	((__LINE__ << 10) + ((i) << 1))

enum {
	IMTK_TEXT_COLOR,
	IMTK_BASE_COLOR,
	IMTK_FOCUS_COLOR,
	IMTK_BEVEL_LIT_COLOR,
	IMTK_BEVEL_SHAD_COLOR
};

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

void imtk_set_color(int col, float r, float g, float b);

void imtk_inp_key(int key, int state);
void imtk_inp_mouse(int bn, int state);
void imtk_inp_motion(int x, int y);
void imtk_inp_reshape(int x, int y);

void imtk_begin(void);
void imtk_end(void);

int imtk_button(int id, const char *label, int x, int y);
int imtk_checkbox(int id, const char *label, int x, int y, int state);
void imtk_textbox(int id, char *textbuf, size_t buf_sz, int x, int y);
float imtk_slider(int id, float pos, float min, float max, int x, int y);
void imtk_progress(int id, float pos, int x, int y);
int imtk_listbox(int id, const char *list, int sel, int x, int y);
int imtk_combobox(int id, char *textbuf, size_t buf_sz, const char *list, int sel, int x, int y);

/* helper functions to create and destroy item lists for listboxes and comboboxes */
char *imtk_create_list(const char *first, ...);
void imtk_free_list(char *list);

#endif	/* IMTK_H_ */
