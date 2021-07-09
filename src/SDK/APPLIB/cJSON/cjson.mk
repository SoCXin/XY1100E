####################################################################################################
# directory
####################################################################################################
CJSON_SRC_DIR := $(APPLIB_SRC_DIR)/cJSON


####################################################################################################
# source file
####################################################################################################
SRCS_CJSON_C_RAM = \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


SRCS_CJSON_C_FLASH = \
	$(wildcard $(CJSON_SRC_DIR)/*.c) \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


SRCS_CJSON_EXCLUDE_LIB_RAM = \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


SRCS_CJSON_EXCLUDE_LIB_FLASH = \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


SRCS_CJSON_EXCLUDE_PRJ = \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


####################################################################################################
# source
####################################################################################################
TMP_CJSON_FILE := $(filter-out $(SRCS_CJSON_C_RAM), $(SRCS_CJSON_EXCLUDE_LIB_FLASH))
TMP_CJSON_FILE := $(filter-out $(SRCS_CJSON_C_FLASH), $(TMP_CJSON_FILE))
TMP_CJSON_FILE := $(filter-out $(SRCS_CJSON_EXCLUDE_LIB_RAM), $(TMP_CJSON_FILE))
TMP_CJSON_FILE := $(filter-out $(SRCS_CJSON_EXCLUDE_PRJ), $(TMP_CJSON_FILE))

SRCS_CJSON_RAM   += $(SRCS_CJSON_C_RAM)
SRCS_CJSON_FLASH += $(SRCS_CJSON_C_FLASH)

CJSON_EXCLUDE_LIB_RAM   += $(SRCS_CJSON_EXCLUDE_LIB_RAM)
CJSON_EXCLUDE_LIB_FLASH += $(TMP_CJSON_FILE)


####################################################################################################
# include path
####################################################################################################
DEPS_PATH_CJSON += $(CJSON_SRC_DIR)


####################################################################################################
# add to sysapp.mk
####################################################################################################
SRCS_APPLIB_RAM   += $(SRCS_CJSON_RAM)
SRCS_APPLIB_FLASH += $(SRCS_CJSON_FLASH)

APPLIB_EXCLUDE_LIB_RAM   += $(CJSON_EXCLUDE_LIB_RAM)
APPLIB_EXCLUDE_LIB_FLASH += $(CJSON_EXCLUDE_LIB_FLASH)

DEPS_PATH_APPLIB += $(DEPS_PATH_CJSON)