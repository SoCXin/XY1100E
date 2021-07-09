####################################################################################################
# directory
####################################################################################################
LIBCOAP_SRC_DIR := $(APPLIB_SRC_DIR)/libcoap


####################################################################################################
# source file
####################################################################################################
SRCS_LIBCOAP_C_RAM = \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


SRCS_LIBCOAP_C_FLASH = \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


SRCS_LIBCOAP_EXCLUDE_LIB_RAM = \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


SRCS_LIBCOAP_EXCLUDE_LIB_FLASH = \
	$(wildcard $(LIBCOAP_SRC_DIR)/CoapPacket/src/*.c) \
	$(wildcard $(LIBCOAP_SRC_DIR)/xy_coap/src/*.c) \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


SRCS_LIBCOAP_EXCLUDE_PRJ = \
	$(LIBCOAP_SRC_DIR)/xy_coap/src/xy_fota_by_coap.c \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


####################################################################################################
# source
####################################################################################################
TMP_LIBCOAP_FILE := $(filter-out $(SRCS_LIBCOAP_C_RAM), $(SRCS_LIBCOAP_EXCLUDE_LIB_FLASH))
TMP_LIBCOAP_FILE := $(filter-out $(SRCS_LIBCOAP_C_FLASH), $(TMP_LIBCOAP_FILE))
TMP_LIBCOAP_FILE := $(filter-out $(SRCS_LIBCOAP_EXCLUDE_LIB_RAM), $(TMP_LIBCOAP_FILE))
TMP_LIBCOAP_FILE := $(filter-out $(SRCS_LIBCOAP_EXCLUDE_PRJ), $(TMP_LIBCOAP_FILE))

SRCS_LIBCOAP_RAM   += $(SRCS_LIBCOAP_C_RAM)
SRCS_LIBCOAP_FLASH += $(SRCS_LIBCOAP_C_FLASH)

LIBCOAP_EXCLUDE_LIB_RAM   += $(SRCS_LIBCOAP_EXCLUDE_LIB_RAM)
LIBCOAP_EXCLUDE_LIB_FLASH += $(TMP_LIBCOAP_FILE)


####################################################################################################
# include path
####################################################################################################
DEPS_PATH_LIBCOAP += $(LIBCOAP_SRC_DIR)/CoapPacket/include/coap2
DEPS_PATH_LIBCOAP += $(LIBCOAP_SRC_DIR)/xy_coap/inc


####################################################################################################
# add to sysapp.mk
####################################################################################################
SRCS_APPLIB_RAM   += $(SRCS_LIBCOAP_RAM)
SRCS_APPLIB_FLASH += $(SRCS_LIBCOAP_FLASH)

APPLIB_EXCLUDE_LIB_RAM   += $(LIBCOAP_EXCLUDE_LIB_RAM)
APPLIB_EXCLUDE_LIB_FLASH += $(LIBCOAP_EXCLUDE_LIB_FLASH)

DEPS_PATH_APPLIB += $(DEPS_PATH_LIBCOAP)