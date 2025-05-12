CC = gcc
CFLAGS = -I/usr/include/lua5.4 -I/usr -I/usr/local/include/SDL3 -I/usr/local/include/SDL3_ttf
LDFLAGS = -llua5.4 -lm -ldl $(shell pkg-config sdl3 --libs) $(shell pkg-config sdl3-ttf --libs)

SRC = $(wildcard src/*.c)
OUT = mooncon

$(OUT): $(SRC)
	$(CC) $(CFLAGS) -o $(OUT) $(SRC) $(LDFLAGS)

clean:
	rm -f $(OUT)
