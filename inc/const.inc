; vim: set ft=asm_ca65 ts=4 sw=4 et:
; IO Ports
VDP_RAM                 = $BF30
VDP_REG                 = $BF31

; colours
VDP_TRANSPARENT         = 0
VDP_BLACK               = 1
VDP_MEDIUM_GREEN        = 2
VDP_LIGHT_GREEN         = 3
VDP_DARK_BLUE           = 4
VDP_LIGHT_BLUE          = 5
VDP_DARK_RED            = 6
VDP_CYAN                = 7
VDP_MEDIUM_RED          = 8
VDP_LIGHT_RED           = 9
VDP_DARK_YELLOW         = 10
VDP_LIGHT_YELLOW        = 11
VDP_DARK_GREEN          = 12
VDP_MAGENTA             = 13
VDP_GRAY                = 14
VDP_WHITE               = 15

; REG 0
VDP_EXT_VID             = %00000001
VDP_M3                  = %00000010

; REG 1
VDP_16K                 = %10000000
VDP_BLANK_SCREEN        = %01000000
VDP_INTERRUPT_ENABLE    = %00100000
VDP_M1                  = %00010000
VDP_M2                  = %00001000
VDP_16_SPRITE           = %00000010
VDP_SPRITE_MAG          = %00000001

VDP_NAME                = $1400
VDP_PATTERN             = $0800
VDP_SPRITE_ATTRIB       = $1000
VDP_SPRITE_PATTERN      = $0000
