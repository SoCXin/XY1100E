####################################################################################################
# directory
####################################################################################################
ABUP_SRC_DIR := $(APPLIB_SRC_DIR)/abup
#ABUP_LIB_DIR = $(ABUP_SRC_DIR)/src

#LIB_ABUP_NAME = abup
#LIB_ABUP_SRC  = $(ABUP_LIB_DIR)/lib$(LIB_ABUP_NAME).a
####################################################################################################
# adapt source file
####################################################################################################
SRCS_ABUP_C_RAM = \

	
SRCS_ABUP_C_FLASH = \

SRCS_ABUP_EXCLUDE_LIB_RAM = \

SRCS_ABUP_EXCLUDE_LIB_FLASH = \
	$(wildcard $(ABUP_SRC_DIR)/src/*.c) 
	
SRCS_ABUP_EXCLUDE_PRJ = \

####################################################################################################
# source
####################################################################################################
TMP_ABUP_FILE := $(filter-out $(SRCS_ABUP_C_RAM), $(SRCS_ABUP_C_FLASH))
TMP_ABUP_FILE := $(filter-out $(SRCS_ABUP_EXCLUDE_LIB_RAM), $(TMP_ABUP_FILE))
TMP_ABUP_FILE := $(filter-out $(SRCS_ABUP_EXCLUDE_LIB_FLASH), $(TMP_ABUP_FILE))
TMP_ABUP_FILE := $(filter-out $(SRCS_ABUP_EXCLUDE_PRJ), $(TMP_ABUP_FILE))

SRCS_ABUP_RAM   += $(SRCS_ABUP_C_RAM)
SRCS_ABUP_FLASH += $(TMP_ABUP_FILE)

ABUP_EXCLUDE_LIB_RAM   += $(SRCS_ABUP_EXCLUDE_LIB_RAM)
ABUP_EXCLUDE_LIB_FLASH += $(SRCS_ABUP_EXCLUDE_LIB_FLASH)
####################################################################################################
# include path
####################################################################################################
DEPS_PATH_ABUP += $(ABUP_SRC_DIR)/inc

####################################################################################################
# add to APPLIB.mk
####################################################################################################
SRCS_APPLIB_RAM   += $(SRCS_ABUP_RAM)
SRCS_APPLIB_FLASH += $(SRCS_ABUP_FLASH)

APPLIB_EXCLUDE_LIB_RAM   += $(ABUP_EXCLUDE_LIB_RAM)
APPLIB_EXCLUDE_LIB_FLASH += $(ABUP_EXCLUDE_LIB_FLASH)

DEPS_PATH_APPLIB += $(DEPS_PATH_ABUP)