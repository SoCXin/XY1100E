####################################################################################################
# directory
####################################################################################################
WAKAAMA_SRC_DIR := $(APPLIB_SRC_DIR)/wakaama


####################################################################################################
# source file
####################################################################################################
SRCS_WAKAAMA_C_RAM = \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


SRCS_WAKAAMA_C_FLASH = \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


SRCS_WAKAAMA_EXCLUDE_LIB_RAM = \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


SRCS_WAKAAMA_EXCLUDE_LIB_FLASH = \
	$(wildcard $(WAKAAMA_SRC_DIR)/core/er-coap-13/*.c) \
	$(wildcard $(WAKAAMA_SRC_DIR)/core/*.c) \
	$(wildcard $(WAKAAMA_SRC_DIR)/examples/lightclient/*.c) \
	$(wildcard $(WAKAAMA_SRC_DIR)/examples/shared/*.c) \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


SRCS_WAKAAMA_EXCLUDE_PRJ = \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


####################################################################################################
# source
####################################################################################################
TMP_WAKAAMA_FILE := $(filter-out $(SRCS_WAKAAMA_C_RAM), $(SRCS_WAKAAMA_EXCLUDE_LIB_FLASH))
TMP_WAKAAMA_FILE := $(filter-out $(SRCS_WAKAAMA_C_FLASH), $(TMP_WAKAAMA_FILE))
TMP_WAKAAMA_FILE := $(filter-out $(SRCS_WAKAAMA_EXCLUDE_LIB_RAM), $(TMP_WAKAAMA_FILE))
TMP_WAKAAMA_FILE := $(filter-out $(SRCS_WAKAAMA_EXCLUDE_PRJ), $(TMP_WAKAAMA_FILE))

SRCS_WAKAAMA_RAM   += $(SRCS_WAKAAMA_C_RAM)
SRCS_WAKAAMA_FLASH += $(SRCS_WAKAAMA_C_FLASH)

WAKAAMA_EXCLUDE_LIB_RAM   += $(SRCS_WAKAAMA_EXCLUDE_LIB_RAM)
WAKAAMA_EXCLUDE_LIB_FLASH += $(TMP_WAKAAMA_FILE)


####################################################################################################
# include path
####################################################################################################
DEPS_PATH_WAKAAMA += $(WAKAAMA_SRC_DIR)/core/er-coap-13
DEPS_PATH_WAKAAMA += $(WAKAAMA_SRC_DIR)/core
DEPS_PATH_WAKAAMA += $(WAKAAMA_SRC_DIR)/src
DEPS_PATH_WAKAAMA += $(WAKAAMA_SRC_DIR)/examples/shared


####################################################################################################
# add to sysapp.mk
####################################################################################################
SRCS_APPLIB_RAM   += $(SRCS_WAKAAMA_RAM)
SRCS_APPLIB_FLASH += $(SRCS_WAKAAMA_FLASH)

APPLIB_EXCLUDE_LIB_RAM   += $(WAKAAMA_EXCLUDE_LIB_RAM)
APPLIB_EXCLUDE_LIB_FLASH += $(WAKAAMA_EXCLUDE_LIB_FLASH)

DEPS_PATH_APPLIB += $(DEPS_PATH_WAKAAMA)