####################################################################################################
# directory
####################################################################################################
ATCMD_SRC_DIR := $(SYSAPP_SRC_DIR)/at_cmd


####################################################################################################
# ATCMD source file
####################################################################################################
SRCS_ATCMD_C_RAM = \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


SRCS_ATCMD_C_FLASH = \
	$(wildcard $(ATCMD_SRC_DIR)/src/*.c) \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


SRCS_ATCMD_EXCLUDE_LIB_RAM = \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


SRCS_ATCMD_EXCLUDE_LIB_FLASH = \

#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


SRCS_ATCMD_EXCLUDE_PRJ = \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


####################################################################################################
# source
####################################################################################################
ATCMD_BASE_FILE_GROUP  = $(SRCS_ATCMD_EXCLUDE_LIB_FLASH)
ATCMD_OTHER_FILE_GROUP = $(SRCS_ATCMD_C_RAM) $(SRCS_ATCMD_C_FLASH) $(SRCS_ATCMD_EXCLUDE_LIB_RAM)

TMP_ATCMD_FILE = $(filter-out $(ATCMD_OTHER_FILE_GROUP), $(ATCMD_BASE_FILE_GROUP))

SRCS_ATCMD_RAM          += $(filter-out $(SRCS_ATCMD_EXCLUDE_PRJ), $(SRCS_ATCMD_C_RAM))
SRCS_ATCMD_FLASH        += $(filter-out $(SRCS_ATCMD_EXCLUDE_PRJ), $(SRCS_ATCMD_C_FLASH))
ATCMD_EXCLUDE_LIB_RAM   += $(filter-out $(SRCS_ATCMD_EXCLUDE_PRJ), $(SRCS_ATCMD_EXCLUDE_LIB_RAM))
ATCMD_EXCLUDE_LIB_FLASH += $(filter-out $(SRCS_ATCMD_EXCLUDE_PRJ), $(TMP_ATCMD_FILE))


####################################################################################################
# include path
####################################################################################################
DEPS_PATH_ATCMD += $(ATCMD_SRC_DIR)/inc


####################################################################################################
# add to sysapp.mk
####################################################################################################
SRCS_SYSAPP_RAM   += $(SRCS_ATCMD_RAM)
SRCS_SYSAPP_FLASH += $(SRCS_ATCMD_FLASH)

SYSAPP_EXCLUDE_LIB_RAM   += $(ATCMD_EXCLUDE_LIB_RAM)
SYSAPP_EXCLUDE_LIB_FLASH += $(ATCMD_EXCLUDE_LIB_FLASH)

DEPS_PATH_SYSAPP += $(DEPS_PATH_ATCMD)
