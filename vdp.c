#include <stdint.h>
#include "vdp.h"


void vdp_set_read_addr(uint16_t addr) {
    VDP_REG = addr & 0x00FF;
    VDP_REG = addr >> 8;
}

void vdp_set_write_addr(uint16_t addr) {
    VDP_REG = addr & 0x00FF;
    VDP_REG = addr >> 8 | 0x40;
}

void vdp_plot_xy(uint8_t x, uint8_t y, uint8_t c) {
    uint8_t dot;
    char pix;
    uint16_t addr = PATTERN_TABLE + 8 * (x / 2) + y % 8 + 256 * (y / 8);
    vdp_set_read_addr(addr);
    dot = VDP_RAM;
    vdp_set_write_addr(addr);
    if (x & 1) // Odd columns
            pix = ((dot & 0xF0) + (c & 0x0f));
        else
            pix = ((dot & 0x0F) + (c << 4));
    VDP_RAM = pix;
}

void vdp_wait() {
    while (VDP_REG >> 7) {
        ;;
    }
}
/*
VDP_16K                 = %10000000
VDP_BLANK_SCREEN        = %01000000
VDP_INTERRUPT_ENABLE    = %00100000
VDP_M2                  = %00001000
                           11101000
                           E8 BLANK OFF
                           10101000
                           A8 BLANK ON
*/
// Set all pixels to black
void vdp_colorize(uint8_t c) {
    uint16_t i = 0;
    VDP_REG=0xA8;
    VDP_REG=0x81;

    vdp_set_write_addr(PATTERN_TABLE);
    for (i=0; i<0xC00; i++) {
        VDP_RAM = (c<<4)|c;
    }
    vdp_wait();
    VDP_REG=0xE8;
    VDP_REG=0x81;


}
