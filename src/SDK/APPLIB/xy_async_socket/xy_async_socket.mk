####################################################################################################
# directory
####################################################################################################
XY_ASYNC_ASYNC_SOCKET_API_SRC_DIR := $(APPLIB_SRC_DIR)/xy_async_socket


####################################################################################################
# source file
####################################################################################################
SRCS_XY_ASYNC_ASYNC_SOCKET_API_C_RAM = 
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


SRCS_XY_ASYNC_ASYNC_SOCKET_API_C_FLASH = \
	$(wildcard $(XY_ASYNC_ASYNC_SOCKET_API_SRC_DIR)/src/*.c) \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


SRCS_XY_ASYNC_ASYNC_SOCKET_API_EXCLUDE_LIB_RAM = \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


SRCS_XY_ASYNC_ASYNC_SOCKET_API_EXCLUDE_LIB_FLASH = \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


SRCS_XY_ASYNC_ASYNC_SOCKET_API_EXCLUDE_LIB_FLASH = \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


####################################################################################################
# source
####################################################################################################
TMP_XY_ASYNC_ASYNC_SOCKET_API_FILE := $(filter-out $(SRCS_XY_ASYNC_ASYNC_SOCKET_API_C_RAM), $(SRCS_XY_ASYNC_ASYNC_SOCKET_API_C_FLASH))
TMP_XY_ASYNC_ASYNC_SOCKET_API_FILE := $(filter-out $(SRCS_XY_ASYNC_ASYNC_SOCKET_API_EXCLUDE_LIB_RAM), $(TMP_XY_ASYNC_ASYNC_SOCKET_API_FILE))
TMP_XY_ASYNC_ASYNC_SOCKET_API_FILE := $(filter-out $(SRCS_XY_ASYNC_ASYNC_SOCKET_API_EXCLUDE_LIB_FLASH), $(TMP_XY_ASYNC_ASYNC_SOCKET_API_FILE))
TMP_XY_ASYNC_ASYNC_SOCKET_API_FILE := $(filter-out $(SRCS_XY_ASYNC_ASYNC_SOCKET_API_EXCLUDE_LIB_FLASH), $(TMP_XY_ASYNC_ASYNC_SOCKET_API_FILE))

SRCS_XY_ASYNC_ASYNC_SOCKET_API_RAM   += $(SRCS_XY_ASYNC_ASYNC_SOCKET_API_C_RAM)
SRCS_XY_ASYNC_ASYNC_SOCKET_API_FLASH += $(TMP_XY_ASYNC_ASYNC_SOCKET_API_FILE)

XY_ASYNC_ASYNC_SOCKET_API_EXCLUDE_LIB_RAM   += $(SRCS_XY_ASYNC_ASYNC_SOCKET_API_EXCLUDE_LIB_RAM)
XY_ASYNC_ASYNC_SOCKET_API_EXCLUDE_LIB_FLASH += $(SRCS_XY_ASYNC_ASYNC_SOCKET_API_EXCLUDE_LIB_FLASH)


####################################################################################################
# include path
####################################################################################################
DEPS_PATH_XY_ASYNC_ASYNC_SOCKET_API += $(XY_ASYNC_ASYNC_SOCKET_API_SRC_DIR)/inc


####################################################################################################
# add to sysapp.mk
####################################################################################################
SRCS_APPLIB_RAM   += $(SRCS_XY_ASYNC_ASYNC_SOCKET_API_RAM)
SRCS_APPLIB_FLASH += $(SRCS_XY_ASYNC_ASYNC_SOCKET_API_FLASH)

APPLIB_EXCLUDE_LIB_RAM   += $(XY_ASYNC_ASYNC_SOCKET_API_EXCLUDE_LIB_RAM)
APPLIB_EXCLUDE_LIB_FLASH += $(XY_ASYNC_ASYNC_SOCKET_API_EXCLUDE_LIB_FLASH)

DEPS_PATH_APPLIB += $(DEPS_PATH_XY_ASYNC_ASYNC_SOCKET_API)