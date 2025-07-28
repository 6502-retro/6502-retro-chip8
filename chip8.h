#ifndef H_CHIP8_
#define H_CHIP8_
#include <stdbool.h>
#include <stdint.h>

typedef struct s_chip8 {
    bool is_running;
    uint8_t ram[4096];
    uint16_t stack[16];
}Chip8;

Chip8 *chip8_init();
void chip8_destroy(Chip8 *chip);
/* Plot to XY in frame buffer, XOR p with whatever was there before */
uint8_t chip8_plot_xy(Chip8 *chip, uint8_t x, uint8_t y, bool p);
uint8_t chip8_draw_sprite(Chip8 *chip, uint8_t x, uint8_t y, uint8_t n);

uint8_t chip8_test_key(uint8_t k);
uint8_t chip8_getkey();

void chip8_clear(Chip8 *chip);

void chip8_run(Chip8 *chip);
#endif
