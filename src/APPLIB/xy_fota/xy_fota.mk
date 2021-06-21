####################################################################################################
# directory
####################################################################################################
XY_FOTA_SRC_DIR := $(APPLIB_SRC_DIR)/xy_fota


####################################################################################################
# source file
####################################################################################################
SRCS_XY_FOTA_C_RAM = \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \

ifeq ($(XY_FOTA_SUPPORT),n)
SRCS_XY_FOTA_C_FLASH+=$(XY_FOTA_SRC_DIR)/src/xy_fota_utils.c
endif
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


SRCS_XY_FOTA_EXCLUDE_LIB_RAM = \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \




ifeq ($(XY_FOTA_SUPPORT),y)
SRCS_XY_FOTA_EXCLUDE_LIB_FLASH = \
	$(wildcard $(XY_FOTA_SRC_DIR)/src/*.c)  
endif
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


SRCS_XY_FOTA_EXCLUDE_PRJ = \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


####################################################################################################
# source
####################################################################################################
TMP_XY_FOTA_FILE := $(filter-out $(SRCS_XY_FOTA_C_RAM), $(SRCS_XY_FOTA_EXCLUDE_LIB_FLASH))
TMP_XY_FOTA_FILE := $(filter-out $(SRCS_XY_FOTA_C_FLASH), $(TMP_XY_FOTA_FILE))
TMP_XY_FOTA_FILE := $(filter-out $(SRCS_XY_FOTA_EXCLUDE_LIB_RAM), $(TMP_XY_FOTA_FILE))
TMP_XY_FOTA_FILE := $(filter-out $(SRCS_XY_FOTA_EXCLUDE_PRJ), $(TMP_XY_FOTA_FILE))

SRCS_XY_FOTA_RAM   += $(SRCS_XY_FOTA_C_RAM)
SRCS_XY_FOTA_FLASH += $(SRCS_XY_FOTA_C_FLASH)

XY_FOTA_EXCLUDE_LIB_RAM   += $(SRCS_XY_FOTA_EXCLUDE_LIB_RAM)
XY_FOTA_EXCLUDE_LIB_FLASH += $(TMP_XY_FOTA_FILE)


####################################################################################################
# include path
####################################################################################################
DEPS_PATH_XY_FOTA += $(XY_FOTA_SRC_DIR)/inc

####################################################################################################
# add to sysapp.mk
####################################################################################################
SRCS_APPLIB_RAM   += $(SRCS_XY_FOTA_RAM)
SRCS_APPLIB_FLASH += $(SRCS_XY_FOTA_FLASH)

APPLIB_EXCLUDE_LIB_RAM   += $(XY_FOTA_EXCLUDE_LIB_RAM)
APPLIB_EXCLUDE_LIB_FLASH += $(XY_FOTA_EXCLUDE_LIB_FLASH)

DEPS_PATH_APPLIB += $(DEPS_PATH_XY_FOTA)