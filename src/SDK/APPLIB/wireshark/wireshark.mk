####################################################################################################
# directory
####################################################################################################
WIRESHARK_SRC_DIR := $(APPLIB_SRC_DIR)/wireshark


####################################################################################################
# source file
####################################################################################################
SRCS_WIRESHARK_C_RAM = \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


SRCS_WIRESHARK_C_FLASH = \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


SRCS_WIRESHARK_EXCLUDE_LIB_RAM = \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


SRCS_WIRESHARK_EXCLUDE_LIB_FLASH = \
	$(wildcard $(WIRESHARK_SRC_DIR)/xy_wireshark/src/*.c) \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


SRCS_WIRESHARK_EXCLUDE_PRJ = \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


####################################################################################################
# source
####################################################################################################
TMP_WIRESHARK_FILE := $(filter-out $(SRCS_WIRESHARK_C_RAM), $(SRCS_WIRESHARK_EXCLUDE_LIB_FLASH))
TMP_WIRESHARK_FILE := $(filter-out $(SRCS_WIRESHARK_C_FLASH), $(TMP_WIRESHARK_FILE))
TMP_WIRESHARK_FILE := $(filter-out $(SRCS_WIRESHARK_EXCLUDE_LIB_RAM), $(TMP_WIRESHARK_FILE))
TMP_WIRESHARK_FILE := $(filter-out $(SRCS_WIRESHARK_EXCLUDE_PRJ), $(TMP_WIRESHARK_FILE))

SRCS_WIRESHARK_RAM   += $(SRCS_WIRESHARK_C_RAM)
SRCS_WIRESHARK_FLASH += $(SRCS_WIRESHARK_C_FLASH)

WIRESHARK_EXCLUDE_LIB_RAM   += $(SRCS_WIRESHARK_EXCLUDE_LIB_RAM)
WIRESHARK_EXCLUDE_LIB_FLASH += $(TMP_WIRESHARK_FILE)


####################################################################################################
# include path
####################################################################################################
DEPS_PATH_WIRESHARK += $(WIRESHARK_SRC_DIR)/xy_wireshark/inc

####################################################################################################
# add to sysapp.mk
####################################################################################################
SRCS_APPLIB_RAM   += $(SRCS_WIRESHARK_RAM)
SRCS_APPLIB_FLASH += $(SRCS_WIRESHARK_FLASH)

APPLIB_EXCLUDE_LIB_RAM   += $(WIRESHARK_EXCLUDE_LIB_RAM)
APPLIB_EXCLUDE_LIB_FLASH += $(WIRESHARK_EXCLUDE_LIB_FLASH)

DEPS_PATH_APPLIB += $(DEPS_PATH_WIRESHARK)