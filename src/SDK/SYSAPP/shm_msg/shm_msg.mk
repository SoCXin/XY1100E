####################################################################################################
# directory
####################################################################################################
SHMMSG_SRC_DIR := $(SYSAPP_SRC_DIR)/shm_msg


####################################################################################################
# SHMMSG source file
####################################################################################################
SRCS_SHMMSG_C_RAM = \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


SRCS_SHMMSG_C_FLASH = \
	$(wildcard $(SHMMSG_SRC_DIR)/src/*.c) \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


SRCS_SHMMSG_EXCLUDE_LIB_RAM = \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


SRCS_SHMMSG_EXCLUDE_LIB_FLASH = \

#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


SRCS_SHMMSG_EXCLUDE_PRJ = \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


####################################################################################################
# source
####################################################################################################
SHMMSG_BASE_FILE_GROUP  = $(SRCS_SHMMSG_EXCLUDE_LIB_FLASH)
SHMMSG_OTHER_FILE_GROUP = $(SRCS_SHMMSG_C_RAM) $(SRCS_SHMMSG_C_FLASH) $(SRCS_SHMMSG_EXCLUDE_LIB_RAM)

TMP_SHMMSG_FILE = $(filter-out $(SHMMSG_OTHER_FILE_GROUP), $(SHMMSG_BASE_FILE_GROUP))

SRCS_SHMMSG_RAM          += $(filter-out $(SRCS_SHMMSG_EXCLUDE_PRJ), $(SRCS_SHMMSG_C_RAM))
SRCS_SHMMSG_FLASH        += $(filter-out $(SRCS_SHMMSG_EXCLUDE_PRJ), $(SRCS_SHMMSG_C_FLASH))
SHMMSG_EXCLUDE_LIB_RAM   += $(filter-out $(SRCS_SHMMSG_EXCLUDE_PRJ), $(SRCS_SHMMSG_EXCLUDE_LIB_RAM))
SHMMSG_EXCLUDE_LIB_FLASH += $(filter-out $(SRCS_SHMMSG_EXCLUDE_PRJ), $(TMP_SHMMSG_FILE))


####################################################################################################
# include path
####################################################################################################
DEPS_PATH_SHMMSG += $(SHMMSG_SRC_DIR)/inc


####################################################################################################
# add to sysapp.mk
####################################################################################################
SRCS_SYSAPP_RAM   += $(SRCS_SHMMSG_RAM)
SRCS_SYSAPP_FLASH += $(SRCS_SHMMSG_FLASH)

SYSAPP_EXCLUDE_LIB_RAM   += $(SHMMSG_EXCLUDE_LIB_RAM)
SYSAPP_EXCLUDE_LIB_FLASH += $(SHMMSG_EXCLUDE_LIB_FLASH)

DEPS_PATH_SYSAPP += $(DEPS_PATH_SHMMSG)