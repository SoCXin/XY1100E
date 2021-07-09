####################################################################################################
# directory
####################################################################################################
RTCTMR_SRC_DIR := $(SYSAPP_SRC_DIR)/rtc_tmr


####################################################################################################
# RTCTMR source file
####################################################################################################
SRCS_RTCTMR_C_RAM = \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


SRCS_RTCTMR_C_FLASH = \
	$(wildcard $(RTCTMR_SRC_DIR)/src/*.c) \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


SRCS_RTCTMR_EXCLUDE_LIB_RAM = \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


SRCS_RTCTMR_EXCLUDE_LIB_FLASH = \

#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


SRCS_RTCTMR_EXCLUDE_PRJ = \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


####################################################################################################
# source
####################################################################################################
RTCTMR_BASE_FILE_GROUP  = $(SRCS_RTCTMR_EXCLUDE_LIB_FLASH)
RTCTMR_OTHER_FILE_GROUP = $(SRCS_RTCTMR_C_RAM) $(SRCS_RTCTMR_C_FLASH) $(SRCS_RTCTMR_EXCLUDE_LIB_RAM)

TMP_RTCTMR_FILE = $(filter-out $(RTCTMR_OTHER_FILE_GROUP), $(RTCTMR_BASE_FILE_GROUP))

SRCS_RTCTMR_RAM          += $(filter-out $(SRCS_RTCTMR_EXCLUDE_PRJ), $(SRCS_RTCTMR_C_RAM))
SRCS_RTCTMR_FLASH        += $(filter-out $(SRCS_RTCTMR_EXCLUDE_PRJ), $(SRCS_RTCTMR_C_FLASH))
RTCTMR_EXCLUDE_LIB_RAM   += $(filter-out $(SRCS_RTCTMR_EXCLUDE_PRJ), $(SRCS_RTCTMR_EXCLUDE_LIB_RAM))
RTCTMR_EXCLUDE_LIB_FLASH += $(filter-out $(SRCS_RTCTMR_EXCLUDE_PRJ), $(TMP_RTCTMR_FILE))


####################################################################################################
# include path
####################################################################################################
DEPS_PATH_RTCTMR += $(RTCTMR_SRC_DIR)/inc


####################################################################################################
# add to sysapp.mk
####################################################################################################
SRCS_SYSAPP_RAM   += $(SRCS_RTCTMR_RAM)
SRCS_SYSAPP_FLASH += $(SRCS_RTCTMR_FLASH)

SYSAPP_EXCLUDE_LIB_RAM   += $(RTCTMR_EXCLUDE_LIB_RAM)
SYSAPP_EXCLUDE_LIB_FLASH += $(RTCTMR_EXCLUDE_LIB_FLASH)

DEPS_PATH_SYSAPP += $(DEPS_PATH_RTCTMR)