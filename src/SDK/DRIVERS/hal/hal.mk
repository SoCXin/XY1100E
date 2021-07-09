####################################################################################################
# directory
####################################################################################################
HAL_SRC_DIR := $(DRIVERS_SRC_DIR)/hal


####################################################################################################
# source file
####################################################################################################
SRCS_HAL_C_RAM = \
	$(wildcard $(HAL_SRC_DIR)/src/*.c) \
	$(wildcard $(HAL_SRC_DIR)/src/*.s) \
	$(wildcard $(HAL_SRC_DIR)/src/*.S) \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


SRCS_HAL_C_FLASH = \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


SRCS_HAL_EXCLUDE_LIB_RAM = \

#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


SRCS_HAL_EXCLUDE_LIB_FLASH = \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


SRCS_HAL_EXCLUDE_PRJ = \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


####################################################################################################
# source
####################################################################################################
TMP_HAL_FILE := $(filter-out $(SRCS_HAL_C_RAM), $(SRCS_HAL_EXCLUDE_LIB_RAM))
TMP_HAL_FILE := $(filter-out $(SRCS_HAL_C_FLASH), $(TMP_HAL_FILE))
TMP_HAL_FILE := $(filter-out $(SRCS_HAL_EXCLUDE_LIB_FLASH), $(TMP_HAL_FILE))
TMP_HAL_FILE := $(filter-out $(SRCS_HAL_EXCLUDE_PRJ), $(TMP_HAL_FILE))

SRCS_HAL_RAM   += $(SRCS_HAL_C_RAM)
SRCS_HAL_FLASH += $(SRCS_HAL_C_FLASH)

HAL_EXCLUDE_LIB_RAM   += $(TMP_HAL_FILE)
HAL_EXCLUDE_LIB_FLASH += $(SRCS_HAL_EXCLUDE_LIB_FLASH)


####################################################################################################
# include path
####################################################################################################
DEPS_PATH_HAL += $(HAL_SRC_DIR)/inc


####################################################################################################
# add to drivers.mk
####################################################################################################
SRCS_DRIVERS_RAM   += $(SRCS_HAL_RAM)
SRCS_DRIVERS_FLASH += $(SRCS_HAL_FLASH)

DRIVERS_EXCLUDE_LIB_RAM   += $(HAL_EXCLUDE_LIB_RAM)
DRIVERS_EXCLUDE_LIB_FLASH += $(HAL_EXCLUDE_LIB_FLASH)

DEPS_PATH_DRIVERS += $(DEPS_PATH_HAL)