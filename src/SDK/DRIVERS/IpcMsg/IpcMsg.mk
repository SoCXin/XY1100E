####################################################################################################
# directory
####################################################################################################
IPCMSG_SRC_DIR := $(DRIVERS_SRC_DIR)/IpcMsg


####################################################################################################
# source file
####################################################################################################
SRCS_IPCMSG_C_RAM = \
	$(wildcard $(IPCMSG_SRC_DIR)/src/*.c) \
	$(wildcard $(IPCMSG_SRC_DIR)/src/*.s) \
	$(wildcard $(IPCMSG_SRC_DIR)/src/*.S) \


SRCS_IPCMSG_C_FLASH = \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


SRCS_IPCMSG_EXCLUDE_LIB_RAM = \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


SRCS_IPCMSG_EXCLUDE_LIB_FLASH = \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


SRCS_IPCMSG_EXCLUDE_PRJ = \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


####################################################################################################
# source
####################################################################################################
TMP_IPCMSG_FILE := $(filter-out $(SRCS_IPCMSG_C_FLASH), $(SRCS_IPCMSG_C_RAM))
TMP_IPCMSG_FILE := $(filter-out $(SRCS_IPCMSG_EXCLUDE_LIB_RAM), $(TMP_IPCMSG_FILE))
TMP_IPCMSG_FILE := $(filter-out $(SRCS_IPCMSG_EXCLUDE_LIB_FLASH), $(TMP_IPCMSG_FILE))
TMP_IPCMSG_FILE := $(filter-out $(SRCS_IPCMSG_EXCLUDE_PRJ), $(TMP_IPCMSG_FILE))

SRCS_IPCMSG_RAM   += $(TMP_IPCMSG_FILE)
SRCS_IPCMSG_FLASH += $(SRCS_IPCMSG_C_FLASH)

IPCMSG_EXCLUDE_LIB_RAM   += $(SRCS_IPCMSG_EXCLUDE_LIB_RAM)
IPCMSG_EXCLUDE_LIB_FLASH += $(SRCS_IPCMSG_EXCLUDE_LIB_FLASH)


####################################################################################################
# include path
####################################################################################################
DEPS_PATH_IPCMSG += $(IPCMSG_SRC_DIR)/inc


####################################################################################################
# add to drivers.mk
####################################################################################################
SRCS_DRIVERS_RAM   += $(SRCS_IPCMSG_RAM)
SRCS_DRIVERS_FLASH += $(SRCS_IPCMSG_FLASH)

DRIVERS_EXCLUDE_LIB_RAM   += $(IPCMSG_EXCLUDE_LIB_RAM)
DRIVERS_EXCLUDE_LIB_FLASH += $(IPCMSG_EXCLUDE_LIB_FLASH)

DEPS_PATH_DRIVERS += $(DEPS_PATH_IPCMSG)