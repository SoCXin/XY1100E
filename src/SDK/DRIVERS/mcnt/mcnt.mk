####################################################################################################
# directory
####################################################################################################
MCNT_SRC_DIR := $(DRIVERS_SRC_DIR)/mcnt


####################################################################################################
# source file
####################################################################################################
SRCS_MCNT_C_RAM = \
	$(wildcard $(MCNT_SRC_DIR)/src/*.c) \
	$(wildcard $(MCNT_SRC_DIR)/src/*.s) \
	$(wildcard $(MCNT_SRC_DIR)/src/*.S) \


SRCS_MCNT_C_FLASH = \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


SRCS_MCNT_EXCLUDE_LIB_RAM = \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


SRCS_MCNT_EXCLUDE_LIB_FLASH = \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


SRCS_MCNT_EXCLUDE_PRJ = \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


####################################################################################################
# source
####################################################################################################
TMP_MCNT_FILE := $(filter-out $(SRCS_MCNT_C_FLASH), $(SRCS_MCNT_C_RAM))
TMP_MCNT_FILE := $(filter-out $(SRCS_MCNT_EXCLUDE_LIB_RAM), $(TMP_MCNT_FILE))
TMP_MCNT_FILE := $(filter-out $(SRCS_MCNT_EXCLUDE_LIB_FLASH), $(TMP_MCNT_FILE))
TMP_MCNT_FILE := $(filter-out $(SRCS_MCNT_EXCLUDE_PRJ), $(TMP_MCNT_FILE))

SRCS_MCNT_RAM   += $(TMP_MCNT_FILE)
SRCS_MCNT_FLASH += $(SRCS_MCNT_C_FLASH)

MCNT_EXCLUDE_LIB_RAM   += $(SRCS_MCNT_EXCLUDE_LIB_RAM)
MCNT_EXCLUDE_LIB_FLASH += $(SRCS_MCNT_EXCLUDE_LIB_FLASH)


####################################################################################################
# include path
####################################################################################################
DEPS_PATH_MCNT += $(MCNT_SRC_DIR)/inc


####################################################################################################
# add to drivers.mk
####################################################################################################
SRCS_DRIVERS_RAM   += $(SRCS_MCNT_RAM)
SRCS_DRIVERS_FLASH += $(SRCS_MCNT_FLASH)

DRIVERS_EXCLUDE_LIB_RAM   += $(MCNT_EXCLUDE_LIB_RAM)
DRIVERS_EXCLUDE_LIB_FLASH += $(MCNT_EXCLUDE_LIB_FLASH)

DEPS_PATH_DRIVERS += $(DEPS_PATH_MCNT)