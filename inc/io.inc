; vim: ft=asm_ca65
RAMBANKREG      = $bf00
ROMBANKREG      = $bf01

ACIA_DATA       = $bf10
ACIA_STATUS     = $bf11
ACIA_COMMAND    = $bf12
ACIA_CONTROL    = $bf13

VIA_PORTB       = $bf20
VIA_PORTA       = $bf21
VIA_DDRB        = $bf22
VIA_DDRA        = $bf23
VIA_T1CL        = $bf24
VIA_T1CH        = $bf25
VIA_T1LL        = $bf26
VIA_T1LH        = $bf27
VIA_T2CL        = $bf28
VIA_T2CH        = $bf29
VIA_SR		= $bf2a
VIA_ACR         = $bf2b
VIA_PCR         = $bf2c
VIA_IFR         = $bf2d
VIA_IER         = $bf2e

SD_SCK          = %00000001
SD_CS           = %00000010
SN_WE           = %00000100
SN_READY        = %00001000
ROMSW           = %01000000
SD_MOSI         = %10000000
LED_ON          = %00010000 ; ORA
LED_OFF         = %11101111 ; AND
BUTTON          = %00100000 ; MASK
ROM_SWITCH_ON   = %01000000 ; ORA
ROM_SWITCH_OFF  = %10111111 ; AND
