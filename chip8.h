#ifndef H_CHIP8_
#define H_CHIP8_
#include <stdbool.h>
#include <stdint.h>

typedef struct s_chip8 {
    bool is_running;
    char ram[2048];
    char fb[2048];
} Chip8;


Chip8 *chip8_init();
void chip8_destroy(Chip8 *);

#endif
