####################################################################################################
# directory
####################################################################################################
SYSTEM_SRC_DIR := $(SYSAPP_SRC_DIR)/system


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
# add to sysapp.mk
####################################################################################################
SRCS_SYSAPP_RAM   += $(SRCS_SYSTEM_RAM)
SRCS_SYSAPP_FLASH += $(SRCS_SYSTEM_FLASH)

SYSAPP_EXCLUDE_LIB_RAM   += $(SYSTEM_EXCLUDE_LIB_RAM)
SYSAPP_EXCLUDE_LIB_FLASH += $(SYSTEM_EXCLUDE_LIB_FLASH)

DEPS_PATH_SYSAPP += $(DEPS_PATH_SYSTEM)