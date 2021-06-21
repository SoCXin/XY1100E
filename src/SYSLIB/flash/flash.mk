####################################################################################################
# directory
####################################################################################################
FLASH_SRC_DIR := $(SYSLIB_SRC_DIR)/flash


####################################################################################################
# source file
####################################################################################################
SRCS_FLASH_C_RAM = \

#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


SRCS_FLASH_C_FLASH = \

#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


SRCS_FLASH_EXCLUDE_LIB_RAM = \
	$(FLASH_SRC_DIR)/src/flash_adapt.c \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


SRCS_FLASH_EXCLUDE_LIB_FLASH = \
	$(wildcard $(FLASH_SRC_DIR)/src/*.c) \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


SRCS_FLASH_EXCLUDE_PRJ = \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


####################################################################################################
# source
####################################################################################################
FLASH_BASE_FILE_GROUP  = $(SRCS_FLASH_EXCLUDE_LIB_FLASH)
FLASH_OTHER_FILE_GROUP = $(SRCS_FLASH_C_RAM) $(SRCS_FLASH_C_FLASH) $(SRCS_FLASH_EXCLUDE_LIB_RAM)

TMP_FLASH_FILE = $(filter-out $(FLASH_OTHER_FILE_GROUP), $(FLASH_BASE_FILE_GROUP))

SRCS_FLASH_RAM          += $(filter-out $(SRCS_FLASH_EXCLUDE_PRJ), $(SRCS_FLASH_C_RAM))
SRCS_FLASH_FLASH        += $(filter-out $(SRCS_FLASH_EXCLUDE_PRJ), $(SRCS_FLASH_C_FLASH))
FLASH_EXCLUDE_LIB_RAM   += $(filter-out $(SRCS_FLASH_EXCLUDE_PRJ), $(SRCS_FLASH_EXCLUDE_LIB_RAM))
FLASH_EXCLUDE_LIB_FLASH += $(filter-out $(SRCS_FLASH_EXCLUDE_PRJ), $(TMP_FLASH_FILE))


####################################################################################################
# include path
####################################################################################################
DEPS_PATH_FLASH += $(FLASH_SRC_DIR)/inc


####################################################################################################
# add to syslib.mk
####################################################################################################
SRCS_SYSLIB_RAM   += $(SRCS_FLASH_RAM)
SRCS_SYSLIB_FLASH += $(SRCS_FLASH_FLASH)

SYSLIB_EXCLUDE_LIB_RAM   += $(FLASH_EXCLUDE_LIB_RAM)
SYSLIB_EXCLUDE_LIB_FLASH += $(FLASH_EXCLUDE_LIB_FLASH)

DEPS_PATH_SYSLIB += $(DEPS_PATH_FLASH)