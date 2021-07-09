####################################################################################################
# directory
####################################################################################################
ZeusOS_SRC_DIR := $(KERNEL_SRC_DIR)/ZeusOS


####################################################################################################
# source file
####################################################################################################
SRCS_ZeusOS_C_RAM = \

#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


SRCS_ZeusOS_C_FLASH = \
	$(wildcard $(ZeusOS_SRC_DIR)/src/*.c) \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


SRCS_ZeusOS_EXCLUDE_LIB_RAM = \

#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


SRCS_ZeusOS_EXCLUDE_LIB_FLASH = \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


SRCS_ZeusOS_EXCLUDE_PRJ = \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


####################################################################################################
# source
####################################################################################################
TMP_ZeusOS_FILE := $(filter-out $(SRCS_ZeusOS_C_RAM), $(SRCS_ZeusOS_EXCLUDE_LIB_RAM))
TMP_ZeusOS_FILE := $(filter-out $(SRCS_ZeusOS_C_FLASH), $(TMP_ZeusOS_FILE))
TMP_ZeusOS_FILE := $(filter-out $(SRCS_ZeusOS_EXCLUDE_LIB_FLASH), $(TMP_ZeusOS_FILE))
TMP_ZeusOS_FILE := $(filter-out $(SRCS_ZeusOS_EXCLUDE_PRJ), $(TMP_ZeusOS_FILE))

SRCS_ZeusOS_RAM   += $(SRCS_ZeusOS_C_RAM)
SRCS_ZeusOS_FLASH += $(SRCS_ZeusOS_C_FLASH)

ZeusOS_EXCLUDE_LIB_RAM   += $(TMP_ZeusOS_FILE)
ZeusOS_EXCLUDE_LIB_FLASH += $(SRCS_ZeusOS_EXCLUDE_LIB_FLASH)


####################################################################################################
# include path
####################################################################################################
DEPS_PATH_ZeusOS += $(ZeusOS_SRC_DIR)/inc


####################################################################################################
# add to kernel.mk
####################################################################################################
SRCS_KERNEL_RAM   += $(SRCS_ZeusOS_RAM)
SRCS_KERNEL_FLASH += $(SRCS_ZeusOS_FLASH)

KERNEL_EXCLUDE_LIB_RAM   += $(ZeusOS_EXCLUDE_LIB_RAM)
KERNEL_EXCLUDE_LIB_FLASH += $(ZeusOS_EXCLUDE_LIB_FLASH)

DEPS_PATH_KERNEL += $(DEPS_PATH_ZeusOS)