####################################################################################################
# directory
####################################################################################################
DIAG_CTRL_SRC_DIR := $(SYSAPP_SRC_DIR)/diag_ctrl


####################################################################################################
# DIAG source file
####################################################################################################
SRCS_DIAG_CTRL_C_RAM = \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


SRCS_DIAG_CTRL_C_FLASH = \
	$(wildcard $(DIAG_CTRL_SRC_DIR)/src/*.c) \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


SRCS_DIAG_CTRL_EXCLUDE_LIB_RAM = \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


SRCS_DIAG_CTRL_EXCLUDE_LIB_FLASH = \

#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


SRCS_DIAG_CTRL_EXCLUDE_PRJ = \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


####################################################################################################
# source
####################################################################################################
DIAG_CTRL_BASE_FILE_GROUP  = $(SRCS_DIAG_CTRL_EXCLUDE_LIB_FLASH)
DIAG_CTRL_OTHER_FILE_GROUP = $(SRCS_DIAG_CTRL_C_RAM) $(SRCS_DIAG_CTRL_C_FLASH) $(SRCS_DIAG_CTRL_EXCLUDE_LIB_RAM)

TMP_DIAG_CTRL_FILE = $(filter-out $(DIAG_CTRL_OTHER_FILE_GROUP), $(DIAG_CTRL_BASE_FILE_GROUP))

SRCS_DIAG_CTRL_RAM          += $(filter-out $(SRCS_DIAG_CTRL_EXCLUDE_PRJ), $(SRCS_DIAG_CTRL_C_RAM))
SRCS_DIAG_CTRL_FLASH        += $(filter-out $(SRCS_DIAG_CTRL_EXCLUDE_PRJ), $(SRCS_DIAG_CTRL_C_FLASH))
DIAG_CTRL_EXCLUDE_LIB_RAM   += $(filter-out $(SRCS_DIAG_CTRL_EXCLUDE_PRJ), $(SRCS_DIAG_CTRL_EXCLUDE_LIB_RAM))
DIAG_CTRL_EXCLUDE_LIB_FLASH += $(filter-out $(SRCS_DIAG_CTRL_EXCLUDE_PRJ), $(TMP_DIAG_CTRL_FILE))


####################################################################################################
# include path
####################################################################################################
DEPS_PATH_DIAG_CTRL += $(DIAG_CTRL_SRC_DIR)/include


####################################################################################################
# add to sysapp.mk
####################################################################################################
SRCS_SYSAPP_RAM   += $(SRCS_DIAG_CTRL_RAM)
SRCS_SYSAPP_FLASH += $(SRCS_DIAG_CTRL_FLASH)

SYSAPP_EXCLUDE_LIB_RAM   += $(DIAG_CTRL_EXCLUDE_LIB_RAM)
SYSAPP_EXCLUDE_LIB_FLASH += $(DIAG_CTRL_EXCLUDE_LIB_FLASH)

DEPS_PATH_SYSAPP += $(DEPS_PATH_DIAG_CTRL)