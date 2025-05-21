# MoonCon Architecture

## TIC-80
```
DISPLAY     240x136 pixels, 16 colors palette.
INPUT       4 gamepads with 8 buttons / mouse / keyboard.
SPRITES     256 8x8 tiles and 256 8x8 sprites.
MAP         240x136 cells, 1920x1088 pixels.
SOUND       4 channels with configurable waveforms.
CODE        64KB of lua, ruby, js, moon, fennel, scheme, squirrel, wren, wasm, janet or python.
```

## CHIP-8
```
DISPLAY    64x32 monochrome  
CART SIZE  ~3584 bytes usable  
SOUND      1 channel buzzer  
CODE       Hex opcodes (interpreter-driven)  
CPU        ~500 Hz (on original 1970s hardware)  
SPRITES    16 built-in (4x5 px) + custom XORed to screen  
MAP        N/A (manual tile handling via memory)
```

## PICO-8
```
DISPLAY 	128x128 16 colours
CART SIZE 	32k
SOUND 		4 channel chip blerps
CODE 		P8 Lua
CPU 		4M vm insts/sec
SPRITES 	256 8x8 sprites
MAP 		128x32 tiles
```

## MEG-4
```
RAM	        576 KiB
MMIO	    192 KiB
SCREEN 	    320 x 200 x 8bpp (the famous DOS resolution)
PALETTE    	256, 32 bit ARGB
SPRITES     1024, each 8 x 8 pixels
MAP 	    320 x 200 tiles (64 screens)
SOUND 	    8 bit, 44100 Hz, 31 waveforms, 64 sound effects
MUSIC 	    8 music tracks, each 1024 patterns long, 4 channels
CODEPAGE 	UTF-8 encoded UNICODE (BMP only, U+0000 to U+FFFF)
```
