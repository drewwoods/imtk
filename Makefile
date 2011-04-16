src = $(wildcard *.c src/*.c)
obj = $(src:.c=.o)
bin = test

CC = gcc
CFLAGS = -pedantic -Wall -g -Isrc
LDFLAGS = $(libgl)

ifeq ($(shell uname -s), Darwin)
	libgl = -framework OpenGL -framework GLUT
else
	libgl = -lGL -lGLU -lglut
endif

$(bin): $(obj)
	$(CC) -o $@ $(obj) $(LDFLAGS)

.PHONY: clean
clean:
	rm -f $(obj) $(bin)
