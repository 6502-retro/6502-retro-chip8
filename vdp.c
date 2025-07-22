#include <stdint.h>
#include <string.h>
#include "vdp.h"

uint16_t i = 0;
char *framebuf = &FRAMEBUF;

void vdp_off() {
    VDP_REG=0xA8;
    VDP_REG=0x81;
}

void vdp_on() {
    VDP_REG=0xE8;
    VDP_REG=0x81;
}

void vdp_plot_xy(uint8_t x, uint8_t y, uint8_t c) {
    uint8_t dot;
    char pix;
    uint16_t addr = 8 * (x / 2) + y % 8 + 256 * (y / 8);
    dot = framebuf[addr];
    if (x & 1) // Odd columns
            pix = ((dot & 0xF0) + (c & 0x0f));
        else
            pix = ((dot & 0x0F) + (c << 4));
    framebuf[addr] = pix;
}

void vdp_wait() {
    while (VDP_REG >> 7) {
        ;;
    }
}

void vdp_flush() {
    vdp_set_write_addr(PATTERN_TABLE);
    for (i=0; i<0xC00; ++i) {
        VDP_RAM = framebuf[i];
    }
}

// Set all pixels to black
void vdp_colorize(uint8_t c) {
    uint8_t b = c<<4 | c;
    memset(framebuf, b, 0xC00);
}

// vim: ts=4 sw=4 et:
