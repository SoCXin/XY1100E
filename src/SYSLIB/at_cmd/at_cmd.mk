####################################################################################################
# directory
####################################################################################################
ATCMD_SRC_DIR := $(SYSLIB_SRC_DIR)/at_cmd


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
# add to syslib.mk
####################################################################################################
SRCS_SYSLIB_RAM   += $(SRCS_ATCMD_RAM)
SRCS_SYSLIB_FLASH += $(SRCS_ATCMD_FLASH)

SYSLIB_EXCLUDE_LIB_RAM   += $(ATCMD_EXCLUDE_LIB_RAM)
SYSLIB_EXCLUDE_LIB_FLASH += $(ATCMD_EXCLUDE_LIB_FLASH)

DEPS_PATH_SYSLIB += $(DEPS_PATH_ATCMD)
