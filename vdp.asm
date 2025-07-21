; vim: set ft=asm_ca65 ts=4 sw=4 et:
.include "const.inc"
.include "app.inc"
.include "macro.inc"

.export _vdp_reset

.autoimport

.globalzp sp

.zeropage

v1: .word 0
v2: .word 0
v3: .word 0

.bss
; This is the framebuffer region in uninitialised BSS memory. It must be page
; aligned to support more efficient memory transfer into the VDP IO port.

.code

; reset vdp into MC mode
_vdp_reset:
    jsr vdp_clear_vram
    jsr vdp_init_mc
    ; init name table
    lda #<VDP_NAME
    ldx #>VDP_NAME
    jsr vdp_set_write_address

    lda #6
    sta v1+0
    lda #0
    sta v2+0
@lpsection:
    lda #4
    sta v1+1
@lpline:
;    lda #$0a
;    jsr bios_conout
;    lda #$0d
;    jsr bios_conout
    ldx #32
    lda v2+0
@lpcol:
;    pha
;    jsr bios_prbyte
;    lda #' '
;    jsr bios_conout
;    pla
    sta VDP_RAM
    inc
    dex
    bne @lpcol

    dec v1+1      ; lines
    bne @lpline
@lpsection_lp:
    lda #32
    clc
    adc v2+0
    sta v2+0

    dec v1+0
    bne @lpsection
    rts

; Zero out all 16KB of VRAM.
; INPUT: VOID
; OUTPUT: VOID
vdp_clear_vram:
    lda #0                  ; A is low byte of vram write address
    ldx #0                  ; X is high byte of vram write address
    jsr vdp_set_write_address ; set the starting address to zero.
    lda #0                  ; A has the value being written to VRAM
    ldy #0                  ; Y is the byte counter
    ldx #$3F                ; X is the page counter
:   sta VDP_RAM             ; save A into vram
    iny                     ; increment Y and loop until a whole page is written
    bne :-
    dex                     ; decement page counter and loop until all 0x3F
    bne :-                  ; pages are written.
    rts

; Sets the VDP internal VRAM pointer for writing.
; INPUT: A is the low byte of the VRAM address.
;        X is the high byte of the VRAM address.
; OUTPUT: VOID
vdp_set_write_address:
    sta VDP_REG             ; As per the TI Programmers Guide.
    txa
    ora #$40
    sta VDP_REG
    rts

; initialize vdp in Multicolour mode (64x48)
vdp_init_mc:
    lda #<mc_regs
    ldx #>mc_regs
    jmp _init_regs

; Copy a table of register values to the VDP.  The table is arranged in order
; from REG-0 to REG-7.  Each value is set to the register of its position in the
; table.
; INPUT: A is the low byte of register table address.
;        X is the high byte of register table address.
; OUTPUT: VOID
_init_regs:
    sta v3                ; set up a pointer to the register table.
    stx v3+1
    ldy #0                  ; Y is the offset in the register table.
:   lda (v3),y            ; load the first byte
    sta VDP_REG             ; save to VRAM
    tya                     ; use the pointer offset to set the VDP register
    ora #$80                ; As per the TI Programmers manual.
    sta VDP_REG
    iny
    cpy #8                  ; there are 8 registers altogether 0-7
    bne :-                  ; loop until complete.
    rts

.rodata
; These are the registers for the multicolour mode
mc_regs:
    .byte $00               ; M3 = 0
    .byte (VDP_16K|VDP_BLANK_SCREEN|VDP_INTERRUPT_ENABLE|VDP_M2)   ; M1=0, M2=1
    .byte $05               ; Address table = 0x1400
    .byte $00               ; colour table not used
    .byte $01               ; Pattern table at 0x800
    .byte $20               ; Sprite attribute table at 0x1000
    .byte $00               ; Sprite pattern table at 0x0000
    .byte VDP_DARK_YELLOW   ; backdrop colour
