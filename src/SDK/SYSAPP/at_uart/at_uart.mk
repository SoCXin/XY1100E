####################################################################################################
# directory
####################################################################################################
ATUART_SRC_DIR := $(SYSAPP_SRC_DIR)/at_uart


####################################################################################################
# source file
####################################################################################################
SRCS_ATUART_C_RAM = \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


SRCS_ATUART_C_FLASH = \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


SRCS_ATUART_EXCLUDE_LIB_RAM = \
	$(ATUART_SRC_DIR)/src/dirty_data_processing.c \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


SRCS_ATUART_EXCLUDE_LIB_FLASH = \
	$(wildcard $(ATUART_SRC_DIR)/src/*.c) \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


SRCS_ATUART_EXCLUDE_PRJ = \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


####################################################################################################
# source
####################################################################################################
ATUART_BASE_FILE_GROUP  = $(SRCS_ATUART_EXCLUDE_LIB_FLASH)
ATUART_OTHER_FILE_GROUP = $(SRCS_ATUART_C_RAM) $(SRCS_ATUART_C_FLASH) $(SRCS_ATUART_EXCLUDE_LIB_RAM)

TMP_ATUART_FILE = $(filter-out $(ATUART_OTHER_FILE_GROUP), $(ATUART_BASE_FILE_GROUP))

SRCS_ATUART_RAM          += $(filter-out $(SRCS_ATUART_EXCLUDE_PRJ), $(SRCS_ATUART_C_RAM))
SRCS_ATUART_FLASH        += $(filter-out $(SRCS_ATUART_EXCLUDE_PRJ), $(SRCS_ATUART_C_FLASH))
ATUART_EXCLUDE_LIB_RAM   += $(filter-out $(SRCS_ATUART_EXCLUDE_PRJ), $(SRCS_ATUART_EXCLUDE_LIB_RAM))
ATUART_EXCLUDE_LIB_FLASH += $(filter-out $(SRCS_ATUART_EXCLUDE_PRJ), $(TMP_ATUART_FILE))


####################################################################################################
# include path
####################################################################################################
DEPS_PATH_ATUART += $(ATUART_SRC_DIR)/inc


####################################################################################################
# add to sysapp.mk
####################################################################################################
SRCS_SYSAPP_RAM   += $(SRCS_ATUART_RAM)
SRCS_SYSAPP_FLASH += $(SRCS_ATUART_FLASH)

SYSAPP_EXCLUDE_LIB_RAM   += $(ATUART_EXCLUDE_LIB_RAM)
SYSAPP_EXCLUDE_LIB_FLASH += $(ATUART_EXCLUDE_LIB_FLASH)

DEPS_PATH_SYSAPP += $(DEPS_PATH_ATUART)