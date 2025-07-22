/* vim: set et ts=4 sw=4: */
#include <stdint.h>
#include "sfos.h"
#include "bios.h"
#include "vdp.h"
#include "chip8.h"

Chip8 *chip;

void main(void) {

    uint8_t i = 0;
    vdp_reset();
    vdp_colorize(VDP_BLACK);

    for (i=0;i<64;++i) {
        vdp_plot_xy(i,32,VDP_GRAY);
    }

    vdp_wait();
    vdp_flush();

    chip = chip8_init();

    chip8_destroy(chip);
    sfos_s_warmboot();
}

