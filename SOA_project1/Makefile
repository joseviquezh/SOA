CC=gcc
CFLAGS=`pkg-config --cflags gtk+-2.0`
LDFLAGS=`pkg-config --libs gtk+-2.0`

UISOURCES=main.c
LOTTERYSOURCES=lottery.c
SERIESOURCES=serie.c

UIEXECUTABLE=interfaz
LOTTERYEXECUTABLE=loterry
SERIEEXECUTABLE=serie

build:
	$(CC) -Wall -g $(UISOURCES) -o $(UIEXECUTABLE) $(CFLAGS) $(LDFLAGS) $(QOUT)
	$(CC) -Wall -g $(LOTTERYSOURCES) -o $(LOTTERYEXECUTABLE) $(CFLAGS) $(LDFLAGS) $(QOUT)
	$(CC) -Wall -g $(SERIESOURCES) -o $(SERIEEXECUTABLE) $(CFLAGS) $(LDFLAGS) $(QOUT)

clean:
	@echo "Cleaning directory"
	@rm -f *.o *~ $(UIEXECUTABLE) $(LOTTERYEXECUTABLE) $(SERIEEXECUTABLE)
	@echo "Ready."
