####################################################################################################
# directory
####################################################################################################
DIAG_CTRL_SRC_DIR := $(SYSLIB_SRC_DIR)/diag_ctrl


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
# add to syslib.mk
####################################################################################################
SRCS_SYSLIB_RAM   += $(SRCS_DIAG_CTRL_RAM)
SRCS_SYSLIB_FLASH += $(SRCS_DIAG_CTRL_FLASH)

SYSLIB_EXCLUDE_LIB_RAM   += $(DIAG_CTRL_EXCLUDE_LIB_RAM)
SYSLIB_EXCLUDE_LIB_FLASH += $(DIAG_CTRL_EXCLUDE_LIB_FLASH)

DEPS_PATH_SYSLIB += $(DEPS_PATH_DIAG_CTRL)