####################################################################################################
# directory
####################################################################################################
REDIRECT_SRC_DIR := $(SYSAPP_SRC_DIR)/redirect


####################################################################################################
# source file
####################################################################################################
SRCS_REDIRECT_C_RAM = \
	$(wildcard $(REDIRECT_SRC_DIR)/src/*.c) \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


SRCS_REDIRECT_C_FLASH = \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


SRCS_REDIRECT_EXCLUDE_LIB_RAM = \

#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


SRCS_REDIRECT_EXCLUDE_LIB_FLASH = \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


SRCS_REDIRECT_EXCLUDE_PRJ = \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


####################################################################################################
# source
####################################################################################################
REDIRECT_BASE_FILE_GROUP  = $(SRCS_REDIRECT_EXCLUDE_LIB_RAM)
REDIRECT_OTHER_FILE_GROUP = $(SRCS_REDIRECT_C_RAM) $(SRCS_REDIRECT_C_FLASH) $(SRCS_REDIRECT_EXCLUDE_LIB_FLASH)

TMP_REDIRECT_FILE = $(filter-out $(REDIRECT_OTHER_FILE_GROUP), $(REDIRECT_BASE_FILE_GROUP))

SRCS_REDIRECT_RAM          += $(filter-out $(SRCS_REDIRECT_EXCLUDE_PRJ), $(SRCS_REDIRECT_C_RAM))
SRCS_REDIRECT_FLASH        += $(filter-out $(SRCS_REDIRECT_EXCLUDE_PRJ), $(SRCS_REDIRECT_C_FLASH))
REDIRECT_EXCLUDE_LIB_RAM   += $(filter-out $(SRCS_REDIRECT_EXCLUDE_PRJ), $(TMP_REDIRECT_FILE))
REDIRECT_EXCLUDE_LIB_FLASH += $(filter-out $(SRCS_REDIRECT_EXCLUDE_PRJ), $(SRCS_REDIRECT_EXCLUDE_LIB_FLASH))


####################################################################################################
# include path
####################################################################################################
DEPS_PATH_REDIRECT += $(REDIRECT_SRC_DIR)/inc


####################################################################################################
# add to sysapp.mk
####################################################################################################
SRCS_SYSAPP_RAM   += $(SRCS_REDIRECT_RAM)
SRCS_SYSAPP_FLASH += $(SRCS_REDIRECT_FLASH)

SYSAPP_EXCLUDE_LIB_RAM   += $(REDIRECT_EXCLUDE_LIB_RAM)
SYSAPP_EXCLUDE_LIB_FLASH += $(REDIRECT_EXCLUDE_LIB_FLASH)

DEPS_PATH_SYSAPP += $(DEPS_PATH_REDIRECT)