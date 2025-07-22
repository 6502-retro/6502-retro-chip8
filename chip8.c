// vim: ts=4 sw=4 tw=80 cc=80:
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "vdp.h"
#include "chip8.h"

Chip8* chip8_init() {
	Chip8 *chip = (Chip8*)malloc(sizeof(struct s_chip8));
	if (chip == NULL)
		exit(-1);
	else {
		memset(chip->fb, 0, 2048);
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
}

void chip8_render(Chip8 *chip) {
	// chip->fb contains 64x32 pixels.
	uint8_t x = 0;
	uint8_t y = 0;
	uint16_t addr = 0;
	for (x=0; x<64; ++x) {
		for (y=0; y<32; ++y) {
			addr = (y*64)+x;
			if (chip->fb[addr])
				vdp_plot_xy(x, y+8, VDP_WHITE);
			else
				vdp_plot_xy(x, y+8, VDP_BLACK);
		}
	}
}

void chip8_clear(Chip8 *chip) {
	memset(chip->fb, 0, 2048);
	chip8_render(chip);
	vdp_wait();
	vdp_flush();
}
