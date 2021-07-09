####################################################################################################
# directory
####################################################################################################
XY_PS_SRC_DIR := $(APPLIB_SRC_DIR)/xy_ps


####################################################################################################
# source file
####################################################################################################
SRCS_XY_PS_C_RAM = 
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \

SRCS_XY_PS_C_FLASH = \
	$(wildcard $(XY_PS_SRC_DIR)/src/*.c) \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


SRCS_XY_PS_EXCLUDE_LIB_RAM = \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


SRCS_XY_PS_EXCLUDE_LIB_FLASH = \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


SRCS_XY_PS_EXCLUDE_PRJ = \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


####################################################################################################
# source
####################################################################################################
TMP_XY_PS_FILE := $(filter-out $(SRCS_XY_PS_C_RAM), $(SRCS_XY_PS_C_FLASH))
TMP_XY_PS_FILE := $(filter-out $(SRCS_XY_PS_EXCLUDE_LIB_RAM), $(TMP_XY_PS_FILE))
TMP_XY_PS_FILE := $(filter-out $(SRCS_XY_PS_EXCLUDE_LIB_FLASH), $(TMP_XY_PS_FILE))
TMP_XY_PS_FILE := $(filter-out $(SRCS_XY_PS_EXCLUDE_PRJ), $(TMP_XY_PS_FILE))

SRCS_XY_PS_RAM   += $(SRCS_XY_PS_C_RAM)
SRCS_XY_PS_FLASH += $(TMP_XY_PS_FILE)

XY_PS_EXCLUDE_LIB_RAM   += $(SRCS_XY_PS_EXCLUDE_LIB_RAM)
XY_PS_EXCLUDE_LIB_FLASH += $(SRCS_XY_PS_EXCLUDE_LIB_FLASH)


####################################################################################################
# include path
####################################################################################################
DEPS_PATH_XY_PS += $(XY_PS_SRC_DIR)/inc


####################################################################################################
# add to sysapp.mk
####################################################################################################
SRCS_APPLIB_RAM   += $(SRCS_XY_PS_RAM)
SRCS_APPLIB_FLASH += $(SRCS_XY_PS_FLASH)

APPLIB_EXCLUDE_LIB_RAM   += $(XY_PS_EXCLUDE_LIB_RAM)
APPLIB_EXCLUDE_LIB_FLASH += $(XY_PS_EXCLUDE_LIB_FLASH)

DEPS_PATH_APPLIB += $(DEPS_PATH_XY_PS)