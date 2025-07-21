#ifndef VDP_H_
#define VDP_H_
#include <stdint.h>

#define VDP_RAM         *(char*)0xBF30
#define VDP_REG         *(char*)0xBF31
#define PATTERN_TABLE   0x800

#define BLACK           1
#define WHITE           15
#define GREY            14

extern void vdp_reset();
extern void vdp_put(uint8_t);
extern uint8_t vdp_get();
void vdp_plot_xy(uint8_t, uint8_t, uint8_t);
void vdp_set_read_addr(uint16_t);
void vdp_set_write_addr(uint16_t);
void vdp_colorize(uint8_t);
#endif
