

PROJECT_NAME    =   CH32V003

PREFIX          =   riscv-none-embed-

CC              =   $(PREFIX)gcc
AS              =   $(PREFIX)gcc -x assembler-with-cpp
CP              =   $(PREFIX)objcopy
SZ              =   $(PREFIX)size
HEX             =   $(CP) -O ihex
BIN             =   $(CP) -O binary -S

PROJECT_DIR     =   .
BUILD_DIR       =   Build

C_INCLUDES      =   -IUser/Inc                                              \
                    -IDrivers/Core/Inc                                      \
                    -IDrivers/CH32V0xx_Driver/Inc

OPT             =   -Og

CFLAGS          =   -march=rv32ec                                           \
                    -mabi=ilp32e                                            \
                    -Wall                                                   \
                    -fdata-sections                                         \
                    -ffunction-sections                                     \
                    $(C_INCLUDES) $(OPT)

CFLAGS          +=  -MMD -MP -MF"$(@:%.o=%.d)"

LDSCRIPT        =   Linker/ch32v00x_flash.ld

LIBS            =   -lc -lm -lnosys 
LDFLAGS         =   -march=rv32ec                                           \
                    -mabi=ilp32e                                            \
                    -nostartfiles -Xlinker                                  \
                    --gc-sections                                           \
                    -Wl,-Map,$(BUILD_DIR)/$(PROJECT_NAME).map               \
                    --specs=nano.specs                                      \
                    -T$(LDSCRIPT) $(LIBS) $(OPT)

ASM_SOURCES     =   User/Src/startup_ch32v00x.S

C_SOURCES       =   Drivers/CH32V0xx_Driver/Src/ch32v00x_adc.c              \
                    Drivers/CH32V0xx_Driver/Src/ch32v00x_dbgmcu.c           \
                    Drivers/CH32V0xx_Driver/Src/ch32v00x_dma.c              \
                    Drivers/CH32V0xx_Driver/Src/ch32v00x_exti.c             \
                    Drivers/CH32V0xx_Driver/Src/ch32v00x_flash.c            \
                    Drivers/CH32V0xx_Driver/Src/ch32v00x_gpio.c             \
                    Drivers/CH32V0xx_Driver/Src/ch32v00x_i2c.c              \
                    Drivers/CH32V0xx_Driver/Src/ch32v00x_iwdg.c             \
                    Drivers/CH32V0xx_Driver/Src/ch32v00x_misc.c             \
                    Drivers/CH32V0xx_Driver/Src/ch32v00x_opa.c              \
                    Drivers/CH32V0xx_Driver/Src/ch32v00x_pwr.c              \
                    Drivers/CH32V0xx_Driver/Src/ch32v00x_rcc.c              \
                    Drivers/CH32V0xx_Driver/Src/ch32v00x_spi.c              \
                    Drivers/CH32V0xx_Driver/Src/ch32v00x_tim.c              \
                    Drivers/CH32V0xx_Driver/Src/ch32v00x_usart.c            \
                    Drivers/CH32V0xx_Driver/Src/ch32v00x_wwdg.c             \
                    User/Src/main.c                                         \
                    User/Src/system_ch32v00x.c

all: $(BUILD_DIR)/$(PROJECT_NAME).elf $(BUILD_DIR)/$(PROJECT_NAME).hex $(BUILD_DIR)/$(PROJECT_NAME).bin

OBJECTS         +=  $(addprefix $(BUILD_DIR)/,$(notdir $(C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))

OBJECTS         +=  $(addprefix $(BUILD_DIR)/,$(notdir $(ASM_SOURCES:.S=.o)))
vpath %.s $(sort $(dir $(ASM_SOURCES)))

$(BUILD_DIR):
	@mkdir $@

$(BUILD_DIR)/%.o: %.s Makefile | $(BUILD_DIR)
	@echo Compiling $<
	@$(AS) -c $(CFLAGS) $< -o $@

$(BUILD_DIR)/%.o: %.c Makefile | $(BUILD_DIR)
	@echo Compiling $<
	@$(CC) -c $(CFLAGS) -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.c=.lst)) $< -o $@

.PRECIOUS $(BUILD_DIR)/%.elf: $(OBJECTS) Makefile
	@echo Linking object...
	@$(CC) $(OBJECTS) $(LDFLAGS) -o $@
	@$(SZ) $@

$(BUILD_DIR)/%.hex: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	@$(HEX) $< $@
	
$(BUILD_DIR)/%.bin: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	@$(BIN) $< $@

rebuild:
	@make --no-print-directory clean
	@make --no-print-directory all

clean: $(BUILD_DIR)
	@rm -r $(BUILD_DIR)
	@echo Cleaned all file.

-include $(wildcard $(BUILD_DIR)/*.d)
