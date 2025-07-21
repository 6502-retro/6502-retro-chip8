#ifndef VDP_H_
#define VDP_H_
#include <stdint.h>

#define VDP_RAM         *(char*)0xBF30
#define VDP_REG         *(char*)0xBF31
#define PATTERN_TABLE   0x800

#define WHITE           15
#define GREY            14

extern void vdp_reset();
extern void __fastcall__ vdp_set_write_address(uint16_t);
extern void __fastcall__ vdp_set_read_address(uint16_t);
void plot_xy(uint8_t, uint8_t, uint8_t);
void set_read_addr(uint16_t);
void set_write_addr(uint16_t);
#endif
