CC = gcc
CFLAGS = -Wall -ansi
DEPS = bgpcounter.h mycmds.h mydefs.h myutils.h
OBJ = main.o bgpcounter.o mycmds.o myutils.o
EXENAME = rsi

$(EXENAME): $(OBJ)
	$(CC) -o $@ $^

%.o: %.c $(DEPS)
	$(CC) -c $(CFLAGS) $<

clean:
	rm -f $(EXENAME) $(OBJ)
