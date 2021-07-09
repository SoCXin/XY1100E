####################################################################################################
# mainly makefile, call sub makefile
####################################################################################################


####################################################################################################
# Some global path
####################################################################################################
OS_NAME := $(shell uname -o)
LC_OS_NAME := $(shell echo $(OS_NAME) | tr A-Z a-z)

# Determine current platform
#ifeq ($(LC_OS_NAME),cygwin)
#CUR_DIR     := $(subst \,/,$(shell cygpath -w $(abspath .)))
#TOP_DIR     := $(subst \,/,$(shell cygpath -w $(abspath ../../../..)))
#PROJECT_DIR := $(subst \,/,$(shell cygpath -w $(abspath ../..)))
#OBJECT_DIR  := $(subst \,/,$(shell cygpath -w $(abspath ../object)))
#else
#CUR_DIR     := $(abspath .)
#TOP_DIR     := $(abspath ../../../..)
#PROJECT_DIR := $(abspath ../..)
#OBJECT_DIR  := $(abspath ../object)
#endif

CUR_DIR     := $(abspath .)
TOP_DIR     := ../../../
PROJECT_DIR := ../
OBJECT_DIR  := ../object
BIN_DIR     := ../bin
ELF_DIR     := ../elf
LIBRARY_DIR := ../library
COMMAND_DIR := ../command
IGNORE_DIR  := $(TOP_DIR)/.ignore
LOGINFO_DIR := ../loginfo
TARGET      := $(shell basename $(abspath $(PROJECT_DIR)))

# Library path
LIBRARY_PATH := -L$(LIBRARY_DIR)

# save object file list to file
OBJ_LIST_FILE := $(IGNORE_DIR)/objfiles.list

# add some path to system PATH
PATH := $(abspath $(COMMAND_DIR)):$(PATH)

# compiling depends path, add "-I" option as the parament to gcc
COMP_DEPS_PATH = $(foreach CUR_PATH,$(SRCS_DEPS_PATH),-I$(CUR_PATH))


SUBDIRS := $(shell ls -l $(TOP_DIR) | grep ^d | awk '{if($$9 != "TARGETS") print $$9}')

#$(foreach SUB_DIR, $(SUBDIRS), $(TOP_DIR)/$(SUB_DIR)/$(SUB_DIR).mk)

#LOWER_UPPER = $(shell echo $(LOWER) | tr a-z A-Z)
#UPPER_LOWER = $(shell echo $(LOWER) | tr A-Z a-z)

####################################################################################################
# files that must be include
####################################################################################################
include config.mk
include feature.mk
include reduce.mk
include define.mk

-include $(TOP_DIR)/ARCH/arch.mk
-include $(TOP_DIR)/DRIVERS/drivers.mk
-include $(TOP_DIR)/KERNEL/kernel.mk
-include $(TOP_DIR)/SYSAPP/sysapp.mk
-include $(TOP_DIR)/APPLIB/applib.mk
-include $(TOP_DIR)/USERAPP/userapp.mk


####################################################################################################
# All Target
####################################################################################################
# target all and clean
all: pre_build secondary_outputs post_build

clean: pre_clean first_clean
	@echo "cleaning..."
	@$(RM) $(OBJECT_DIR) $(BIN_DIR) $(ELF_DIR)
	@$(RM) $(TARGET).*
	@echo "clean done"

# show string, example: make showv PAR=RM
shows:
	@echo ''
	@echo "***********************************************************************"
	@echo $(PAR)=
	@echo $($(PAR))
	@echo "***********************************************************************"
	@echo ''

# show string and replace to '\n', example: make shows PAR=C_SRC_FILES
shown:
	@echo ''
	@echo "***********************************************************************"
	@echo $(PAR)=
	@echo $($(PAR)) | tr ' ' '\n'
	@echo "***********************************************************************"
	@echo ''

# do at compiling step
pre_build: create_list_file

post_build:

pre_link:

pre_clean:

secondary_outputs: $(TARGET).elf
	@echo "Creating flash.bin..."
	@$(OBJCOPY) -O binary -j .flashtext $(TARGET).elf flash.bin
	@echo "Creating ram.bin..."
	@$(OBJCOPY) -O binary -j .isr_vector -j .text -j .data $(TARGET).elf ram.bin
	@echo "Creating siz..."
	@$(OBJSIZE) @$(OBJ_LIST_FILE) > $(TARGET).siz
	@echo "Creating lst..."
	@$(OBJDUMP) -S $(TARGET).elf > $(TARGET).lst
	@echo "Creating dumpelf..."
	@$(READELF) -a $(TARGET).elf > $(TARGET).dumpelf
	@echo "Calculating size..."
	@$(OBJSIZE) --format=berkeley "$(TARGET).elf"
	@python m3ld.py ./
	@echo "Moving files..."
	@$(MKDIR) $(BIN_DIR)
	@$(MKDIR) $(ELF_DIR)
	@$(MV) flash.bin   $(BIN_DIR)
	@$(MV) ram.bin     $(BIN_DIR)
	@$(MV) arm.img     $(BIN_DIR)
	@$(MV) $(TARGET).* $(ELF_DIR)
	@echo "compiling done!!!"


# use --start-group and --end-group to resolve the interdependencies between libraries
LINK_WITH_LIBRARY += -Wl,--start-group $(LIBRARY_PATH) $(SRCS_LIBS_INC) -Wl,--end-group

$(TARGET).elf: $(MAIN_ALL) pre_link
	@echo "linking..."
	@$(CXX) $(COMPILE_OPTION) $(LINK) $(LINKOPT) $(USE_LIB) -o "$(TARGET).elf" @$(OBJ_LIST_FILE) $(LINK_WITH_LIBRARY)

first_clean: $(MAIN_CLEAN)


# create folder before target all
create_list_file:
	@$(MKDIR) $(IGNORE_DIR)
	@echo $(SRCS_OBJ_FILES) | tr ' ' '\n' > $(OBJ_LIST_FILE)


.PHONY: all clean shows shown