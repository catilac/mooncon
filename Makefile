CC = gcc
CFLAGS = -I/usr/include/lua5.4 -I/usr -I/usr/local/include/SDL3

LDFLAGS = -llua5.4 -lm -ldl
LDFLAGS += $(shell pkg-config sdl3 --libs)

SRC = main.c
OUT = mooncon

$(OUT): $(SRC)
	$(CC) $(CFLAGS) -o $(OUT) $(SRC) $(LDFLAGS)

hello: hello.c
	$(CC) $(CFLAGS) -o hello hello.c $(LDFLAGS)

clean:
	rm -f $(OUT)
