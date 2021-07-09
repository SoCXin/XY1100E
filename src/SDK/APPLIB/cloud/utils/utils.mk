####################################################################################################
# directory
####################################################################################################
UTILS_SRC_DIR := $(CLOUD_SRC_DIR)/utils


####################################################################################################
# source file
####################################################################################################
SRCS_UTILS_C_RAM = \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


SRCS_UTILS_C_FLASH = \
	$(wildcard $(UTILS_SRC_DIR)/src/*.c) \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


SRCS_UTILS_EXCLUDE_LIB_RAM = \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


SRCS_UTILS_EXCLUDE_LIB_FLASH = \

#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


SRCS_UTILS_EXCLUDE_PRJ = \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


####################################################################################################
# source
####################################################################################################
TMP_UTILS_FILE := $(filter-out $(SRCS_UTILS_C_RAM), $(SRCS_UTILS_EXCLUDE_LIB_FLASH))
TMP_UTILS_FILE := $(filter-out $(SRCS_UTILS_C_FLASH), $(TMP_UTILS_FILE))
TMP_UTILS_FILE := $(filter-out $(SRCS_UTILS_EXCLUDE_LIB_RAM), $(TMP_UTILS_FILE))
TMP_UTILS_FILE := $(filter-out $(SRCS_UTILS_EXCLUDE_PRJ), $(TMP_UTILS_FILE))

SRCS_UTILS_RAM   += $(SRCS_UTILS_C_RAM)
SRCS_UTILS_FLASH += $(SRCS_UTILS_C_FLASH)

UTILS_EXCLUDE_LIB_RAM   += $(SRCS_UTILS_EXCLUDE_LIB_RAM)
UTILS_EXCLUDE_LIB_FLASH += $(TMP_UTILS_FILE)


####################################################################################################
# include path
####################################################################################################
DEPS_PATH_UTILS += $(UTILS_SRC_DIR)/inc


####################################################################################################
# add to sysapp.mk
####################################################################################################
SRCS_APPLIB_RAM   += $(SRCS_UTILS_RAM)
SRCS_APPLIB_FLASH += $(SRCS_UTILS_FLASH)

APPLIB_EXCLUDE_LIB_RAM   += $(UTILS_EXCLUDE_LIB_RAM)
APPLIB_EXCLUDE_LIB_FLASH += $(UTILS_EXCLUDE_LIB_FLASH)

DEPS_PATH_APPLIB += $(DEPS_PATH_UTILS)