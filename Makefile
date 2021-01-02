PROJECT = blink
# SRCS: all source files from src directory
SRCS = $(wildcard src/*.c) \
		$(wildcard libs/*.c)

$(info $$SRCS is [${SRCS}])

OBJ = obj/
# OBJS: list of object files
OBJS = $(addprefix $(OBJ),$(notdir $(SRCS:.c=.o)))

#Flag points to the INC folder containing header files
INC =-I./inc -IC:/Users/cmart/ST/BlueNRG-1DKLib/Library/hal/inc -IC:/Users/cmart/ST/BlueNRG-1DKLib/Library/BlueNRG1_Periph_Driver/inc -IC:/Users/cmart/ST/BlueNRG-1DKLib/Library/Bluetooth_LE/inc -IC:/Users/cmart/ST/BlueNRG-1DKLib/Library/CMSIS/Include -IC:/Users/cmart/ST/BlueNRG-1DKLib/Library/CMSIS/Device/ST/BlueNRG1/Include -IC:/Users/cmart/ST/BlueNRG-1DKLib/Library/SDK_Eval_BlueNRG1/inc -IC:/Users/cmart/ST/BlueNRG-1DKLib/Library/BLE_Application/OTA/inc -IC:/Users/cmart/ST/BlueNRG-1DKLib/Library/BLE_Application/Utils/inc -IC:/Users/cmart/ST/BlueNRG-1DKLib/Library/BLE_Application/layers_inc -IC:/armtoolslib/lib/gcc/arm-none-eabi/8.2.1/include -IC:/armtoolslib/lib/gcc/arm-none-eabi/8.2.1/include-fixed -IC:/armtoolslib/arm-none-eabi/include -IC:/armtoolslib/arm-none-eabi/include/machine -IC:/armtoolslib/arm-none-eabi/include/newlib-nano -IC:/armtoolslib/arm-none-eabi/include/sys

# LD_SCRIPT: linker script
LD_SCRIPT=./BlueNRG1.ld


#UTILITY VARIABLES
CC = arm-none-eabi-gcc #compiler
LD = arm-none-eabi-ld #linker
OBJCOPY = arm-none-eabi-objcopy #final executable builder
# FLASHER = lm4flash #flashing utility
RM      = rm -rf
# MKDIR   = @mkdir -p $(@D) #creates folders if not present

#GCC FLAGS
CFLAGS = -ggdb -mthumb -mcpu=cortex-m0 -specs=nano.specs
CFLAGS += -Os -MD -std=c99 -c 

#LINKER FLAGS
LDFLAGS = -T$(LD_SCRIPT) -e Reset_Handler

# Rules to build bin
all: bin/$(PROJECT).bin

$(OBJ)%.o: src/%.c
	$(CC) -o $@ $^ $(INC) $(CFLAGS)

$(OBJ)%.o: libs/%.c
	$(CC) -o $@ $^ $(INC) $(CFLAGS)

bin/$(PROJECT).elf: $(OBJS)
	$(LD) -o $@ $^ $(LDFLAGS)

bin/$(PROJECT).bin: bin/$(PROJECT).elf
	$(OBJCOPY) -O binary $< $@

# #Flashes bin to TM4C
# flash:
# 	$(FLASHER) -S $(DEV) bin/$(PROJECT).bin

#remove object and bin files
clean:
	-$(RM) obj
	-$(RM) bin

.PHONY: all clean