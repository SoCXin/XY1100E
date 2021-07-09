####################################################################################################
# directory
####################################################################################################
LPM_SRC_DIR := $(SYSLIB_SRC_DIR)/lpm


####################################################################################################
# source file
####################################################################################################
SRCS_LPM_C_RAM = \
	$(wildcard $(LPM_SRC_DIR)/src/*.c) \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


SRCS_LPM_C_FLASH = \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


SRCS_LPM_EXCLUDE_LIB_RAM = \

#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


SRCS_LPM_EXCLUDE_LIB_FLASH = \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


SRCS_LPM_EXCLUDE_PRJ = \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


####################################################################################################
# source
####################################################################################################
LPM_BASE_FILE_GROUP  = $(SRCS_LPM_EXCLUDE_LIB_RAM)
LPM_OTHER_FILE_GROUP = $(SRCS_LPM_C_RAM) $(SRCS_LPM_C_FLASH) $(SRCS_LPM_EXCLUDE_LIB_FLASH)

TMP_LPM_FILE = $(filter-out $(LPM_OTHER_FILE_GROUP), $(LPM_BASE_FILE_GROUP))

SRCS_LPM_RAM          += $(filter-out $(SRCS_LPM_EXCLUDE_PRJ), $(SRCS_LPM_C_RAM))
SRCS_LPM_FLASH        += $(filter-out $(SRCS_LPM_EXCLUDE_PRJ), $(SRCS_LPM_C_FLASH))
LPM_EXCLUDE_LIB_RAM   += $(filter-out $(SRCS_LPM_EXCLUDE_PRJ), $(TMP_LPM_FILE))
LPM_EXCLUDE_LIB_FLASH += $(filter-out $(SRCS_LPM_EXCLUDE_PRJ), $(SRCS_LPM_EXCLUDE_LIB_FLASH))


####################################################################################################
# include path
####################################################################################################
DEPS_PATH_LPM += $(LPM_SRC_DIR)/inc


####################################################################################################
# add to syslib.mk
####################################################################################################
SRCS_SYSLIB_RAM   += $(SRCS_LPM_RAM)
SRCS_SYSLIB_FLASH += $(SRCS_LPM_FLASH)

SYSLIB_EXCLUDE_LIB_RAM   += $(LPM_EXCLUDE_LIB_RAM)
SYSLIB_EXCLUDE_LIB_FLASH += $(LPM_EXCLUDE_LIB_FLASH)

DEPS_PATH_SYSLIB += $(DEPS_PATH_LPM)