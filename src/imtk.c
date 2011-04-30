#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#ifndef __APPLE__
#include <GL/glut.h>
#else
#include <GLUT/glut.h>
#endif
#include "imtk.h"
#include "state.h"
#include "draw.h"


void imtk_post_redisplay(void)
{
	glutPostRedisplay();
}

void imtk_begin(void)
{
	int width, height;

	glPushAttrib(GL_ENABLE_BIT | GL_COLOR_BUFFER_BIT | GL_TRANSFORM_BIT);

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_STENCIL_TEST);
	glDisable(GL_ALPHA_TEST);
	glDisable(GL_TEXTURE_1D);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_CULL_FACE);
	glDisable(GL_SCISSOR_TEST);
	glDisable(GL_LIGHTING);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	imtk_get_viewport(&width, &height);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glTranslatef(-1, 1, 0);
	glScalef(2.0 / width, -2.0 / height, 1.0);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
}

void imtk_end(void)
{
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	glPopAttrib();
}

void imtk_label(const char *str, int x, int y)
{
	if(x == IMTK_AUTO || y == IMTK_AUTO) {
		imtk_layout_get_pos(&x, &y);
	}

	glColor4fv(imtk_get_color(IMTK_TEXT_COLOR));
	imtk_draw_string(x, y + 14, str);
	imtk_layout_advance(imtk_string_size(str), 12);
}


static int box[4], span[4];
static int spacing;
static int layout;

void imtk_layout_start(int x, int y, int sp, int dir)
{
	box[0] = span[0] = x;
	box[1] = span[1] = y;
	box[2] = box[3] = span[2] = span[3] = 0;
	spacing = sp;
	layout = dir;
}

void imtk_layout_dir(int dir)
{
	layout = dir;
	if(dir == IMTK_VERTICAL) {
		imtk_layout_newline();
	}
}

void imtk_layout_advance(int width, int height)
{
	int max_span_y, max_box_y;

	span[2] += width + spacing;

	if(height > span[3]) {
		span[3] = height;
	}

	max_span_y = span[1] + span[3];
	max_box_y = box[1] + box[3];

	if(max_span_y > max_box_y) {
		box[3] = max_span_y - box[1];
	}
	if(span[2] > box[2]) {
		box[2] = span[2];
	}

	if(layout == IMTK_VERTICAL) {
		imtk_layout_newline();
	}
}

void imtk_layout_newline(void)
{
	span[0] = box[0];
	span[1] = box[1] + box[3] + spacing;
	span[2] = span[3] = 0;
}

void imtk_layout_get_pos(int *x, int *y)
{
	*x = span[0] + span[2];
	*y = span[1];
}

void imtk_layout_get_bounds(int *bbox)
{
	memcpy(bbox, box, sizeof box);
}
