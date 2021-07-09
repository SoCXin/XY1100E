####################################################################################################
# directory
####################################################################################################
ATUART_SRC_DIR := $(SYSLIB_SRC_DIR)/at_uart


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
# add to syslib.mk
####################################################################################################
SRCS_SYSLIB_RAM   += $(SRCS_ATUART_RAM)
SRCS_SYSLIB_FLASH += $(SRCS_ATUART_FLASH)

SYSLIB_EXCLUDE_LIB_RAM   += $(ATUART_EXCLUDE_LIB_RAM)
SYSLIB_EXCLUDE_LIB_FLASH += $(ATUART_EXCLUDE_LIB_FLASH)

DEPS_PATH_SYSLIB += $(DEPS_PATH_ATUART)