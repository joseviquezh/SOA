
all: WriterTest.c
	@gcc -o writertest WriterTest.c Writer.c
	@echo writertest build done
	@./writertest
	@sudo apt-get install wiki2beamer
	@pdflatex main.tex
	
