####################################################################################################
# directory
####################################################################################################
MPU_SRC_DIR := $(DRIVERS_SRC_DIR)/mpu


####################################################################################################
# source file
####################################################################################################
SRCS_MPU_C_RAM = \
	$(wildcard $(MPU_SRC_DIR)/src/*.c) \
	$(wildcard $(MPU_SRC_DIR)/src/*.s) \
	$(wildcard $(MPU_SRC_DIR)/src/*.S) \


SRCS_MPU_C_FLASH = \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


SRCS_MPU_EXCLUDE_LIB_RAM = \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


SRCS_MPU_EXCLUDE_LIB_FLASH = \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


SRCS_MPU_EXCLUDE_PRJ = \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


####################################################################################################
# source
####################################################################################################
TMP_MPU_FILE := $(filter-out $(SRCS_MPU_C_FLASH), $(SRCS_MPU_C_RAM))
TMP_MPU_FILE := $(filter-out $(SRCS_MPU_EXCLUDE_LIB_RAM), $(TMP_MPU_FILE))
TMP_MPU_FILE := $(filter-out $(SRCS_MPU_EXCLUDE_LIB_FLASH), $(TMP_MPU_FILE))
TMP_MPU_FILE := $(filter-out $(SRCS_MPU_EXCLUDE_PRJ), $(TMP_MPU_FILE))

SRCS_MPU_RAM   += $(TMP_MPU_FILE)
SRCS_MPU_FLASH += $(SRCS_MPU_C_FLASH)

MPU_EXCLUDE_LIB_RAM   += $(SRCS_MPU_EXCLUDE_LIB_RAM)
MPU_EXCLUDE_LIB_FLASH += $(SRCS_MPU_EXCLUDE_LIB_FLASH)


####################################################################################################
# include path
####################################################################################################
DEPS_PATH_MPU += $(MPU_SRC_DIR)/inc


####################################################################################################
# add to drivers.mk
####################################################################################################
SRCS_DRIVERS_RAM   += $(SRCS_MPU_RAM)
SRCS_DRIVERS_FLASH += $(SRCS_MPU_FLASH)

DRIVERS_EXCLUDE_LIB_RAM   += $(MPU_EXCLUDE_LIB_RAM)
DRIVERS_EXCLUDE_LIB_FLASH += $(MPU_EXCLUDE_LIB_FLASH)

DEPS_PATH_DRIVERS += $(DEPS_PATH_MPU)