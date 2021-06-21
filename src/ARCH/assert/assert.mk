####################################################################################################
# directory
####################################################################################################
ASSERT_SRC_DIR := $(ARCH_SRC_DIR)/assert


####################################################################################################
# source file
####################################################################################################
SRCS_ASSERT_C_RAM = \
	$(wildcard $(ASSERT_SRC_DIR)/src/*.c) \
	$(wildcard $(ASSERT_SRC_DIR)/src/*.s) \
	$(wildcard $(ASSERT_SRC_DIR)/src/*.S) \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


SRCS_ASSERT_C_FLASH = \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


SRCS_ASSERT_EXCLUDE_LIB_RAM = \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


SRCS_ASSERT_EXCLUDE_LIB_FLASH = \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


SRCS_ASSERT_EXCLUDE_PRJ = \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


####################################################################################################
# source
####################################################################################################
TMP_ASSERT_FILE := $(filter-out $(SRCS_ASSERT_C_FLASH), $(SRCS_ASSERT_C_RAM))
TMP_ASSERT_FILE := $(filter-out $(SRCS_ASSERT_EXCLUDE_LIB_RAM), $(TMP_ASSERT_FILE))
TMP_ASSERT_FILE := $(filter-out $(SRCS_ASSERT_EXCLUDE_LIB_FLASH), $(TMP_ASSERT_FILE))
TMP_ASSERT_FILE := $(filter-out $(SRCS_ASSERT_EXCLUDE_PRJ), $(TMP_ASSERT_FILE))

SRCS_ASSERT_RAM   += $(TMP_ASSERT_FILE)
SRCS_ASSERT_FLASH += $(SRCS_ASSERT_C_FLASH)

ASSERT_EXCLUDE_LIB_RAM   += $(SRCS_ASSERT_EXCLUDE_LIB_RAM)
ASSERT_EXCLUDE_LIB_FLASH += $(SRCS_ASSERT_EXCLUDE_LIB_FLASH)


####################################################################################################
# include path
####################################################################################################
DEPS_PATH_ASSERT += $(ASSERT_SRC_DIR)/inc


####################################################################################################
# add to arch.mk
####################################################################################################
SRCS_ARCH_RAM   += $(SRCS_ASSERT_RAM)
SRCS_ARCH_FLASH += $(SRCS_ASSERT_FLASH)

ARCH_EXCLUDE_LIB_RAM   += $(ASSERT_EXCLUDE_LIB_RAM)
ARCH_EXCLUDE_LIB_FLASH += $(ASSERT_EXCLUDE_LIB_FLASH)

DEPS_PATH_ARCH += $(DEPS_PATH_ASSERT)