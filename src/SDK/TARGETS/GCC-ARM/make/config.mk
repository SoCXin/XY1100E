####################################################################################################
# Compiler & Linker Tools, Do not edit!!!
####################################################################################################
# compiler
AS  :=arm-none-eabi-as
CC  :=arm-none-eabi-gcc
CXX :=arm-none-eabi-g++
CPP :=$(CC) -E

OBJCOPY :=arm-none-eabi-objcopy
OBJDUMP :=arm-none-eabi-objdump
READELF :=arm-none-eabi-readelf
OBJSIZE :=arm-none-eabi-size
AR      :=arm-none-eabi-ar
RANLIB  :=arm-none-eabi-ranlib


####################################################################################################
# Chip kernel correlation, Do not edit!!!
####################################################################################################
CORE = cortex-m3
HARDWARE_VER = xinyi_1100


####################################################################################################
# Compiler & Linker Options
####################################################################################################
# Options for specific architecture
CPU = -mcpu=$(CORE) -mthumb
# Compile level
#LEVEL=-O0 -Os
LEVEL=-O2 -Os
# Section
SECTION = -std=gnu11 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding
# Warning config
WARNING = -Wall -Wextra -g
# Error config
ERROR += -Werror=implicit-function-declaration
ERROR += -Werror=int-conversion
ERROR += -Werror=sign-compare
ERROR += -Werror=uninitialized
# Generate dependent file
GENNER = -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)"

# All configurable compile options
# -fmacro-prefix-map=OLD=NEW is similar to -fdebug-prefix-map, 
# but addresses unreproducibility due to the use of __FILE__ macros in assert calls for example.
# (available since GCC 8 and Clang 10)
COMPILE_OPTION = $(CPU) $(LEVEL) $(SECTION) $(WARNING) $(ERROR) -fmacro-prefix-map=$(dir $<)=

# .c file and .s file generate dependent file
GENNER_DEPS_RAM=-MMD -MP -MF"$(@:%.o.ram=%.d)" -MT"$(@)"
GENNER_DEPS_FLASH=-MMD -MP -MF"$(@:%.o.flash=%.d)" -MT"$(@)"

# Link file
LINK=-T mem.ld -T sections.ld

# link script path
LINK_DIR=../link
LINK_PATH = -L$(LINK_DIR)

# Create map file
MAP = -Wl,-Map=$(TARGET).map

# Link for code size
LINKOPT = -Xlinker --gc-sections $(LINK_PATH) $(MAP) -u _printf_float


####################################################################################################
# gcc Library
####################################################################################################
# Use newlib-nano. To disable it, specify USE_NANO=
GNULIB_NANO = --specs=nano.specs
GNULIB_SEMIHOST = --specs=rdimon.specs
GNULIB_NOHOST = --specs=nosys.specs

# Called in makefile
USE_LIB = $(GNULIB_NANO) $(GNULIB_NOHOST)


####################################################################################################
# Commands
####################################################################################################
RM    := -rm -rf
RMDIR := -rmdir -p
MKDIR := -mkdir -p
MV    := -mv
CP    := -cp -p

MAKE  := make -j 10000


####################################################################################################
# export variable to use sub makefile
####################################################################################################
# Complier
export AS CC CXX CPP AR
export OBJCOPY OBJDUMP READELF SIZE RANLIB

# Commands
export RM RMDIR MKDIR MV MAKE