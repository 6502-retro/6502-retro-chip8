// vim: ts=4 sw=4 tw=80 cc=80:
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "bios.h"
#include "c8_lib.h"
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
uint8_t delay,sp = 0;
uint8_t V[16] = {0};
uint8_t key = 0;
bool drawflag = 0;

#define sound *(uint8_t*)0x65F


void invalid(char op, uint16_t instr) {
	printf("[0x%X] Invalid instruction: 0x%04X\n", op, instr);
	prog_exit();
	bios_sn_stop();
	bios_wboot();
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
		I = 0;
		bios_sn_start();
		return chip;
	}
};

void chip8_destroy(Chip8 *chip) {
	bios_sn_stop();
	free(chip);
}

uint8_t chip8_draw_sprite(Chip8 *chip, uint8_t x, uint8_t y, uint8_t n) {
	static uint8_t collision = 0;
	static uint8_t row = 0;
	static uint8_t p = 0;
	uint8_t i = 0;
	while (n > 0) {
		row = chip->ram[I + --n];
		p = 7;
		for (i=0; i<8; ++i) {
			if (row & 1)
				collision = vdp_plot_xy(x+p, y+n, VDP_WHITE);
			else
				collision = vdp_plot_xy(x+p, y+n, VDP_BLACK);
			row >>= 1;
			p--;
		}
	}
	return collision;
}


void debug_regs() {
	uint8_t i=0;
	for (i=0; i<16; ++i) {
		printf("\t\tv[%02X] = 0x%02X\n", i, V[i]);
	}
}
void debug_stack(Chip8 *chip, uint8_t sp) {
	uint8_t i=0;
	for (i=0; i<16; ++i) {
		if (i==sp)
			printf("\t\t*[%02X] = 0x%04X\n", i, chip->stack[i]);
		else
			printf("\t\t[%02X] = 0x%04X\n", i, chip->stack[i]);
	}
}
static bool keys[16] = {0};
static uint8_t k = 0;

uint8_t wait_for_key() {

	do {
		k = bios_const();
		if (k != 0) {
			switch (k) {
				case '1':  return 0x1;
				case '2':  return 0x2;
				case '3':  return 0x3;
				case '4':  return 0xc;
				case 'q':  return 0x4;
				case 'w':  return 0x5;
				case 'e':  return 0x6;
				case 'r':  return 0xd;
				case 'a':  return 0x7;
				case 's':  return 0x8;
				case 'd':  return 0x9;
				case 'f':  return 0xe;
				case 'z':  return 0xa;
				case 'x':  return 0x0;
				case 'c':  return 0xb;
				case 'v':  return 0xf;
				case 0x1b: {
					prog_exit();
					bios_sn_stop();
					bios_wboot();
				}
			}
			bios_conout(k);
		}
	} while (true);
}
void update_keys() {
	k = bios_const();
	switch(k) {
		case '1': keys[0x01] = (keys[0x01]) ? false : true; break;
		case '2': keys[0x02] = (keys[0x02]) ? false : true; break;
		case '3': keys[0x03] = (keys[0x03]) ? false : true; break;
		case '4': keys[0x0c] = (keys[0x0c]) ? false : true; break;
		case 'q': keys[0x04] = (keys[0x04]) ? false : true; break;
		case 'w': keys[0x05] = (keys[0x05]) ? false : true; break;
		case 'e': keys[0x06] = (keys[0x06]) ? false : true; break;
		case 'r': keys[0x0d] = (keys[0x0d]) ? false : true; break;
		case 'a': keys[0x07] = (keys[0x07]) ? false : true; break;
		case 's': keys[0x08] = (keys[0x08]) ? false : true; break;
		case 'd': keys[0x09] = (keys[0x09]) ? false : true; break;
		case 'f': keys[0x0e] = (keys[0x0e]) ? false : true; break;
		case 'z': keys[0x0a] = (keys[0x0a]) ? false : true; break;
		case 'x': keys[0x00] = (keys[0x00]) ? false : true; break;
		case 'c': keys[0x0b] = (keys[0x0b]) ? false : true; break;
		case 'v': keys[0x0f] = (keys[0x0f]) ? false : true; break;
	}
}

void chip8_run(Chip8 *chip) {

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
		printf("PC: 0x%03X, Inst: 0x%04X, op: 0x%1X, x: 0x%1X, y: 0x%01X, n: 0x%1X, kk: 0x%02X, nnn: 0x%03X", pc, instr, op, x, y, n, kk, nnn);
		printf("\n\tREGS:\n");
		debug_regs();
		printf("\n\t i:0x%03X, delay:0x%02X, sound:0x%02X\n", I, delay, sound);
		printf("\n\tSTACK: \n");
		debug_stack(chip, sp);
		printf("\n");
#endif

		pc += 2;
		if (pc >= 4096)
			pc = 512;		// Wrap around to start of program space


		switch (op) {
			case 0:
				switch (kk) {
					case 0xE0:
						memset(&FRAMEBUF, 0x11, 0x400);
						vdp_flush(&FRAMEBUF);
						break;
					case 0xEE:
						--sp;
						pc = chip->stack[sp];
						break;
					default:
						invalid(0, instr);
				}
				break;
			case 1:
				// Jump
				pc = nnn;
				break;
			case 2:
				// call
				chip->stack[sp] = pc;
				++sp;
				pc = nnn;
				break;
			case 3:
				// if Vx != byte then skip next instruction.
				if (V[x] == kk)
					pc += 2;
				break;
			case 4:
				// if Vx == byte then skip next instruction.
				if (V[x] != kk)
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
					case 14:
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
				{
					switch(kk) {
						case 0x9E:
							if (keys[V[x]])
								pc += 2;
							break;
						case 0xA1:
							if (!keys[V[x]])
								pc += 2;
							break;
						default:
							invalid(9, instr);
					}
					break;
				}
			case 0xF: // F
				{
					switch (kk) {
						case 0x07:
							// Vx = delay
							V[x] = delay;
							break;
						case 0x0A:
							// wait for keypress
							V[x] = wait_for_key();
							break;
						case 0x15:
							delay = V[x];
							break;
						case 0x18:
							sound = V[x];
							if (V[x] > 0) {
								sn_play_note();
							} else {
								bios_sn_silence();
							}

							break;
						case 0x1E:
							I = (I + V[x]) & 0xFFF;
							break;
						case 0x29:
							I = V[x] * 5; // font is at 0x000
							break;
						case 0x33:
							tmp = V[x];
							chip->ram[I+0] = (tmp - (tmp % 100))/100;
							tmp -= chip->ram[I+0] * 100;
							chip->ram[I+1] = (tmp - (tmp % 10))/10;
							tmp -= chip->ram[I+1] * 10;
							chip->ram[I+2] = tmp;
							//printf("%d%d%d\n",chip->ram[I+0],chip->ram[I+1],chip->ram[I+2]);
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
		update_keys();
		if (k == 0x1b) chip->is_running = false;
	}
}
