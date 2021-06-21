####################################################################################################
# directory
####################################################################################################
BASIC_SRC_DIR := $(USERAPP_SRC_DIR)/basic


####################################################################################################
# adapt source file
####################################################################################################
SRCS_BASIC_C_RAM = \
	$(wildcard $(BASIC_SRC_DIR)/src/main.c) \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


SRCS_BASIC_C_FLASH = \

#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


SRCS_BASIC_EXCLUDE_LIB_RAM = \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


SRCS_BASIC_EXCLUDE_LIB_FLASH = \
	$(wildcard $(BASIC_SRC_DIR)/src/*.c) \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


SRCS_BASIC_EXCLUDE_PRJ = \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


####################################################################################################
# source
####################################################################################################
TMP_BASIC_FILE := $(filter-out $(SRCS_BASIC_C_RAM), $(SRCS_BASIC_C_FLASH))
TMP_BASIC_FILE := $(filter-out $(SRCS_BASIC_EXCLUDE_LIB_RAM), $(TMP_BASIC_FILE))
TMP_BASIC_FILE := $(filter-out $(SRCS_BASIC_EXCLUDE_LIB_FLASH), $(TMP_BASIC_FILE))
TMP_BASIC_FILE := $(filter-out $(SRCS_BASIC_EXCLUDE_PRJ), $(TMP_BASIC_FILE))

SRCS_BASIC_RAM   += $(SRCS_BASIC_C_RAM)
SRCS_BASIC_FLASH += $(TMP_BASIC_FILE)

BASIC_EXCLUDE_LIB_RAM   += $(SRCS_BASIC_EXCLUDE_LIB_RAM)
BASIC_EXCLUDE_LIB_FLASH += $(SRCS_BASIC_EXCLUDE_LIB_FLASH)


####################################################################################################
# include path
####################################################################################################
DEPS_PATH_BASIC += $(BASIC_SRC_DIR)/inc


####################################################################################################
# add to sysapp.mk
####################################################################################################
SRCS_USERAPP_RAM   += $(SRCS_BASIC_RAM)
SRCS_USERAPP_FLASH += $(SRCS_BASIC_FLASH)

USERAPP_EXCLUDE_LIB_RAM   += $(BASIC_EXCLUDE_LIB_RAM)
USERAPP_EXCLUDE_LIB_FLASH += $(BASIC_EXCLUDE_LIB_FLASH)

DEPS_PATH_USERAPP += $(DEPS_PATH_BASIC)