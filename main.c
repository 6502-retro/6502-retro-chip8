/* vim: set et ts=4 sw=4: */
#include <6502.h>
#include <stdlib.h>
#include <stdint.h>
#include "bios.h"
#include "sfos.h"
#include "vdp.h"
#include "chip8.h"

Chip8 *chip;
uint16_t i = 0;
uint16_t j = 0;
uint8_t argc = 0;
char *argv[8] = {0};
char *cmd = (char*)0x301;
uint8_t *rom;
uint8_t current_drive;
uint8_t result;


void parse_args(char* cmd) {
    char *p2;
    p2 = strtok(cmd, " ");

    while (p2 && argc < 7) {
        argv[argc++] = p2;
        p2 = strtok('\0', " ");
    }
    argv[argc] = '\0';
}

void fatal(char * t) {
    sfos_c_printstr(t);
    sfos_d_getsetdrive(current_drive);
    prog_exit();
    sfos_s_warmboot();
}
void main(void) {

    SEI();
    vdp_reset();
    vdp_colorize(VDP_BLACK);
    CLI();

    vdp_flush(&FRAMEBUF);

    srand(1);
    chip = chip8_init();
    sfos_c_write('\n');

    rom = &chip->ram[0x200];    // rom will be our dma

    // read in the file
    current_drive = sfos_d_getsetdrive(0xFF);
    sfos_d_getsetdrive((&fcb2)->DRIVE);
    // open the file
    result = sfos_d_open(&fcb2);

    if (result) {
        fatal("Could not open file. Try: chip8 logo.ch8");
    }

    // Set the dma
    sfos_d_setdma((uint16_t*)rom);
    // Read the first block
    for (i=0; i < fcb2.SC; i++) {
        sfos_d_setdma((uint16_t*)rom);
        result = sfos_d_readseqblock(&fcb2);
        if (!result) break;
        rom += 512;
    }

    sfos_c_write('\n');
    sfos_c_printstr("Press ESC to quit\n");

    /*
    * Process chip8 loop
    */
    chip->is_running = true;
    chip8_run(chip);

    chip8_destroy(chip);
    sfos_d_getsetdrive(current_drive);
    prog_exit();
    bios_wboot();
}

