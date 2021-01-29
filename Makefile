PROJECT = ${BINARY_FILE_NAME}
# SRCS: all source files from src directory
SRCS = $(wildcard src/*.c) \
		$(wildcard libs/*.c) 

PERIPH_DRIVER_PATH = BlueNRG1_Periph_Driver/src
HAL_SOURCE_PATH = hal/src
SDK_EVAL_PATH = SDK_Eval_BlueNRG1/src

SRCS_TO_COPY = \
	${BLUENRG_DK_LIB_PATH}/${PERIPH_DRIVER_PATH}/BlueNRG1_adc.c \
	${BLUENRG_DK_LIB_PATH}/${PERIPH_DRIVER_PATH}/BlueNRG1_dma.c \
	${BLUENRG_DK_LIB_PATH}/${PERIPH_DRIVER_PATH}/BlueNRG1_flash.c \
	${BLUENRG_DK_LIB_PATH}/${PERIPH_DRIVER_PATH}/BlueNRG1_gpio.c \
	${BLUENRG_DK_LIB_PATH}/${PERIPH_DRIVER_PATH}/BlueNRG1_mft.c \
	${BLUENRG_DK_LIB_PATH}/${PERIPH_DRIVER_PATH}/BlueNRG1_pka.c \
	${BLUENRG_DK_LIB_PATH}/${PERIPH_DRIVER_PATH}/BlueNRG1_radio.c \
	${BLUENRG_DK_LIB_PATH}/${PERIPH_DRIVER_PATH}/BlueNRG1_rng.c \
	${BLUENRG_DK_LIB_PATH}/${PERIPH_DRIVER_PATH}/BlueNRG1_rtc.c \
	${BLUENRG_DK_LIB_PATH}/${PERIPH_DRIVER_PATH}/BlueNRG1_spi.c \
	${BLUENRG_DK_LIB_PATH}/${PERIPH_DRIVER_PATH}/BlueNRG1_sysCtrl.c \
	${BLUENRG_DK_LIB_PATH}/${PERIPH_DRIVER_PATH}/BlueNRG1_timer.c \
	${BLUENRG_DK_LIB_PATH}/${PERIPH_DRIVER_PATH}/BlueNRG1_uart.c \
	${BLUENRG_DK_LIB_PATH}/${PERIPH_DRIVER_PATH}/BlueNRG1_wdg.c \
	${BLUENRG_DK_LIB_PATH}/${PERIPH_DRIVER_PATH}/misc.c \
	${BLUENRG_DK_LIB_PATH}/${HAL_SOURCE_PATH}/clock.c \
	${BLUENRG_DK_LIB_PATH}/${HAL_SOURCE_PATH}/osal.c \
	${BLUENRG_DK_LIB_PATH}/${HAL_SOURCE_PATH}/sleep.c \
	${BLUENRG_DK_LIB_PATH}/${HAL_SOURCE_PATH}/miscutil.c \
	${BLUENRG_DK_LIB_PATH}/${SDK_EVAL_PATH}/SDK_EVAL_Button.c \
	${BLUENRG_DK_LIB_PATH}/${SDK_EVAL_PATH}/SDK_EVAL_Com.c \
	${BLUENRG_DK_LIB_PATH}/${SDK_EVAL_PATH}/SDK_EVAL_Config.c \
	${BLUENRG_DK_LIB_PATH}/${SDK_EVAL_PATH}/SDK_EVAL_Led.c \
	${BLUENRG_DK_LIB_PATH}/${SDK_EVAL_PATH}/SDK_EVAL_SPI.c \
	${BLUENRG_DK_LIB_PATH}/${SDK_EVAL_PATH}/SDK_EVAL_Config.c \
	${BLUENRG_DK_LIB_PATH}/CMSIS/Device/ST/BlueNRG1/Source/system_bluenrg1.c \
	${BLUENRG_DK_LIB_PATH}/Bluetooth_LE/src/stack_user_cfg.c


PRE_OBJS = $(wildcard precompiled/*.o)

LIB_ASSM = $(wildcard assembly/*.a)

S_ASSM = $(addprefix -l,$(LIB_ASSM)))

OBJ = obj/

# OBJS: list of object files
OBJS = $(addprefix $(OBJ),$(notdir $(SRCS:.c=.o)))

S_OBJS = $(addprefix $(assembly/made/),$(notdir $(S_SRCS:.S=.o)))

C_SWITCH_OBJS = $(addprefix $(OBJ),$(notdir $(CONTEXT_SWITCH:.s=.o)))


# ! NOTE ARM_TOOLCHAIN_VERSION, ARM_TOOLCHAIN_PATH, and BLUENRG_DK_LIB_PATH are set in .vscode/tasks.json envs variable
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
# COPY	= copy "libs\\misc.c" "./libs/misc2.c"

DEFINES = -DBLUENRG1_DEVICE -DDEBUG -DHS_SPEED_XTAL=HS_SPEED_XTAL_16MHZ -DLS_SOURCE=LS_SOURCE_INTERNAL_RO -DSMPS_INDUCTOR=SMPS_INDUCTOR_4_7uH -Dmcpu=cortexm0

#GCC FLAGS
CFLAGS = -mthumb -mcpu=cortex-m0 $(DEFINES) -specs=nano.specs -mfloat-abi=soft#-specs=nano.specs 
CFLAGS +=  -MD -std=c99 -c -fdata-sections -ffunction-sections  -Og -fdata-sections -g -fstack-usage -Wall

ASFLAGS = -Wall -ggdb -mthumb

SFLAGS =  -mthumb -mcpu=cortex-m0 -g -Wa,--no-warn -x assembler-with-cpp # -specs=nano.specs

LDFLAGS = -T$(LD_SCRIPT) -mthumb -mfloat-abi=soft -specs=nano.specs -nostartfiles -mcpu=cortex-m0 -Wl,--gc-sections -Wl,--defsym=malloc_getpagesize_P=0x80 -nodefaultlibs "-Wl,-Map=BLE_Beacon.map" -static -Wl,--cref  -static -L./assembly  -Wl,--start-group -lc -lm -Wl,--end-group -lbluenrg1_stack -lcrypto

# Potentially these might work better if you are getting errors about _exit and stuff
# LDFLAGS = -T$(LD_SCRIPT) --specs=nosys.specs -mthumb -mfloat-abi=softfp -mcpu=cortex-m0 -Wl,--gc-sections -Wl,--defsym=malloc_getpagesize_P=0x80 -nodefaultlibs "-Wl,-Map=BLE_Beacon.map" -static -Wl,--cref  -static -L./assembly  -Wl,--start-group -lc -lc -lnosys -lm -Wl,--end-group -lbluenrg1_stack -lcrypto
define \n


endef

# Rules to build bin
# all: bin/$(PROJECT).bin
all: post-build

# Copies necessary files from source dirs 
pre-build:
	@echo PRE
	if not exist "libs" mkdir "libs"
	$(foreach file,$(subst /,\,$(SRCS_TO_COPY)),@if not exist "libs/$(notdir $(file))" @copy "$(file)" "libs/$(notdir $(file))"${\n})
	$(info $$SRCS is [${SRCS}])

post-build: main-build
	@echo POST

main-build: pre-build
	$(MAKE) --no-print-directory bin/$(PROJECT).bin

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