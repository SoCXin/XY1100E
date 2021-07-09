####################################################################################################
# directory
####################################################################################################
CORECM3_SRC_DIR := $(ARCH_SRC_DIR)/core_cm3


####################################################################################################
# source file
####################################################################################################
SRCS_CORECM3_C_RAM = \
	$(wildcard $(CORECM3_SRC_DIR)/src/*.c) \
	$(wildcard $(CORECM3_SRC_DIR)/src/*.s) \
	$(wildcard $(CORECM3_SRC_DIR)/src/*.S) \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


SRCS_CORECM3_C_FLASH = \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


SRCS_CORECM3_EXCLUDE_LIB_RAM = \
	$(CORECM3_SRC_DIR)/src/core_cm3.c \
	$(CORECM3_SRC_DIR)/src/HardFault.c \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


SRCS_CORECM3_EXCLUDE_LIB_FLASH = \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


SRCS_CORECM3_EXCLUDE_PRJ = \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


####################################################################################################
# source
####################################################################################################
TMP_CORECM3_FILE := $(filter-out $(SRCS_CORECM3_C_FLASH), $(SRCS_CORECM3_C_RAM))
TMP_CORECM3_FILE := $(filter-out $(SRCS_CORECM3_EXCLUDE_LIB_RAM), $(TMP_CORECM3_FILE))
TMP_CORECM3_FILE := $(filter-out $(SRCS_CORECM3_EXCLUDE_LIB_FLASH), $(TMP_CORECM3_FILE))
TMP_CORECM3_FILE := $(filter-out $(SRCS_CORECM3_EXCLUDE_PRJ), $(TMP_CORECM3_FILE))

SRCS_CORECM3_RAM   += $(TMP_CORECM3_FILE)
SRCS_CORECM3_FLASH += $(SRCS_CORECM3_C_FLASH)

CORECM3_EXCLUDE_LIB_RAM   += $(SRCS_CORECM3_EXCLUDE_LIB_RAM)
CORECM3_EXCLUDE_LIB_FLASH += $(SRCS_CORECM3_EXCLUDE_LIB_FLASH)

####################################################################################################
# include path
####################################################################################################
DEPS_PATH_CORECM3 += $(CORECM3_SRC_DIR)/inc


####################################################################################################
# add to arch.mk
####################################################################################################
SRCS_ARCH_RAM   += $(SRCS_CORECM3_RAM)
SRCS_ARCH_FLASH += $(SRCS_CORECM3_FLASH)

ARCH_EXCLUDE_LIB_RAM   += $(CORECM3_EXCLUDE_LIB_RAM)
ARCH_EXCLUDE_LIB_FLASH += $(CORECM3_EXCLUDE_LIB_FLASH)

DEPS_PATH_ARCH += $(DEPS_PATH_CORECM3)