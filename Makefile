CC = gcc
CFLAGS = -I/usr/include/lua5.4 -I/usr -I/usr/local/include/SDL3
LDFLAGS = -llua5.4 -lm -ldl $(shell pkg-config sdl3 --libs)

SRC = main.c ilua.c moon_context.c moonlang.c
OUT = mooncon

$(OUT): $(SRC)
	$(CC) $(CFLAGS) -o $(OUT) $(SRC) $(LDFLAGS)

clean:
	rm -f $(OUT)
