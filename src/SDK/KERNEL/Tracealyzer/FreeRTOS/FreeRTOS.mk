####################################################################################################
# directory
####################################################################################################
TRACEALYZER_FREERTOS_SRC_DIR := $(TRACEALYZER_SRC_DIR)/FreeRTOS


####################################################################################################
# source file
####################################################################################################
SRCS_TRACEALYZER_FREERTOS_C_RAM = \
	$(wildcard $(TRACEALYZER_FREERTOS_SRC_DIR)/*.c) \
	$(wildcard $(TRACEALYZER_FREERTOS_SRC_DIR)/streamports/Jlink_RTT/*.c) \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


SRCS_TRACEALYZER_FREERTOS_C_FLASH = \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


SRCS_TRACEALYZER_FREERTOS_EXCLUDE_LIB_RAM = \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


SRCS_TRACEALYZER_FREERTOS_EXCLUDE_LIB_FLASH = \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


SRCS_TRACEALYZER_FREERTOS_EXCLUDE_PRJ = \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


####################################################################################################
# source
####################################################################################################
TRACEALYZER_FREERTOS_BASE_FILE_GROUP  = $(SRCS_TRACEALYZER_FREERTOS_C_RAM)
TRACEALYZER_FREERTOS_OTHER_FILE_GROUP = $(SRCS_TRACEALYZER_FREERTOS_C_FLASH) $(SRCS_TRACEALYZER_FREERTOS_EXCLUDE_LIB_RAM) $(SRCS_TRACEALYZER_FREERTOS_EXCLUDE_LIB_FLASH)

TMP_TRACEALYZER_FREERTOS_FILE = $(filter-out $(TRACEALYZER_FREERTOS_OTHER_FILE_GROUP), $(TRACEALYZER_FREERTOS_BASE_FILE_GROUP))

SRCS_TRACEALYZER_FREERTOS_RAM          += $(filter-out $(SRCS_TRACEALYZER_FREERTOS_EXCLUDE_PRJ), $(TMP_TRACEALYZER_FREERTOS_FILE))
SRCS_TRACEALYZER_FREERTOS_FLASH        += $(filter-out $(SRCS_TRACEALYZER_FREERTOS_EXCLUDE_PRJ), $(SRCS_TRACEALYZER_FREERTOS_C_FLASH))
TRACEALYZER_FREERTOS_EXCLUDE_LIB_RAM   += $(filter-out $(SRCS_TRACEALYZER_FREERTOS_EXCLUDE_PRJ), $(SRCS_TRACEALYZER_FREERTOS_EXCLUDE_LIB_RAM))
TRACEALYZER_FREERTOS_EXCLUDE_LIB_FLASH += $(filter-out $(SRCS_TRACEALYZER_FREERTOS_EXCLUDE_PRJ), $(SRCS_TRACEALYZER_FREERTOS_EXCLUDE_LIB_FLASH))


####################################################################################################
# include path
####################################################################################################
DEPS_PATH_TRACEALYZER_FREERTOS += $(TRACEALYZER_FREERTOS_SRC_DIR)/config
DEPS_PATH_TRACEALYZER_FREERTOS += $(TRACEALYZER_FREERTOS_SRC_DIR)/include
DEPS_PATH_TRACEALYZER_FREERTOS += $(TRACEALYZER_FREERTOS_SRC_DIR)/streamports/Jlink_RTT/include


####################################################################################################
# add to Tracealyzer.mk
####################################################################################################
SRCS_KERNEL_RAM   += $(SRCS_TRACEALYZER_FREERTOS_RAM)
SRCS_KERNEL_FLASH += $(SRCS_TRACEALYZER_FREERTOS_FLASH)

KERNEL_EXCLUDE_LIB_RAM   += $(TRACEALYZER_FREERTOS_EXCLUDE_LIB_RAM)
KERNEL_EXCLUDE_LIB_FLASH += $(TRACEALYZER_FREERTOS_EXCLUDE_LIB_FLASH)

DEPS_PATH_KERNEL += $(DEPS_PATH_TRACEALYZER_FREERTOS)