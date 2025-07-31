#ifndef C8_LIB_H_
#define C8_LIB_H_

extern void vdp_reset();
extern void __fastcall__ vdp_set_read_addr(uint16_t);
extern void __fastcall__ vdp_set_write_addr(uint16_t);
extern uint16_t __fastcall__ vdp_xy_to_offset(uint16_t);
extern void vdp_clear_pattern_table();
extern void vdp_wait();
extern void prog_exit();
extern void sn_play_note();

#endif //C8_LIB_H_
