####################################################################################################
# directory
####################################################################################################
AT_SOCKET_SRC_DIR := $(APPLIB_SRC_DIR)/at_socket


####################################################################################################
# source file
####################################################################################################
SRCS_AT_SOCKET_C_RAM = \
	$(AT_SOCKET_SRC_DIR)/src/at_socket_ram.c \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


SRCS_AT_SOCKET_C_FLASH = \
	$(wildcard $(AT_SOCKET_SRC_DIR)/src/*.c) \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


SRCS_AT_SOCKET_EXCLUDE_LIB_RAM = \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


SRCS_AT_SOCKET_EXCLUDE_LIB_FLASH = \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


SRCS_AT_SOCKET_EXCLUDE_PRJ = \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


####################################################################################################
# source
####################################################################################################
TMP_AT_SOCKET_FILE := $(filter-out $(SRCS_AT_SOCKET_C_RAM), $(SRCS_AT_SOCKET_C_FLASH))
TMP_AT_SOCKET_FILE := $(filter-out $(SRCS_AT_SOCKET_EXCLUDE_LIB_RAM), $(TMP_AT_SOCKET_FILE))
TMP_AT_SOCKET_FILE := $(filter-out $(SRCS_AT_SOCKET_EXCLUDE_LIB_FLASH), $(TMP_AT_SOCKET_FILE))
TMP_AT_SOCKET_FILE := $(filter-out $(SRCS_AT_SOCKET_EXCLUDE_PRJ), $(TMP_AT_SOCKET_FILE))

SRCS_AT_SOCKET_RAM   += $(SRCS_AT_SOCKET_C_RAM)
SRCS_AT_SOCKET_FLASH += $(TMP_AT_SOCKET_FILE)

AT_SOCKET_EXCLUDE_LIB_RAM   += $(SRCS_AT_SOCKET_EXCLUDE_LIB_RAM)
AT_SOCKET_EXCLUDE_LIB_FLASH += $(SRCS_AT_SOCKET_EXCLUDE_LIB_FLASH)


####################################################################################################
# include path
####################################################################################################
DEPS_PATH_AT_SOCKET += $(AT_SOCKET_SRC_DIR)/inc


####################################################################################################
# add to sysapp.mk
####################################################################################################
SRCS_APPLIB_RAM   += $(SRCS_AT_SOCKET_RAM)
SRCS_APPLIB_FLASH += $(SRCS_AT_SOCKET_FLASH)

APPLIB_EXCLUDE_LIB_RAM   += $(AT_SOCKET_EXCLUDE_LIB_RAM)
APPLIB_EXCLUDE_LIB_FLASH += $(AT_SOCKET_EXCLUDE_LIB_FLASH)

DEPS_PATH_APPLIB += $(DEPS_PATH_AT_SOCKET)