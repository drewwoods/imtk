#ifndef IMTK_H_
#define IMTK_H_

#define IMUID	(65536 + __LINE__)

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

void imtk_window(int id, const char *title, int x, int y, int width, int height);
int imtk_button(int id, const char *label, int x, int y);
int imtk_checkbox(int id, const char *label, int x, int y, int state);
void imtk_textbox(int id, char *textbuf, size_t buf_sz, int x, int y);

#endif	/* IMTK_H_ */
