// vim: set et ts=4 sw=4
#ifndef BIOS_H
#define BIOS_H

extern void __fastcall__ bios_conout(char c);
extern uint8_t bios_conin();
extern uint8_t bios_const();
extern void bios_wboot();
extern void __fastcall__ bios_puts(char* s);

extern void bios_sn_start();
extern void bios_sn_silence();
extern void bios_sn_stop();
extern void __fastcall__ bios_sn_send(uint8_t b);

extern void bios_led_on();
extern void bios_led_off();
extern uint8_t bios_get_button();

extern void bios_sn_start();
extern void bios_sn_stop();
extern void bios_sn_send();

#endif
