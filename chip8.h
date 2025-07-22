#ifndef H_CHIP8_
#define H_CHIP8_
#include <stdbool.h>
#include <stdint.h>

typedef struct s_chip8 {
    bool is_running;
    uint8_t ram[4096];
    uint8_t fb[2048];
    uint8_t pc;
    uint8_t v[16];
    uint16_t i;
    uint16_t stack[48];
    uint8_t delay;
    uint8_t sound;
}Chip8;


Chip8 *chip8_init();
void chip8_destroy(Chip8 *chip);
void chip8_render(Chip8 *chip);
/* Plot to XY in frame buffer, XOR p with whatever was there before */
void chip8_plot_xy(Chip8 *chip, uint8_t x, uint8_t y, bool p);

void chip8_clear(Chip8 *chip);
#endif
