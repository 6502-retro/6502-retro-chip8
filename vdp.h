#ifndef VDP_H_
#define VDP_H_
#include <stdint.h>

#define FRAMEBUF 	*(char*)0xA000
#define VDP_RAM         *(char*)0xBF30
#define VDP_REG         *(char*)0xBF31
#define PATTERN_TABLE   0x800

#define VDP_TRANSPARENT         0
#define VDP_BLACK               1
#define VDP_MEDIUM_GREEN        2
#define VDP_LIGHT_GREEN         3
#define VDP_DARK_BLUE           4
#define VDP_LIGHT_BLUE          5
#define VDP_DARK_RED            6
#define VDP_CYAN                7
#define VDP_MEDIUM_RED          8
#define VDP_LIGHT_RED           9
#define VDP_DARK_YELLOW         10
#define VDP_LIGHT_YELLOW        11
#define VDP_DARK_GREEN          12
#define VDP_MAGENTA             13
#define VDP_GRAY                14
#define VDP_WHITE               15

extern void vdp_reset();
extern void __fastcall__ vdp_set_read_addr(uint16_t);
extern void __fastcall__ vdp_set_write_addr(uint16_t);
extern void vdp_wait();

void vdp_plot_xy(uint8_t, uint8_t, uint8_t);
void vdp_colorize(uint8_t);
void vdp_off();
void vdp_on();
void vdp_flush();
#endif
