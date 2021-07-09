####################################################################################################
# directory
####################################################################################################
TELECOM_SRC_DIR := $(CLOUD_SRC_DIR)/cdp


####################################################################################################
# source file
####################################################################################################
SRCS_TELECOM_C_RAM = \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


SRCS_TELECOM_C_FLASH = \
	$(wildcard $(TELECOM_SRC_DIR)/AgentTiny/atiny_lwm2m/*.c) \
	$(wildcard $(TELECOM_SRC_DIR)/AgentTiny/log/*.c) \
	$(wildcard $(TELECOM_SRC_DIR)/AgentTiny/lwm2m/core/er-coap-13/*.c) \
	$(wildcard $(TELECOM_SRC_DIR)/AgentTiny/lwm2m/core/*.c) \
	$(wildcard $(TELECOM_SRC_DIR)/AgentTiny/lwm2m/examples/shared/*.c) \
	$(wildcard $(TELECOM_SRC_DIR)/AgentTiny/net/*.c) \
	$(wildcard $(TELECOM_SRC_DIR)/AgentTiny/osdepends/*.c) \
	$(wildcard $(TELECOM_SRC_DIR)/AgentTiny/ota/flag_operate/*.c) \
	$(wildcard $(TELECOM_SRC_DIR)/AgentTiny/ota/sota/*.c) \
	$(wildcard $(TELECOM_SRC_DIR)/AgentTiny/ota/fota/*.c) \
	$(wildcard $(TELECOM_SRC_DIR)/AgentTiny/ota/package/*.c) \
	$(wildcard $(TELECOM_SRC_DIR)/AgentTiny/ota/utility/*.c) \
	$(wildcard $(TELECOM_SRC_DIR)/xy_cdp/src/*.c) \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


SRCS_TELECOM_EXCLUDE_LIB_RAM = \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


SRCS_TELECOM_EXCLUDE_LIB_FLASH = \

#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


SRCS_TELECOM_EXCLUDE_PRJ = \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


####################################################################################################
# source
####################################################################################################
TMP_TELECOM_FILE := $(filter-out $(SRCS_TELECOM_C_RAM), $(SRCS_TELECOM_EXCLUDE_LIB_FLASH))
TMP_TELECOM_FILE := $(filter-out $(SRCS_TELECOM_C_FLASH), $(TMP_TELECOM_FILE))
TMP_TELECOM_FILE := $(filter-out $(SRCS_TELECOM_EXCLUDE_LIB_RAM), $(TMP_TELECOM_FILE))
TMP_TELECOM_FILE := $(filter-out $(SRCS_TELECOM_EXCLUDE_PRJ), $(TMP_TELECOM_FILE))

SRCS_TELECOM_RAM   += $(SRCS_TELECOM_C_RAM)
SRCS_TELECOM_FLASH += $(SRCS_TELECOM_C_FLASH)

TELECOM_EXCLUDE_LIB_RAM   += $(SRCS_TELECOM_EXCLUDE_LIB_RAM)
TELECOM_EXCLUDE_LIB_FLASH += $(TMP_TELECOM_FILE)


####################################################################################################
# include path
####################################################################################################
DEPS_PATH_TELECOM += $(TELECOM_SRC_DIR)/AgentTiny/atiny_lwm2m
DEPS_PATH_TELECOM += $(TELECOM_SRC_DIR)/AgentTiny/include
DEPS_PATH_TELECOM += $(TELECOM_SRC_DIR)/AgentTiny/log
DEPS_PATH_TELECOM += $(TELECOM_SRC_DIR)/AgentTiny/osdepends
DEPS_PATH_TELECOM += $(TELECOM_SRC_DIR)/AgentTiny/net
DEPS_PATH_TELECOM += $(TELECOM_SRC_DIR)/AgentTiny/lwm2m/core/er-cope-13
DEPS_PATH_TELECOM += $(TELECOM_SRC_DIR)/AgentTiny/lwm2m/core
DEPS_PATH_TELECOM += $(TELECOM_SRC_DIR)/AgentTiny/lwm2m/examples/shared
DEPS_PATH_TELECOM += $(TELECOM_SRC_DIR)/AgentTiny/ota/flag_operate
DEPS_PATH_TELECOM += $(TELECOM_SRC_DIR)/AgentTiny/ota/package/opt
DEPS_PATH_TELECOM += $(TELECOM_SRC_DIR)/AgentTiny/ota/package
DEPS_PATH_TELECOM += $(TELECOM_SRC_DIR)/AgentTiny/ota/fota
DEPS_PATH_TELECOM += $(TELECOM_SRC_DIR)/AgentTiny/ota/sota
DEPS_PATH_TELECOM += $(TELECOM_SRC_DIR)/AgentTiny/ota/utility
DEPS_PATH_TELECOM += $(TELECOM_SRC_DIR)/xy_cdp/inc

####################################################################################################
# add to sysapp.mk
####################################################################################################
SRCS_APPLIB_RAM   += $(SRCS_TELECOM_RAM)
SRCS_APPLIB_FLASH += $(SRCS_TELECOM_FLASH)

APPLIB_EXCLUDE_LIB_RAM   += $(TELECOM_EXCLUDE_LIB_RAM)
APPLIB_EXCLUDE_LIB_FLASH += $(TELECOM_EXCLUDE_LIB_FLASH)

DEPS_PATH_APPLIB += $(DEPS_PATH_TELECOM)