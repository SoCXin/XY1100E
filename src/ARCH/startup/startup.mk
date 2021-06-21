####################################################################################################
# directory
####################################################################################################
STARTUP_SRC_DIR := $(ARCH_SRC_DIR)/startup


####################################################################################################
# source file
####################################################################################################
SRCS_STARTUP_C_RAM = \
	$(wildcard $(STARTUP_SRC_DIR)/src/*.c) \
	$(wildcard $(STARTUP_SRC_DIR)/src/*.s) \
	$(wildcard $(STARTUP_SRC_DIR)/src/*.S) \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


SRCS_STARTUP_C_FLASH = \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \
	$(STARTUP_SRC_DIR)/src/system_cm3.c \


SRCS_STARTUP_EXCLUDE_LIB_RAM = \
	$(STARTUP_SRC_DIR)/src/startup_cm3.s \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


SRCS_STARTUP_EXCLUDE_LIB_FLASH = \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


SRCS_STARTUP_EXCLUDE_PRJ = \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


####################################################################################################
# source
####################################################################################################
TMP_STARTUP_FILE := $(filter-out $(SRCS_STARTUP_C_FLASH), $(SRCS_STARTUP_C_RAM))
TMP_STARTUP_FILE := $(filter-out $(SRCS_STARTUP_EXCLUDE_LIB_RAM), $(TMP_STARTUP_FILE))
TMP_STARTUP_FILE := $(filter-out $(SRCS_STARTUP_EXCLUDE_LIB_FLASH), $(TMP_STARTUP_FILE))
TMP_STARTUP_FILE := $(filter-out $(SRCS_STARTUP_EXCLUDE_PRJ), $(TMP_STARTUP_FILE))

SRCS_STARTUP_RAM   += $(TMP_STARTUP_FILE)
SRCS_STARTUP_FLASH += $(SRCS_STARTUP_C_FLASH)

STARTUP_EXCLUDE_LIB_RAM   += $(SRCS_STARTUP_EXCLUDE_LIB_RAM)
STARTUP_EXCLUDE_LIB_FLASH += $(SRCS_STARTUP_EXCLUDE_LIB_FLASH)


####################################################################################################
# include path
####################################################################################################
DEPS_PATH_STARTUP += $(STARTUP_SRC_DIR)/inc


####################################################################################################
# add to arch.mk
####################################################################################################
SRCS_ARCH_RAM   += $(SRCS_STARTUP_RAM)
SRCS_ARCH_FLASH += $(SRCS_STARTUP_FLASH)

ARCH_EXCLUDE_LIB_RAM   += $(STARTUP_EXCLUDE_LIB_RAM)
ARCH_EXCLUDE_LIB_FLASH += $(STARTUP_EXCLUDE_LIB_FLASH)

DEPS_PATH_ARCH += $(DEPS_PATH_STARTUP)