####################################################################################################
# directory
####################################################################################################
OSAL_FREERTOS_SRC_DIR := $(OSAL_SRC_DIR)/FreeRTOS


####################################################################################################
# source file
####################################################################################################
SRCS_OSAL_FREERTOS_C_RAM = \
	$(wildcard $(OSAL_FREERTOS_SRC_DIR)/Source/*.c) \
	$(wildcard $(OSAL_FREERTOS_SRC_DIR)/Source/*.s) \
	$(wildcard $(OSAL_FREERTOS_SRC_DIR)/Source/*.S) \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


SRCS_OSAL_FREERTOS_C_FLASH = \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


SRCS_OSAL_FREERTOS_EXCLUDE_LIB_RAM = \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


SRCS_OSAL_FREERTOS_EXCLUDE_LIB_FLASH = \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


SRCS_OSAL_FREERTOS_EXCLUDE_PRJ = \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


####################################################################################################
# source
####################################################################################################
TMP_OSAL_FREERTOS_FILE := $(filter-out $(SRCS_OSAL_FREERTOS_C_FLASH), $(SRCS_OSAL_FREERTOS_C_RAM))
TMP_OSAL_FREERTOS_FILE := $(filter-out $(SRCS_OSAL_FREERTOS_EXCLUDE_LIB_RAM), $(TMP_OSAL_FREERTOS_FILE))
TMP_OSAL_FREERTOS_FILE := $(filter-out $(SRCS_OSAL_FREERTOS_EXCLUDE_LIB_FLASH), $(TMP_OSAL_FREERTOS_FILE))
TMP_OSAL_FREERTOS_FILE := $(filter-out $(SRCS_OSAL_FREERTOS_EXCLUDE_PRJ), $(TMP_OSAL_FREERTOS_FILE))

SRCS_OSAL_FREERTOS_RAM   += $(TMP_OSAL_FREERTOS_FILE)
SRCS_OSAL_FREERTOS_FLASH += $(SRCS_OSAL_FREERTOS_C_FLASH)

OSAL_FREERTOS_EXCLUDE_LIB_RAM   += $(SRCS_OSAL_FREERTOS_EXCLUDE_LIB_RAM)
OSAL_FREERTOS_EXCLUDE_LIB_FLASH += $(SRCS_OSAL_FREERTOS_EXCLUDE_LIB_FLASH)


####################################################################################################
# include path
####################################################################################################
DEPS_PATH_OSAL_FREERTOS += $(OSAL_FREERTOS_SRC_DIR)/Config
DEPS_PATH_OSAL_FREERTOS += $(OSAL_FREERTOS_SRC_DIR)/Include


####################################################################################################
# add to osal.mk
####################################################################################################
SRCS_KERNEL_RAM   += $(SRCS_OSAL_FREERTOS_RAM)
SRCS_KERNEL_FLASH += $(SRCS_OSAL_FREERTOS_FLASH)

KERNEL_EXCLUDE_LIB_RAM   += $(OSAL_FREERTOS_EXCLUDE_LIB_RAM)
KERNEL_EXCLUDE_LIB_FLASH += $(OSAL_FREERTOS_EXCLUDE_LIB_FLASH)

DEPS_PATH_KERNEL += $(DEPS_PATH_OSAL_FREERTOS)