// vim: ts=4 sw=4 tw=80 cc=80:
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "vdp.h"
#include "chip8.h"

uint8_t hexfont[80] = {
	0xF0, 0x90, 0x90, 0x90, 0xF0,	// 0
	0x20, 0x60, 0x20, 0x20, 0x70,	// 1
	0xF0, 0x10, 0xF0, 0x80, 0xF0,	// 2
	0xF0, 0x10, 0xF0, 0x10, 0xF0,	// 3
	0x90, 0x90, 0xF0, 0x10, 0x10,	// 4
	0xF0, 0x80, 0xF0, 0x10, 0xF0,	// 5
	0xF0, 0x80, 0xF0, 0x90, 0xF0,	// 6
	0xF0, 0x10, 0x20, 0x40, 0x40,	// 7
	0xF0, 0x90, 0xF0, 0x90, 0xF0,	// 8
	0xF0, 0x90, 0xF0, 0x10, 0xF0,	// 9
	0xF0, 0x90, 0xF0, 0x90, 0x90,	// A
	0xE0, 0x90, 0xE0, 0x90, 0xE0,	// B
	0xF0, 0x80, 0x80, 0x80, 0xF0,	// C
	0xE0, 0x90, 0x90, 0x90, 0xE0,	// D
	0xF0, 0x80, 0xF0, 0x80, 0xF0,	// E
	0xF0, 0x80, 0xF0, 0x80, 0x80 	// F
};

Chip8* chip8_init() {
	Chip8 *chip = (Chip8*)malloc(sizeof(struct s_chip8));
	if (chip == NULL)
		exit(-1);
	else {
		memset(chip, 0, sizeof(Chip8));
		memcpy(chip->ram, hexfont, 80);
		chip->pc = 0x200;
		return chip;
	}
};

void chip8_destroy(Chip8 *chip) {
	free(chip);
}

// write a pixel into the frame buffer
// XOR the value that's there.
void chip8_plot_xy(Chip8 *chip, uint8_t x, uint8_t y, bool p) {
	uint16_t addr = (y*64)+x;
	uint8_t b = chip->fb[addr];

	chip->fb[addr] = b^p;
	if (!chip->fb[addr])
		chip->v[0xF] = 1;
	else
		chip->v[0xF] = 0;
}

void chip8_render(Chip8 *chip) {
	// chip->fb contains 64x32 pixels.
	uint8_t x = 0;
	uint8_t y = 0;
	uint16_t addr = 0;
	for (x=0; x<64; ++x) {
		for (y=0; y<32; ++y) {
			addr = (y*64)+x;
			if (chip->fb[addr]) {
				vdp_plot_xy(x, y+8, VDP_WHITE);
			}
			else {
				vdp_plot_xy(x, y+8, VDP_BLACK);
			}
		}
	}
}

void chip8_clear(Chip8 *chip) {
	memset(chip->fb, 0, 2048);
	chip8_render(chip);
	vdp_wait();
	vdp_flush();
}
