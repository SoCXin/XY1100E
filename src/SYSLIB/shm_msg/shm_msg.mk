####################################################################################################
# directory
####################################################################################################
SHMMSG_SRC_DIR := $(SYSLIB_SRC_DIR)/shm_msg


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
# add to syslib.mk
####################################################################################################
SRCS_SYSLIB_RAM   += $(SRCS_SHMMSG_RAM)
SRCS_SYSLIB_FLASH += $(SRCS_SHMMSG_FLASH)

SYSLIB_EXCLUDE_LIB_RAM   += $(SHMMSG_EXCLUDE_LIB_RAM)
SYSLIB_EXCLUDE_LIB_FLASH += $(SHMMSG_EXCLUDE_LIB_FLASH)

DEPS_PATH_SYSLIB += $(DEPS_PATH_SHMMSG)