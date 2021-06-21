####################################################################################################
# directory
####################################################################################################
ADAPTER_SRC_DIR := $(LWIP_SRC_DIR)/xy_adapter


####################################################################################################
# source file
####################################################################################################
SRCS_ADAPTER_C_RAM = \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


SRCS_ADAPTER_C_FLASH = \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


SRCS_ADAPTER_EXCLUDE_LIB_RAM = \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


SRCS_ADAPTER_EXCLUDE_LIB_FLASH = \
	$(wildcard $(ADAPTER_SRC_DIR)/src/*.c) \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


SRCS_ADAPTER_EXCLUDE_PRJ = \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


####################################################################################################
# source
####################################################################################################
TMP_ADAPTER_FILE := $(filter-out $(SRCS_ADAPTER_C_RAM), $(SRCS_ADAPTER_EXCLUDE_LIB_FLASH))
TMP_ADAPTER_FILE := $(filter-out $(SRCS_ADAPTER_C_FLASH), $(TMP_ADAPTER_FILE))
TMP_ADAPTER_FILE := $(filter-out $(SRCS_ADAPTER_EXCLUDE_LIB_RAM), $(TMP_ADAPTER_FILE))
TMP_ADAPTER_FILE := $(filter-out $(SRCS_ADAPTER_EXCLUDE_PRJ), $(TMP_ADAPTER_FILE))

SRCS_ADAPTER_RAM   += $(SRCS_ADAPTER_C_RAM)
SRCS_ADAPTER_FLASH += $(SRCS_ADAPTER_C_FLASH)

ADAPTER_EXCLUDE_LIB_RAM   += $(SRCS_ADAPTER_EXCLUDE_LIB_RAM)
ADAPTER_EXCLUDE_LIB_FLASH += $(TMP_ADAPTER_FILE)


####################################################################################################
# include path
####################################################################################################
DEPS_PATH_ADAPTER += $(ADAPTER_SRC_DIR)/inc


####################################################################################################
# add to sysapp.mk
####################################################################################################
SRCS_APPLIB_RAM   += $(SRCS_ADAPTER_RAM)
SRCS_APPLIB_FLASH += $(SRCS_ADAPTER_FLASH)

APPLIB_EXCLUDE_LIB_RAM   += $(ADAPTER_EXCLUDE_LIB_RAM)
APPLIB_EXCLUDE_LIB_FLASH += $(ADAPTER_EXCLUDE_LIB_FLASH)

DEPS_PATH_APPLIB += $(DEPS_PATH_ADAPTER)