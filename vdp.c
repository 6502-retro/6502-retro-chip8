#include <stdint.h>
#include "vdp.h"

void set_read_addr(uint16_t addr) {
    VDP_REG = addr & 0x00FF;
    VDP_REG = addr >> 8;
}

void set_write_addr(uint16_t addr) {
    VDP_REG = addr & 0x00FF;
    VDP_REG = addr >> 8 | 0x40;
}

void plot_xy(uint8_t x, uint8_t y, uint8_t c) {
    uint8_t dot;
    char pix;
    uint16_t addr = PATTERN_TABLE + 8 * (x / 2) + y % 8 + 256 * (y / 8);
    set_read_addr(addr);
    dot = VDP_RAM;
    set_write_addr(addr);
    if (x & 1) // Odd columns
            pix = ((dot & 0xF0) + (c & 0x0f));
        else
            pix = ((dot & 0x0F) + (c << 4));
    VDP_RAM = pix;
}


