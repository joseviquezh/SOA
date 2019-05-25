all: main.c
	gcc -o main main.c task.h test/test.c scheduler/scheduler.c gui.h `(pkg-config --cflags --libs gtk+-3.0 gmodule-2.0)` -lm
clean:
	@rm -f *.o main
	@echo Clean done
