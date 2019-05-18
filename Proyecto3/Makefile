all: main.c
	gcc -o main main.c gui.h task.h algorithms/RM.c test/test.c `(pkg-config --cflags --libs gtk+-3.0 gmodule-2.0)` -lm
clean:
	@rm -f *.o main
	@echo Clean done
