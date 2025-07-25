# Sources and objects
C_SOURCES = \
	    vdp.c \
	    chip8.c \
	    main.c
AS_SOURCES = \
	     vdp.asm
APPNAME = chip8

# DO NOT EDIT THIS
include Make.rules# Sources and objects

$(BUILD_DIR)/$(APPNAME).raw: $(AS_SOURCES) $(C_SOURCES)
	mkdir -pv $(BUILD_DIR)
	$(AS) $(ASFLAGS) -l $(BUILD_DIR)/vdp.lst -o $(BUILD_DIR)/vdp.o vdp.asm
	$(CC) $(CCFLAGS) -O -o $(BUILD_DIR)/$(APPNAME).raw $(C_SOURCES) $(BUILD_DIR)/vdp.o lib/sfoslib.lib 

$(BUILD_DIR)/$(APPNAME).bin: $(BUILD_DIR)/$(APPNAME).raw
	$(LOADTRIM) $^ $@ $(LOAD_ADDR)

copy: build/chip8.com
	../6502-retro-os/py_sfs_v2/cli.py rm -i ../6502-retro-os/py_sfs_v2/6502-retro-sdcard.img -d g://chip8.com
	../6502-retro-os/py_sfs_v2/cli.py cp -i ../6502-retro-os/py_sfs_v2/6502-retro-sdcard.img -s build/chip8.com -d g://chip8.com
