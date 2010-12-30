#ifndef IMTK_H_
#define IMTK_H_

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

void imtk_inp_key(int key, int state);
void imtk_inp_mouse(int bn, int state);
void imtk_inp_motion(int x, int y);
void imtk_inp_reshape(int x, int y);

void imtk_begin(void);
void imtk_end(void);

int imtk_button(int id, const char *label, int x, int y);

#endif	/* IMTK_H_ */
