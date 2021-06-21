####################################################################################################
# directory
####################################################################################################
DRIVERLIB_SRC_DIR := $(DRIVERS_SRC_DIR)/driverlib


####################################################################################################
# source file
####################################################################################################
SRCS_DRIVERLIB_C_RAM = \
	$(wildcard $(DRIVERLIB_SRC_DIR)/src/*.c) \
	$(wildcard $(DRIVERLIB_SRC_DIR)/src/*.s) \
	$(wildcard $(DRIVERLIB_SRC_DIR)/src/*.S) \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


SRCS_DRIVERLIB_C_FLASH = \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


SRCS_DRIVERLIB_EXCLUDE_LIB_RAM = \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


SRCS_DRIVERLIB_EXCLUDE_LIB_FLASH = \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


SRCS_DRIVERLIB_EXCLUDE_PRJ = \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


####################################################################################################
# source
####################################################################################################
TMP_DRIVERLIB_FILE := $(filter-out $(SRCS_DRIVERLIB_C_FLASH), $(SRCS_DRIVERLIB_C_RAM))
TMP_DRIVERLIB_FILE := $(filter-out $(SRCS_DRIVERLIB_EXCLUDE_LIB_RAM), $(TMP_DRIVERLIB_FILE))
TMP_DRIVERLIB_FILE := $(filter-out $(SRCS_DRIVERLIB_EXCLUDE_LIB_FLASH), $(TMP_DRIVERLIB_FILE))
TMP_DRIVERLIB_FILE := $(filter-out $(SRCS_DRIVERLIB_EXCLUDE_PRJ), $(TMP_DRIVERLIB_FILE))

SRCS_DRIVERLIB_RAM   += $(TMP_DRIVERLIB_FILE)
SRCS_DRIVERLIB_FLASH += $(SRCS_DRIVERLIB_C_FLASH)

DRIVERLIB_EXCLUDE_LIB_RAM   += $(SRCS_DRIVERLIB_EXCLUDE_LIB_RAM)
DRIVERLIB_EXCLUDE_LIB_FLASH += $(SRCS_DRIVERLIB_EXCLUDE_LIB_FLASH)


####################################################################################################
# include path
####################################################################################################
DEPS_PATH_DRIVERLIB += $(DRIVERLIB_SRC_DIR)/inc


####################################################################################################
# add to drivers.mk
####################################################################################################
SRCS_DRIVERS_RAM   += $(SRCS_DRIVERLIB_RAM)
SRCS_DRIVERS_FLASH += $(SRCS_DRIVERLIB_FLASH)

DRIVERS_EXCLUDE_LIB_RAM   += $(DRIVERLIB_EXCLUDE_LIB_RAM)
DRIVERS_EXCLUDE_LIB_FLASH += $(DRIVERLIB_EXCLUDE_LIB_FLASH)

DEPS_PATH_DRIVERS += $(DEPS_PATH_DRIVERLIB)