####################################################################################################
# directory
####################################################################################################
ATCTRL_SRC_DIR := $(SYSAPP_SRC_DIR)/at_ctrl


####################################################################################################
# ATCTRL source file
####################################################################################################
SRCS_ATCTRL_C_RAM = \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


SRCS_ATCTRL_C_FLASH = \
	$(wildcard $(ATCTRL_SRC_DIR)/src/*.c) \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


SRCS_ATCTRL_EXCLUDE_LIB_RAM = \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


SRCS_ATCTRL_EXCLUDE_LIB_FLASH = \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


SRCS_ATCTRL_EXCLUDE_PRJ = \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


####################################################################################################
# source
####################################################################################################
ATCTRL_BASE_FILE_GROUP  = $(SRCS_ATCTRL_C_FLASH)
ATCTRL_OTHER_FILE_GROUP = $(SRCS_ATCTRL_C_RAM) $(SRCS_ATCTRL_EXCLUDE_LIB_RAM) $(SRCS_ATCTRL_EXCLUDE_LIB_FLASH)

TMP_ATCTRL_FILE = $(filter-out $(ATCTRL_OTHER_FILE_GROUP), $(ATCTRL_BASE_FILE_GROUP))

SRCS_ATCTRL_RAM          += $(filter-out $(SRCS_ATCTRL_EXCLUDE_PRJ), $(SRCS_ATCTRL_C_RAM))
SRCS_ATCTRL_FLASH        += $(filter-out $(SRCS_ATCTRL_EXCLUDE_PRJ), $(TMP_ATCTRL_FILE))
ATCTRL_EXCLUDE_LIB_RAM   += $(filter-out $(SRCS_ATCTRL_EXCLUDE_PRJ), $(SRCS_ATCTRL_EXCLUDE_LIB_RAM))
ATCTRL_EXCLUDE_LIB_FLASH += $(filter-out $(SRCS_ATCTRL_EXCLUDE_PRJ), $(SRCS_ATCTRL_EXCLUDE_LIB_FLASH))


####################################################################################################
# include path
####################################################################################################
DEPS_PATH_ATCTRL += $(ATCTRL_SRC_DIR)/inc


####################################################################################################
# add to sysapp.mk
####################################################################################################
SRCS_SYSAPP_RAM   += $(SRCS_ATCTRL_RAM)
SRCS_SYSAPP_FLASH += $(SRCS_ATCTRL_FLASH)

SYSAPP_EXCLUDE_LIB_RAM   += $(ATCTRL_EXCLUDE_LIB_RAM)
SYSAPP_EXCLUDE_LIB_FLASH += $(ATCTRL_EXCLUDE_LIB_FLASH)

DEPS_PATH_SYSAPP += $(DEPS_PATH_ATCTRL)