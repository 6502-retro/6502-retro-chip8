#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "chip8.h"

Chip8 *chip8_init() {
    Chip8 *chip = (Chip8*)malloc(sizeof(struct s_chip8));
    if (chip == NULL)
        exit(-1);
    else
        return chip;
};

void chip8_destroy(Chip8 *chip) {
    free(chip);
}
