PROJECT = blink
# SRCS: all source files from src directory
SRCS = $(wildcard src/*.c) \
		$(wildcard libs/*.c) 
		

PRE_OBJS = $(wildcard precompiled/*.o)

LIB_ASSM = $(wildcard assembly/*.a)

S_ASSM = $(addprefix -l,$(LIB_ASSM)))

OBJ = obj/

# OBJS: list of object files
OBJS = $(addprefix $(OBJ),$(notdir $(SRCS:.c=.o)))

S_OBJS = $(addprefix $(assembly/made/),$(notdir $(S_SRCS:.S=.o)))

C_SWITCH_OBJS = $(addprefix $(OBJ),$(notdir $(CONTEXT_SWITCH:.s=.o)))


# ! NOTE ARM_TOOLCHAIN_VERSION and ARM_TOOLCHAIN_PATH are set in .vscode/tasks.json envs variable
#Flag points to the INC folder containing header files
INC = -I${BLUENRG_DK_LIB_PATH}/Bluetooth_LE/library/static_stack \
	-I${BLUENRG_DK_LIB_PATH}/BLE_Application/Profile_Central/includes \
	-I${BLUENRG_DK_LIB_PATH}/Bluetooth_LE/library/static_stack \
	-I./inc \
	-I${BLUENRG_DK_LIB_PATH}/hal/inc \
	-I${BLUENRG_DK_LIB_PATH}/BlueNRG1_Periph_Driver/inc \
	-I${BLUENRG_DK_LIB_PATH}/Bluetooth_LE/inc \
	-I${BLUENRG_DK_LIB_PATH}/CMSIS/Include \
	-I${BLUENRG_DK_LIB_PATH}/CMSIS/Device/ST/BlueNRG1/Include \
	-I${BLUENRG_DK_LIB_PATH}/SDK_Eval_BlueNRG1/inc \
	-I${BLUENRG_DK_LIB_PATH}/BLE_Application/OTA/inc \
	-I${BLUENRG_DK_LIB_PATH}/BLE_Application/Utils/inc \
	-I${BLUENRG_DK_LIB_PATH}/BLE_Application/layers_inc \
	-I${ARM_TOOLCHAIN_PATH}/../lib/gcc/arm-none-eabi/${ARM_TOOLCHAIN_VERSION}/include \
	-I${ARM_TOOLCHAIN_PATH}/../lib/gcc/arm-none-eabi/${ARM_TOOLCHAIN_VERSION}/include-fixed \
	-I${ARM_TOOLCHAIN_PATH}/../arm-none-eabi/include \
	-I${ARM_TOOLCHAIN_PATH}/../arm-none-eabi/include/machine \
	-I${ARM_TOOLCHAIN_PATH}/../arm-none-eabi/include/newlib-nano \
	-I${ARM_TOOLCHAIN_PATH}/../arm-none-eabi/include/sys

# LD_SCRIPT: linker script
LD_SCRIPT=./BlueNRG1.ld

#UTILITY VARIABLES
CC = arm-none-eabi-gcc #compiler
LD = arm-none-eabi-gcc#arm-none-eabi-ld #linker
AS = arm-none-eabi-as
OBJCOPY = arm-none-eabi-objcopy #final executable builder
# FLASHER = lm4flash #flashing utility
RM      = rmdir /s
MKDIR   = if not exist $(@D) mkdir $(@D)#creates folders if not present

DEFINES = -DBLUENRG1_DEVICE -DDEBUG -DHS_SPEED_XTAL=HS_SPEED_XTAL_16MHZ -DLS_SOURCE=LS_SOURCE_INTERNAL_RO -DSMPS_INDUCTOR=SMPS_INDUCTOR_4_7uH -Dmcpu=cortexm0

#GCC FLAGS
CFLAGS = -mthumb -mcpu=cortex-m0 $(DEFINES) -specs=nano.specs -mfloat-abi=soft#-specs=nano.specs 
CFLAGS +=  -MD -std=c99 -c -fdata-sections -ffunction-sections  -Og -fdata-sections -g -fstack-usage -Wall

ASFLAGS = -Wall -ggdb -mthumb

SFLAGS =  -mthumb -mcpu=cortex-m0 -g -Wa,--no-warn -x assembler-with-cpp # -specs=nano.specs


LDFLAGS = -T$(LD_SCRIPT) -mthumb -mfloat-abi=soft -specs=nano.specs -nostartfiles -mcpu=cortex-m0 -Wl,--gc-sections -Wl,--defsym=malloc_getpagesize_P=0x80 -nodefaultlibs "-Wl,-Map=BLE_Beacon.map" -static -Wl,--cref  -static -L./assembly  -Wl,--start-group -lc -lm -Wl,--end-group -lbluenrg1_stack -lcrypto

# Potentially these might work better if you are getting errors about _exit and stuff
# LDFLAGS = -T$(LD_SCRIPT) --specs=nosys.specs -mthumb -mfloat-abi=softfp -mcpu=cortex-m0 -Wl,--gc-sections -Wl,--defsym=malloc_getpagesize_P=0x80 -nodefaultlibs "-Wl,-Map=BLE_Beacon.map" -static -Wl,--cref  -static -L./assembly  -Wl,--start-group -lc -lc -lnosys -lm -Wl,--end-group -lbluenrg1_stack -lcrypto



# Rules to build bin
# all: bin/$(PROJECT).bin
all: bin/$(PROJECT).bin

$(OBJ)%.o: libs/%.s
	$(MKDIR)
	$(CC) $(SFLAGS) -o $@ $^ 
# assembly
$(assembly/made/)%.o: aeabi-cortexm0/%.S
	$(MKDIR)
	$(AS) $(ASFLAGS) -c $< -o $@

$(OBJ)%.o: src/%.c
	$(MKDIR)
	$(CC) -o $@ $^ $(INC) $(CFLAGS)

$(OBJ)%.o: libs/%.c
	$(MKDIR)
	$(CC) -o $@ $^ $(INC) $(CFLAGS)

bin/$(PROJECT).elf: $(OBJS) $(S_OBJS) $(PRE_OBJS) $(C_SWITCH_OBJS)
	$(MKDIR)
	$(LD) -o $@ $^ $(LDFLAGS)
	
bin/$(PROJECT).bin: bin/$(PROJECT).elf
	$(MKDIR)
	$(OBJCOPY) -O binary $< $@

#remove object and bin files
clean:
	-$(RM) obj
	-$(RM) bin

.PHONY: all clean