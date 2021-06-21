####################################################################################################
# directory
####################################################################################################
SYSTEM_SRC_DIR := $(SYSLIB_SRC_DIR)/system


####################################################################################################
# source file
####################################################################################################
SRCS_SYSTEM_C_RAM = \

#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


SRCS_SYSTEM_C_FLASH = \

#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


SRCS_SYSTEM_EXCLUDE_LIB_RAM = \
	$(SYSTEM_SRC_DIR)/src/xy_hwi.c \
	$(SYSTEM_SRC_DIR)/src/xy_mem.c \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


SRCS_SYSTEM_EXCLUDE_LIB_FLASH = \
	$(wildcard $(SYSTEM_SRC_DIR)/src/*.c) \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


SRCS_SYSTEM_EXCLUDE_PRJ = \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


####################################################################################################
# source
####################################################################################################
SYSTEM_BASE_FILE_GROUP  = $(SRCS_SYSTEM_EXCLUDE_LIB_FLASH)
SYSTEM_OTHER_FILE_GROUP = $(SRCS_SYSTEM_C_RAM) $(SRCS_SYSTEM_C_FLASH) $(SRCS_SYSTEM_EXCLUDE_LIB_RAM)

TMP_SYSTEM_FILE = $(filter-out $(SYSTEM_OTHER_FILE_GROUP), $(SYSTEM_BASE_FILE_GROUP))

SRCS_SYSTEM_RAM          += $(filter-out $(SRCS_SYSTEM_EXCLUDE_PRJ), $(SRCS_SYSTEM_C_RAM))
SRCS_SYSTEM_FLASH        += $(filter-out $(SRCS_SYSTEM_EXCLUDE_PRJ), $(SRCS_SYSTEM_C_FLASH))
SYSTEM_EXCLUDE_LIB_RAM   += $(filter-out $(SRCS_SYSTEM_EXCLUDE_PRJ), $(SRCS_SYSTEM_EXCLUDE_LIB_RAM))
SYSTEM_EXCLUDE_LIB_FLASH += $(filter-out $(SRCS_SYSTEM_EXCLUDE_PRJ), $(TMP_SYSTEM_FILE))


####################################################################################################
# include path
####################################################################################################
DEPS_PATH_SYSTEM += $(SYSTEM_SRC_DIR)/inc


####################################################################################################
# add to syslib.mk
####################################################################################################
SRCS_SYSLIB_RAM   += $(SRCS_SYSTEM_RAM)
SRCS_SYSLIB_FLASH += $(SRCS_SYSTEM_FLASH)

SYSLIB_EXCLUDE_LIB_RAM   += $(SYSTEM_EXCLUDE_LIB_RAM)
SYSLIB_EXCLUDE_LIB_FLASH += $(SYSTEM_EXCLUDE_LIB_FLASH)

DEPS_PATH_SYSLIB += $(DEPS_PATH_SYSTEM)