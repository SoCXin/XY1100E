####################################################################################################
# directory
####################################################################################################
ONENET_SRC_DIR := $(CLOUD_SRC_DIR)/onenet


####################################################################################################
# source file
####################################################################################################
SRCS_ONENET_C_RAM = \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


SRCS_ONENET_C_FLASH = \
	$(wildcard $(ONENET_SRC_DIR)/ciscore/cis_er-coap-13/*.c) \
	$(wildcard $(ONENET_SRC_DIR)/ciscore/dtls/*.c) \
	$(wildcard $(ONENET_SRC_DIR)/ciscore/std_object/*.c) \
	$(wildcard $(ONENET_SRC_DIR)/ciscore/*.c) \
	$(wildcard $(ONENET_SRC_DIR)/xy_onenet/src/*.c) \
	
ifeq ($(DM_SUPPORT),y)	
SRCS_ONENET_C_FLASH += \
	$(wildcard $(ONENET_SRC_DIR)/ciscore/dm_utils/*.c) \
	$(wildcard $(ONENET_SRC_DIR)/DM/src/*.c) 
endif
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


SRCS_ONENET_EXCLUDE_LIB_RAM = \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


SRCS_ONENET_EXCLUDE_LIB_FLASH = \

#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


SRCS_ONENET_EXCLUDE_PRJ = \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


####################################################################################################
# source
####################################################################################################
TMP_ONENET_FILE := $(filter-out $(SRCS_ONENET_C_RAM), $(SRCS_ONENET_EXCLUDE_LIB_FLASH))
TMP_ONENET_FILE := $(filter-out $(SRCS_ONENET_C_FLASH), $(TMP_ONENET_FILE))
TMP_ONENET_FILE := $(filter-out $(SRCS_ONENET_EXCLUDE_LIB_RAM), $(TMP_ONENET_FILE))
TMP_ONENET_FILE := $(filter-out $(SRCS_ONENET_EXCLUDE_PRJ), $(TMP_ONENET_FILE))

SRCS_ONENET_RAM   += $(SRCS_ONENET_C_RAM)
SRCS_ONENET_FLASH += $(SRCS_ONENET_C_FLASH)

ONENET_EXCLUDE_LIB_RAM   += $(SRCS_ONENET_EXCLUDE_LIB_RAM)
ONENET_EXCLUDE_LIB_FLASH += $(TMP_ONENET_FILE)


####################################################################################################
# include path
####################################################################################################
DEPS_PATH_ONENET += $(ONENET_SRC_DIR)/ciscore/cis_er-coap-13
DEPS_PATH_ONENET += $(ONENET_SRC_DIR)/ciscore/dm_utils
DEPS_PATH_ONENET += $(ONENET_SRC_DIR)/ciscore/dtls
DEPS_PATH_ONENET += $(ONENET_SRC_DIR)/ciscore/std_object
DEPS_PATH_ONENET += $(ONENET_SRC_DIR)/ciscore
DEPS_PATH_ONENET += $(ONENET_SRC_DIR)/xy_onenet/inc

ifeq ($(DM_SUPPORT),y)
DEPS_PATH_ONENET += $(ONENET_SRC_DIR)/DM/inc
DEPS_PATH_ONENET += $(ONENET_SRC_DIR)/ciscore/dm_utils
endif

####################################################################################################
# add to applib.mk
####################################################################################################
SRCS_APPLIB_RAM   += $(SRCS_ONENET_RAM)
SRCS_APPLIB_FLASH += $(SRCS_ONENET_FLASH)

APPLIB_EXCLUDE_LIB_RAM   += $(ONENET_EXCLUDE_LIB_RAM)
APPLIB_EXCLUDE_LIB_FLASH += $(ONENET_EXCLUDE_LIB_FLASH)

DEPS_PATH_APPLIB += $(DEPS_PATH_ONENET)