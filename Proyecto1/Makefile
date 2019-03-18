all: main.c
	gcc -o main main.c gui.h timer/timer.c `(pkg-config --cflags --libs gtk+-3.0 gmodule-2.0)` -lm
