####################################################################################################
# directory
####################################################################################################
OS_FREERTOS_SRC_DIR := $(OS_SRC_DIR)/FreeRTOS


####################################################################################################
# source file
####################################################################################################
SRCS_OS_FREERTOS_C_RAM = \
	$(wildcard $(OS_FREERTOS_SRC_DIR)/*.c) \
	$(wildcard $(OS_FREERTOS_SRC_DIR)/*.s) \
	$(wildcard $(OS_FREERTOS_SRC_DIR)/*.S) \
	$(wildcard $(OS_FREERTOS_SRC_DIR)/portable/GCC/ARM_CM3/*.c) \
	$(wildcard $(OS_FREERTOS_SRC_DIR)/portable/GCC/ARM_CM3/*.s) \
	$(wildcard $(OS_FREERTOS_SRC_DIR)/portable/GCC/ARM_CM3/*.S) \
	$(wildcard $(OS_FREERTOS_SRC_DIR)/portable/MemMang/*.c) \
	$(wildcard $(OS_FREERTOS_SRC_DIR)/portable/MemMang/*.s) \
	$(wildcard $(OS_FREERTOS_SRC_DIR)/portable/MemMang/*.S) \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


SRCS_OS_FREERTOS_C_FLASH = \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


SRCS_OS_FREERTOS_EXCLUDE_LIB_RAM = \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


SRCS_OS_FREERTOS_EXCLUDE_LIB_FLASH = \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


SRCS_OS_FREERTOS_EXCLUDE_PRJ = \
	$(OS_FREERTOS_SRC_DIR)/croutine.c \
	$(OS_FREERTOS_SRC_DIR)/event_groups.c \
	$(OS_FREERTOS_SRC_DIR)/stream_buffer.c \
	$(OS_FREERTOS_SRC_DIR)/portable/MemMang/heap_1.c \
	$(OS_FREERTOS_SRC_DIR)/portable/MemMang/heap_2.c \
	$(OS_FREERTOS_SRC_DIR)/portable/MemMang/heap_3.c \
	$(OS_FREERTOS_SRC_DIR)/portable/MemMang/heap_4.c \
	$(OS_FREERTOS_SRC_DIR)/portable/MemMang/heap_5.c \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


####################################################################################################
# source
####################################################################################################
TMP_OS_FREERTOS_FILE := $(filter-out $(SRCS_OS_FREERTOS_C_FLASH), $(SRCS_OS_FREERTOS_C_RAM))
TMP_OS_FREERTOS_FILE := $(filter-out $(SRCS_OS_FREERTOS_EXCLUDE_LIB_RAM), $(TMP_OS_FREERTOS_FILE))
TMP_OS_FREERTOS_FILE := $(filter-out $(SRCS_OS_FREERTOS_EXCLUDE_LIB_FLASH), $(TMP_OS_FREERTOS_FILE))
TMP_OS_FREERTOS_FILE := $(filter-out $(SRCS_OS_FREERTOS_EXCLUDE_PRJ), $(TMP_OS_FREERTOS_FILE))

SRCS_OS_FREERTOS_RAM   += $(TMP_OS_FREERTOS_FILE)
SRCS_OS_FREERTOS_FLASH += $(SRCS_OS_FREERTOS_C_FLASH)

OS_FREERTOS_EXCLUDE_LIB_RAM   += $(SRCS_OS_FREERTOS_EXCLUDE_LIB_RAM)
OS_FREERTOS_EXCLUDE_LIB_FLASH += $(SRCS_OS_FREERTOS_EXCLUDE_LIB_FLASH)


####################################################################################################
# include path
####################################################################################################
DEPS_PATH_OS_FREERTOS += $(OS_FREERTOS_SRC_DIR)/include
DEPS_PATH_OS_FREERTOS += $(OS_FREERTOS_SRC_DIR)/portable/GCC/ARM_CM3


####################################################################################################
# add to kernel.mk
####################################################################################################
SRCS_KERNEL_RAM   += $(SRCS_OS_FREERTOS_RAM)
SRCS_KERNEL_FLASH += $(SRCS_OS_FREERTOS_FLASH)

KERNEL_EXCLUDE_LIB_RAM   += $(OS_FREERTOS_EXCLUDE_LIB_RAM)
KERNEL_EXCLUDE_LIB_FLASH += $(OS_FREERTOS_EXCLUDE_LIB_FLASH)

DEPS_PATH_KERNEL += $(DEPS_PATH_OS_FREERTOS)