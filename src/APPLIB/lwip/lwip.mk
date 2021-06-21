####################################################################################################
# directory
####################################################################################################
LWIP_SRC_DIR := $(APPLIB_SRC_DIR)/lwip


####################################################################################################
# include other makefile to include source
####################################################################################################
-include $(LWIP_SRC_DIR)/xy_adapter/xy_adapter.mk

####################################################################################################
# source file
####################################################################################################
SRCS_LWIP_C_RAM = \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


SRCS_LWIP_C_FLASH = \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


SRCS_LWIP_EXCLUDE_LIB_RAM = \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


SRCS_LWIP_EXCLUDE_LIB_FLASH = \
	$(wildcard $(LWIP_SRC_DIR)/lwip_port/os/*.c) \
	$(wildcard $(LWIP_SRC_DIR)/lwip-2.1.2/api/*.c) \
	$(wildcard $(LWIP_SRC_DIR)/lwip-2.1.2/apps/sntp/*.c) \
	$(wildcard $(LWIP_SRC_DIR)/lwip-2.1.2/core/*.c) \
	$(wildcard $(LWIP_SRC_DIR)/lwip-2.1.2/core/ipv4/*.c) \
	$(wildcard $(LWIP_SRC_DIR)/lwip-2.1.2/core/ipv6/*.c) \
	$(wildcard $(LWIP_SRC_DIR)/lwip-2.1.2/netif/*.c) \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


SRCS_LWIP_EXCLUDE_PRJ = \
	$(LWIP_SRC_DIR)/lwip-2.1.2/apps/sntp \
	$(LWIP_SRC_DIR)/lwip-2.1.2/netif/bridgeif.c \
	$(LWIP_SRC_DIR)/lwip-2.1.2/netif/bridgeif_fdb.c \
	$(LWIP_SRC_DIR)/lwip-2.1.2/netif/lowpan6.c \
	$(LWIP_SRC_DIR)/lwip-2.1.2/netif/lowpan6_ble.c \
	$(LWIP_SRC_DIR)/lwip-2.1.2/netif/lowpan6_common.c \
	$(LWIP_SRC_DIR)/lwip-2.1.2/netif/slipif.c \
	$(LWIP_SRC_DIR)/lwip-2.1.2/netif/zepif.c \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


####################################################################################################
# source
####################################################################################################
TMP_LWIP_FILE := $(filter-out $(SRCS_LWIP_C_RAM), $(SRCS_LWIP_EXCLUDE_LIB_FLASH))
TMP_LWIP_FILE := $(filter-out $(SRCS_LWIP_C_FLASH), $(TMP_LWIP_FILE))
TMP_LWIP_FILE := $(filter-out $(SRCS_LWIP_EXCLUDE_LIB_RAM), $(TMP_LWIP_FILE))
TMP_LWIP_FILE := $(filter-out $(SRCS_LWIP_EXCLUDE_PRJ), $(TMP_LWIP_FILE))

SRCS_LWIP_RAM   += $(SRCS_LWIP_C_RAM)
SRCS_LWIP_FLASH += $(SRCS_LWIP_C_FLASH)

LWIP_EXCLUDE_LIB_RAM   += $(SRCS_LWIP_EXCLUDE_LIB_RAM)
LWIP_EXCLUDE_LIB_FLASH += $(TMP_LWIP_FILE)


####################################################################################################
# include path
####################################################################################################
DEPS_PATH_LWIP += $(LWIP_SRC_DIR)/lwip_config
DEPS_PATH_LWIP += $(LWIP_SRC_DIR)/lwip_port
DEPS_PATH_LWIP += $(LWIP_SRC_DIR)/lwip_port/arch
DEPS_PATH_LWIP += $(LWIP_SRC_DIR)/lwip_port/os
DEPS_PATH_LWIP += $(LWIP_SRC_DIR)/lwip-2.1.2/include


####################################################################################################
# add to sysapp.mk
####################################################################################################
SRCS_APPLIB_RAM   += $(SRCS_LWIP_RAM)
SRCS_APPLIB_FLASH += $(SRCS_LWIP_FLASH)

APPLIB_EXCLUDE_LIB_RAM   += $(LWIP_EXCLUDE_LIB_RAM)
APPLIB_EXCLUDE_LIB_FLASH += $(LWIP_EXCLUDE_LIB_FLASH)

DEPS_PATH_APPLIB += $(DEPS_PATH_LWIP)