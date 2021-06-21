####################################################################################################
# directory
####################################################################################################
NVIC_SRC_DIR := $(ARCH_SRC_DIR)/nvic


####################################################################################################
# source file
####################################################################################################
SRCS_NVIC_C_RAM = \
	$(wildcard $(NVIC_SRC_DIR)/src/*.c) \
	$(wildcard $(NVIC_SRC_DIR)/src/*.s) \
	$(wildcard $(NVIC_SRC_DIR)/src/*.S) \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


SRCS_NVIC_C_FLASH = \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


SRCS_NVIC_EXCLUDE_LIB_RAM = \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


SRCS_NVIC_EXCLUDE_LIB_FLASH = \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


SRCS_NVIC_EXCLUDE_PRJ = \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


####################################################################################################
# source
####################################################################################################
NVIC_BASE_FILE_GROUP  = $(SRCS_NVIC_C_FLASH)
NVIC_OTHER_FILE_GROUP = $(SRCS_NVIC_C_RAM) $(SRCS_NVIC_EXCLUDE_LIB_RAM) $(SRCS_NVIC_EXCLUDE_LIB_FLASH)

TMP_NVIC_FILE = $(filter-out $(NVIC_OTHER_FILE_GROUP), $(NVIC_BASE_FILE_GROUP))

SRCS_NVIC_RAM          += $(filter-out $(SRCS_NVIC_EXCLUDE_PRJ), $(SRCS_NVIC_C_RAM))
SRCS_NVIC_FLASH        += $(filter-out $(SRCS_NVIC_EXCLUDE_PRJ), $(TMP_NVIC_FILE))
NVIC_EXCLUDE_LIB_RAM   += $(filter-out $(SRCS_NVIC_EXCLUDE_PRJ), $(SRCS_NVIC_EXCLUDE_LIB_RAM))
NVIC_EXCLUDE_LIB_FLASH += $(filter-out $(SRCS_NVIC_EXCLUDE_PRJ), $(SRCS_NVIC_EXCLUDE_LIB_FLASH))


####################################################################################################
# include path
####################################################################################################
DEPS_PATH_NVIC += $(NVIC_SRC_DIR)/inc


####################################################################################################
# add to arch.mk
####################################################################################################
SRCS_ARCH_RAM   += $(SRCS_NVIC_RAM)
SRCS_ARCH_FLASH += $(SRCS_NVIC_FLASH)

ARCH_EXCLUDE_LIB_RAM   += $(NVIC_EXCLUDE_LIB_RAM)
ARCH_EXCLUDE_LIB_FLASH += $(NVIC_EXCLUDE_LIB_FLASH)

DEPS_PATH_ARCH += $(DEPS_PATH_NVIC)