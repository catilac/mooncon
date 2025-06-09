CC = gcc
PKGS = lua5.4 sdl3 sdl3-ttf

CFLAGS = $(shell pkg-config --cflags $(PKGS))
LDFLAGS = $(shell pkg-config --libs $(PKGS)) -lm -ldl

SRC = $(wildcard src/*.c)
OUT = mooncon

$(OUT): $(SRC)
	$(CC) $(CFLAGS) -o $(OUT) $(SRC) $(LDFLAGS)

clean:
	rm -f $(OUT)
