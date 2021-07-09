####################################################################################################
# directory
####################################################################################################
PING_SRC_DIR := $(APPLIB_SRC_DIR)/ping


####################################################################################################
# source file
####################################################################################################
SRCS_PING_C_RAM = \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


SRCS_PING_C_FLASH = \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


SRCS_PING_EXCLUDE_LIB_RAM = \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


SRCS_PING_EXCLUDE_LIB_FLASH = \
	$(wildcard $(PING_SRC_DIR)/xy_ping/src/*.c) \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


SRCS_PING_EXCLUDE_PRJ = \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


####################################################################################################
# source
####################################################################################################
TMP_PING_FILE := $(filter-out $(SRCS_PING_C_RAM), $(SRCS_PING_EXCLUDE_LIB_FLASH))
TMP_PING_FILE := $(filter-out $(SRCS_PING_C_FLASH), $(TMP_PING_FILE))
TMP_PING_FILE := $(filter-out $(SRCS_PING_EXCLUDE_LIB_RAM), $(TMP_PING_FILE))
TMP_PING_FILE := $(filter-out $(SRCS_PING_EXCLUDE_PRJ), $(TMP_PING_FILE))

SRCS_PING_RAM   += $(SRCS_PING_C_RAM)
SRCS_PING_FLASH += $(SRCS_PING_C_FLASH)

PING_EXCLUDE_LIB_RAM   += $(SRCS_PING_EXCLUDE_LIB_RAM)
PING_EXCLUDE_LIB_FLASH += $(TMP_PING_FILE)


####################################################################################################
# include path
####################################################################################################
DEPS_PATH_PING += $(PING_SRC_DIR)/xy_ping/inc


####################################################################################################
# add to sysapp.mk
####################################################################################################
SRCS_APPLIB_RAM   += $(SRCS_PING_RAM)
SRCS_APPLIB_FLASH += $(SRCS_PING_FLASH)

APPLIB_EXCLUDE_LIB_RAM   += $(PING_EXCLUDE_LIB_RAM)
APPLIB_EXCLUDE_LIB_FLASH += $(PING_EXCLUDE_LIB_FLASH)

DEPS_PATH_APPLIB += $(DEPS_PATH_PING)