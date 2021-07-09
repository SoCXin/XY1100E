####################################################################################################
# directory
####################################################################################################
MQTT_SRC_DIR := $(APPLIB_SRC_DIR)/mqtt


####################################################################################################
# source file
####################################################################################################
SRCS_MQTT_C_RAM = \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


SRCS_MQTT_C_FLASH = \
	$(wildcard $(MQTT_SRC_DIR)/MQTTPacket/src/*.c) \
	$(wildcard $(MQTT_SRC_DIR)/xy_mqtt/src/*.c) \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


SRCS_MQTT_EXCLUDE_LIB_RAM = \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


SRCS_MQTT_EXCLUDE_LIB_FLASH = \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


SRCS_MQTT_EXCLUDE_PRJ = \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


####################################################################################################
# source
####################################################################################################
TMP_MQTT_FILE := $(filter-out $(SRCS_MQTT_C_RAM), $(SRCS_MQTT_C_FLASH))
TMP_MQTT_FILE := $(filter-out $(SRCS_MQTT_EXCLUDE_LIB_FLASH), $(TMP_MQTT_FILE))
TMP_MQTT_FILE := $(filter-out $(SRCS_MQTT_EXCLUDE_LIB_RAM), $(TMP_MQTT_FILE))
TMP_MQTT_FILE := $(filter-out $(SRCS_MQTT_EXCLUDE_PRJ), $(TMP_MQTT_FILE))

SRCS_MQTT_RAM   += $(SRCS_MQTT_C_RAM)
SRCS_MQTT_FLASH += $(TMP_MQTT_FILE)

MQTT_EXCLUDE_LIB_RAM   += $(SRCS_MQTT_EXCLUDE_LIB_RAM)
MQTT_EXCLUDE_LIB_FLASH += $(SRCS_MQTT_EXCLUDE_LIB_FLASH)


####################################################################################################
# include path
####################################################################################################
DEPS_PATH_MQTT += $(MQTT_SRC_DIR)/MQTTPacket/src
DEPS_PATH_MQTT += $(MQTT_SRC_DIR)/xy_mqtt/inc


####################################################################################################
# add to sysapp.mk
####################################################################################################
SRCS_APPLIB_RAM   += $(SRCS_MQTT_RAM)
SRCS_APPLIB_FLASH += $(SRCS_MQTT_FLASH)

APPLIB_EXCLUDE_LIB_RAM   += $(MQTT_EXCLUDE_LIB_RAM)
APPLIB_EXCLUDE_LIB_FLASH += $(MQTT_EXCLUDE_LIB_FLASH)

DEPS_PATH_APPLIB += $(DEPS_PATH_MQTT)