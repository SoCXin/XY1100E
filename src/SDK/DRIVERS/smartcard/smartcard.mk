####################################################################################################
# directory
####################################################################################################
SMARTCARD_SRC_DIR := $(DRIVERS_SRC_DIR)/smartcard


####################################################################################################
# source file
####################################################################################################
SRCS_SMARTCARD_C_RAM = \
	$(wildcard $(SMARTCARD_SRC_DIR)/src/*.c) \
	$(wildcard $(SMARTCARD_SRC_DIR)/src/*.s) \
	$(wildcard $(SMARTCARD_SRC_DIR)/src/*.S) \


SRCS_SMARTCARD_C_FLASH = \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


SRCS_SMARTCARD_EXCLUDE_LIB_RAM = \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


SRCS_SMARTCARD_EXCLUDE_LIB_FLASH = \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


SRCS_SMARTCARD_EXCLUDE_PRJ = \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


####################################################################################################
# source
####################################################################################################
TMP_SMARTCARD_FILE := $(filter-out $(SRCS_SMARTCARD_C_FLASH), $(SRCS_SMARTCARD_C_RAM))
TMP_SMARTCARD_FILE := $(filter-out $(SRCS_SMARTCARD_EXCLUDE_LIB_RAM), $(TMP_SMARTCARD_FILE))
TMP_SMARTCARD_FILE := $(filter-out $(SRCS_SMARTCARD_EXCLUDE_LIB_FLASH), $(TMP_SMARTCARD_FILE))
TMP_SMARTCARD_FILE := $(filter-out $(SRCS_SMARTCARD_EXCLUDE_PRJ), $(TMP_SMARTCARD_FILE))

SRCS_SMARTCARD_RAM   += $(TMP_SMARTCARD_FILE)
SRCS_SMARTCARD_FLASH += $(SRCS_SMARTCARD_C_FLASH)

SMARTCARD_EXCLUDE_LIB_RAM   += $(SRCS_SMARTCARD_EXCLUDE_LIB_RAM)
SMARTCARD_EXCLUDE_LIB_FLASH += $(SRCS_SMARTCARD_EXCLUDE_LIB_FLASH)


####################################################################################################
# include path
####################################################################################################
DEPS_PATH_SMARTCARD += $(SMARTCARD_SRC_DIR)/inc


####################################################################################################
# add to drivers.mk
####################################################################################################
SRCS_DRIVERS_RAM   += $(SRCS_SMARTCARD_RAM)
SRCS_DRIVERS_FLASH += $(SRCS_SMARTCARD_FLASH)

DRIVERS_EXCLUDE_LIB_RAM   += $(SMARTCARD_EXCLUDE_LIB_RAM)
DRIVERS_EXCLUDE_LIB_FLASH += $(SMARTCARD_EXCLUDE_LIB_FLASH)

DEPS_PATH_DRIVERS += $(DEPS_PATH_SMARTCARD)