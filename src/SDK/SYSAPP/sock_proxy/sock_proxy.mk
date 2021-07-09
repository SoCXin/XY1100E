####################################################################################################
# directory
####################################################################################################
SOCKPROXY_SRC_DIR := $(SYSAPP_SRC_DIR)/sock_proxy


####################################################################################################
# SOCKPROXY source file
####################################################################################################
SRCS_SOCKPROXY_C_RAM = \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


SRCS_SOCKPROXY_C_FLASH = \
	$(wildcard $(SOCKPROXY_SRC_DIR)/src/*.c) \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


SRCS_SOCKPROXY_EXCLUDE_LIB_RAM = \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


SRCS_SOCKPROXY_EXCLUDE_LIB_FLASH = \

#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


SRCS_SOCKPROXY_EXCLUDE_PRJ = \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


####################################################################################################
# source
####################################################################################################
SOCKPROXY_BASE_FILE_GROUP  = $(SRCS_SOCKPROXY_EXCLUDE_LIB_FLASH)
SOCKPROXY_OTHER_FILE_GROUP = $(SRCS_SOCKPROXY_C_RAM) $(SRCS_SOCKPROXY_C_FLASH) $(SRCS_SOCKPROXY_EXCLUDE_LIB_RAM)

TMP_SOCKPROXY_FILE = $(filter-out $(SOCKPROXY_OTHER_FILE_GROUP), $(SOCKPROXY_BASE_FILE_GROUP))

SRCS_SOCKPROXY_RAM          += $(filter-out $(SRCS_SOCKPROXY_EXCLUDE_PRJ), $(SRCS_SOCKPROXY_C_RAM))
SRCS_SOCKPROXY_FLASH        += $(filter-out $(SRCS_SOCKPROXY_EXCLUDE_PRJ), $(SRCS_SOCKPROXY_C_FLASH))
SOCKPROXY_EXCLUDE_LIB_RAM   += $(filter-out $(SRCS_SOCKPROXY_EXCLUDE_PRJ), $(SRCS_SOCKPROXY_EXCLUDE_LIB_RAM))
SOCKPROXY_EXCLUDE_LIB_FLASH += $(filter-out $(SRCS_SOCKPROXY_EXCLUDE_PRJ), $(TMP_SOCKPROXY_FILE))


####################################################################################################
# include path
####################################################################################################
DEPS_PATH_SOCKPROXY += $(SOCKPROXY_SRC_DIR)/inc


####################################################################################################
# add to sysapp.mk
####################################################################################################
SRCS_SYSAPP_RAM   += $(SRCS_SOCKPROXY_RAM)
SRCS_SYSAPP_FLASH += $(SRCS_SOCKPROXY_FLASH)

SYSAPP_EXCLUDE_LIB_RAM   += $(SOCKPROXY_EXCLUDE_LIB_RAM)
SYSAPP_EXCLUDE_LIB_FLASH += $(SOCKPROXY_EXCLUDE_LIB_FLASH)

DEPS_PATH_SYSAPP += $(DEPS_PATH_SOCKPROXY)