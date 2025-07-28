// vim: ts=4 sw=4 tw=80 cc=80:
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "sfos.h"
#include "vdp.h"
#include "chip8.h"

uint8_t hexfont[80] = {
	0xF0, 0x90, 0x90, 0x90, 0xF0,	// 0
	0x20, 0x60, 0x20, 0x20, 0x70,	// 1
	0xF0, 0x10, 0xF0, 0x80, 0xF0,	// 2
	0xF0, 0x10, 0xF0, 0x10, 0xF0,	// 3
	0x90, 0x90, 0xF0, 0x10, 0x10,	// 4
	0xF0, 0x80, 0xF0, 0x10, 0xF0,	// 5
	0xF0, 0x80, 0xF0, 0x90, 0xF0,	// 6
	0xF0, 0x10, 0x20, 0x40, 0x40,	// 7
	0xF0, 0x90, 0xF0, 0x90, 0xF0,	// 8
	0xF0, 0x90, 0xF0, 0x10, 0xF0,	// 9
	0xF0, 0x90, 0xF0, 0x90, 0x90,	// A
	0xE0, 0x90, 0xE0, 0x90, 0xE0,	// B
	0xF0, 0x80, 0x80, 0x80, 0xF0,	// C
	0xE0, 0x90, 0x90, 0x90, 0xE0,	// D
	0xF0, 0x80, 0xF0, 0x80, 0xF0,	// E
	0xF0, 0x80, 0xF0, 0x80, 0x80	// F
};

uint16_t tmp, I, pc = 0;
uint8_t sp,delay,sound = 0;
uint8_t V[16] = {0};

void invalid(char op, uint16_t instr) {
	printf("[0x%X] Invalid instruction: 0x%04X\n", op, instr);
	sfos_s_warmboot();
}
Chip8* chip8_init() {
	Chip8 *chip = (Chip8*)malloc(sizeof(struct s_chip8));
	if (chip == NULL)
		exit(-1);
	else {
		memset(chip, 0, sizeof(Chip8));
		memcpy(chip->ram, hexfont, 80);
		pc = 0x200;
		sp = 0;
		return chip;
	}
};

void chip8_destroy(Chip8 *chip) {
	free(chip);
}

uint8_t chip8_draw_sprite(Chip8 *chip, uint8_t x, uint8_t y, uint8_t n) {
	static uint8_t collision = 0;
	static uint8_t row = 0;
	static uint8_t p = 0;

	while (n > 0) {
		row = chip->ram[I + --n];
		for (p=8; p>0; p--) {
			if (row & 1)
				collision = vdp_plot_xy(x+p-1, y+n, VDP_WHITE);
			else
				collision = vdp_plot_xy(x+p-1, y+n, VDP_BLACK);
			row >>= 1;
		}
	}
	return collision;
}


bool chip8_test_key(uint8_t k) {
	uint8_t s = sfos_c_status();
	return s == k;
}

uint8_t chip8_get_key() {
	uint8_t k = 0;
	while (1) {
		k = sfos_c_status();
		if ( '0' <= k <= '9')
			return k-'0';
		if ( 'a' <= k <= 'f' )
			return k-'a';
		if ( 'A' <= k <= 'F' )
			return k-'A';
	}
}

