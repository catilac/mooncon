CC = gcc
CFLAGS = -I/usr/include/lua5.4
LDFLAGS = -llua5.4 -lm -ldl
SRC = main.c
OUT = mooncon

$(OUT): $(SRC)
	$(CC) $(CFLAGS) -o $(OUT) $(SRC) $(LDFLAGS)

clean:
	rm -f $(OUT)
