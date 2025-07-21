/* vim: set et ts=4 sw=4 */
#include <stdint.h>
#include "sfos.h"
#include "bios.h"
#include "vdp.h"
#include "chip8.h"

uint8_t i = 0;
Chip8 *chip;

void main(void) {

    vdp_reset();
    for (i=0;i<64;i++) {
        plot_xy(i,32,GREY);
    }

    chip = chip8_init();

    chip8_destroy(chip);
    sfos_s_warmboot();
}