void debug_regs() {
	uint8_t i=0;
	for (i=0; i<16; ++i) {
		printf("v[%02X] = 0x%02X ", i, V[i]);
	}
}
void chip8_run(Chip8 *chip) {
	bool drawflag = 0;

	while (chip->is_running) {
		// read two big endian bytes at PC
		uint16_t instr = chip->ram[pc] << 8 | chip->ram[pc + 1];
		// advance the program counter to the next instruction
		uint8_t op   = instr >> 12;           	// ?---
		uint16_t nnn = instr & 0x0FFF;		  	// -???
		uint8_t n    = instr & 0x000F;	      	// ---?
		uint8_t x    = (instr & 0x0F00) >> 8; 	// -?--
		uint8_t y    = (instr & 0x00F0) >> 4; 	// --?-
		uint8_t kk   = instr & 0x00FF;		  	// --??

#if 0
		printf("PC: 0x%03X, Inst: 0x%04X, op: 0x%1X, x: 0x%1X, y: 0x%01X, n: 0x%1X, kk: 0x%02X, nnn: 0x%03X\n", pc, instr, op, x, y, n, kk, nnn);
		printf("\t REGS: ");
		debug_regs();
		printf("\n");
		printf("\t i:0x%03X, delay:0x%02X, sound:0x%02X\n", I, delay, sound);
#endif

		pc += 2;
		if (pc >= 4096)
			pc = 512;		// Wrap around to start of program space


		switch (op) {
			case 0:
				if (nnn==0x0E0) {
					// clear screen
					vdp_clear_pattern_table(chip);
					break;
				} else if (nnn==0x0EE) {
					// Return from subroutine
					pc = chip->stack[sp];
					sp --;
					break;
				}
				else {
					// jump to subroutine
					pc = nnn;
					break;
				}
			case 1:
				// Jump
				pc = nnn;
				break;
			case 2:
				// call
				sp ++;
				chip->stack[sp] = pc;
				pc = nnn;
				break;
			case 3:
				// if Vx == byte then skip next instruction.
				if (x == kk)
					pc += 2;
				break;
			case 4:
				// if Vx != byte then skip next instruction.
				if (x != kk)
					pc += 2;
				break;
			case 5:
				{
					if (n==0) {
						if (x==y)
							pc += 2;
						break;
					} else {
						invalid(5, instr);
						break;
					}
				}
				break;
			case 6:
				// Vx = kk
				V[x] = kk;
				break;
			case 7:
				V[x] = V[x] + kk;
				break;

			case 8:
				switch(n) {
					case 0:
						// Vx = Vy
						V[x] = V[y];
						break;
					case 1:
						// Vx = Vx | Vy
						V[x] = V[x] | V[y];
						break;
					case 2:
						// Vx = Vx & Vy
						V[x] = V[x] & V[y];
						break;
					case 3:
						// Vx = Vx ^ Vy
						V[x] = V[x] ^ V[y];
						break;
					case 4:
						// Vx = Vx + Vy, VF = carry
						tmp = V[x] + V[y];
						if (tmp > 255)
							V[0xF] = 1;
						else
							V[0xF] = 0;
						V[x] = tmp & 0xFF;
						break;
					case 5:
						// Vx = Vx - Vy, VF = NOT borrow
						if (V[x] > V[y])
							V[0xF] = 1;
						else
							V[0xF] = 0;
						V[x] = V[x] - V[y];
						break;
					case 6:
						// Vx = Vx >> 1, VF has least significant bit
						if (V[x] & 1)
							V[0xF] = 1;
						else
							V[0xF] = 0;
						V[x] = V[x] >> 1;
						break;
					case 7:
						// Vx = Vy - Vx, VF = NOT borrow
						if (V[y] > V[x])
							V[0xF] = 1;
						else
							V[0xF] = 0;
						V[x] = V[y] - V[x];
						break;
					case 8:
						// Vx = Vx << 1, VF has most significant bit
						if (V[x] >> 7)
							V[0xF] = 1;
						else
							V[0xF] = 0;
						V[x] = V[x] << 1;
						break;
					default:
						invalid(8, instr);
						break;
				}
				break;
			case 9:
				// skip if vx != vy
				if (V[x] != V[y])
					pc += 2;
				break;
			case 0xA: //A
				// I = nnn
				I = nnn;
				break;
			case 0xB: // B
				// jmp to nnn + V0
				pc = (nnn + V[0]) & 0xFFF;
				break;
			case 0xC: // C
				// Vx == RAND() & kk;
				V[x] = (rand() & 0xFF) & kk;
				break;
			case 0xD: // D
				// DRW Vx, Vy, n - Display n byte sprite at vx,vy VF set on
				// collision.
				V[0xF] = chip8_draw_sprite(chip, V[x], V[y], n);
				drawflag = true;
				break;
			case 0xE: // E
				break;
			case 0xF: // F
				{
					switch (kk) {
						case 0x07:
							// Vx = delay
							V[x] = delay;
							break;
						case 0x0A:
							// wait for keypress
							V[x] = chip8_get_key();
							break;
						case 0x15:
							delay = V[x];
							break;
						case 0x18:
							sound = V[x];
							break;
						case 0x1E:
							I = (I + V[x]) & 0xFFF;
							break;
						case 0x29:
							I = V[x] * 5; // font is at 0x000
							break;
						case 0x33:
							chip->ram[I] = V[x] / 100;
							chip->ram[I+1] = (V[x] / 10) % 10;
							chip->ram[I+2] = V[x] % 10;
							break;
						case 0x55:
							memcpy(&chip->ram[I], V, x+1);
							break;
						case 0x65:
							// Read registers from ram into v0 to vx
							memcpy(V, &chip->ram[I], x+1);
							break;
						default:
							invalid(0xF, instr);
							break;
					}
					break;
				}
			default:
				invalid(-1, instr);
				break;
		}
		//vdp_wait();

		if (delay > 0) {
			delay -= 1;
		}

		if (sound > 0) {
			sound -= 1;
		}

		if (drawflag) {
			vdp_flush(&FRAMEBUF);
			drawflag = false;
		}
		if (sfos_c_status() == 0x1b)
			chip->is_running = false;

	}
}
