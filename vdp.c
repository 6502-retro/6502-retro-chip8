#include <6502.h>
#include <stdint.h>
#include <string.h>
#include "vdp.h"
#include "c8_lib.h"

extern uint16_t i;
char *framebuf = &FRAMEBUF;



uint8_t vdp_plot_xy(uint8_t x, uint8_t y, uint8_t c) {
    static uint8_t dot = 0;
    static uint8_t old = 0;
    static char pix = 0;
    static uint16_t addr = 0;
    char collide = 0;

    //addr = (8 * (x / 2)) + (y % 8) + (256 * (y / 8));
    x = x & 0x3F;
    y = y & 0x1F;

    addr = vdp_xy_to_offset(x<<8|y);
    dot = framebuf[addr];

    if (x & 1) // Odd columns
    {
        // -X
        if ( (c == VDP_WHITE) && ( (dot & 0x0F) == VDP_WHITE) )
        {
            collide = 1;
            pix = (dot & 0xF0) | VDP_BLACK;
        }
        else
        {
            pix = (dot & 0xF0) | c;
        }
    }
    else
    {
        // X-
        if ( (c == VDP_WHITE) && ( (dot>>4) == VDP_WHITE) )
        {
            collide = 1;
            pix = (VDP_BLACK<<4) | (dot & 0x0F);
        }
        else {
            pix = (c << 4) | (dot & 0x0F);
        }
    }
    framebuf[addr] = pix;
    return collide;
}

// Set all pixels to black
void vdp_colorize(uint8_t c) {
    uint8_t b = c<<4 | c;
    memset(framebuf, b, 0x600);
}

// vim: ts=4 sw=4 et:
