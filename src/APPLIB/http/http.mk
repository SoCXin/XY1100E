####################################################################################################
# directory
####################################################################################################
HTTP_SRC_DIR := $(APPLIB_SRC_DIR)/http


####################################################################################################
# source file
####################################################################################################
SRCS_HTTP_C_RAM = \

SRCS_HTTP_C_FLASH = \
	$(wildcard $(HTTP_SRC_DIR)/*.c) \
	$(wildcard $(HTTP_SRC_DIR)/atcmd/*.c) \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


SRCS_HTTP_EXCLUDE_LIB_RAM = \
	
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


SRCS_HTTP_EXCLUDE_LIB_FLASH = \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


SRCS_HTTP_EXCLUDE_PRJ = \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


####################################################################################################
# source
####################################################################################################
TMP_HTTP_FILE := $(filter-out $(SRCS_HTTP_C_RAM), $(SRCS_HTTP_EXCLUDE_LIB_FLASH))
TMP_HTTP_FILE := $(filter-out $(SRCS_HTTP_C_FLASH), $(TMP_HTTP_FILE))
TMP_HTTP_FILE := $(filter-out $(SRCS_HTTP_EXCLUDE_LIB_RAM), $(TMP_HTTP_FILE))
TMP_HTTP_FILE := $(filter-out $(SRCS_HTTP_EXCLUDE_PRJ), $(TMP_HTTP_FILE))

SRCS_HTTP_RAM   += $(SRCS_HTTP_C_RAM)
SRCS_HTTP_FLASH += $(SRCS_HTTP_C_FLASH)

HTTP_EXCLUDE_LIB_RAM   += $(SRCS_HTTP_EXCLUDE_LIB_RAM)
HTTP_EXCLUDE_LIB_FLASH += $(TMP_HTTP_FILE)


####################################################################################################
# include path
####################################################################################################
DEPS_PATH_HTTP += $(HTTP_SRC_DIR)
DEPS_PATH_HTTP += $(HTTP_SRC_DIR)/atcmd


####################################################################################################
# add to sysapp.mk
####################################################################################################
SRCS_APPLIB_RAM   += $(SRCS_HTTP_RAM)
SRCS_APPLIB_FLASH += $(SRCS_HTTP_FLASH)

APPLIB_EXCLUDE_LIB_RAM   += $(HTTP_EXCLUDE_LIB_RAM)
APPLIB_EXCLUDE_LIB_FLASH += $(HTTP_EXCLUDE_LIB_FLASH)

DEPS_PATH_APPLIB += $(DEPS_PATH_HTTP)