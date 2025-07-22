/* vim: set et ts=4 sw=4: */
#include <6502.h>
#include <string.h>
#include <stdint.h>
#include "sfos.h"
#include "bios.h"
#include "vdp.h"
#include "chip8.h"

Chip8 *chip;
uint16_t i = 0;
uint16_t j = 0;

void main(void) {

    SEI();
    vdp_reset();
    vdp_colorize(VDP_BLACK);

    for (j=0;j<8;++j) {
        for (i=0;i<64;++i)
            vdp_plot_xy(i, j, VDP_DARK_RED);
    }
    for (j=40;j<48;++j) {
        for (i=0;i<64;++i)
            vdp_plot_xy(i, j, VDP_DARK_RED);
    }
    CLI();

    vdp_wait();
    vdp_flush();

    chip = chip8_init();

    for (i=0;i<64;++i)
        chip8_plot_xy(chip, i, 16, 1);

    for (i=0;i<32;++i)
        chip8_plot_xy(chip, 32, i, 1);

    chip8_render(chip);
    vdp_wait();
    vdp_flush();

    for (i=0; i<60; ++i) {
        vdp_wait();
    }
    chip8_clear(chip);

    chip8_destroy(chip);
    sfos_s_warmboot();
}

