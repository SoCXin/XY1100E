####################################################################################################
# directory
####################################################################################################
XYPERF_SRC_DIR := $(APPLIB_SRC_DIR)/perf


####################################################################################################
# source file
####################################################################################################
SRCS_XYPERF_C_RAM = \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


SRCS_XYPERF_C_FLASH = \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


SRCS_XYPERF_EXCLUDE_LIB_RAM = \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


SRCS_XYPERF_EXCLUDE_LIB_FLASH = \
	$(wildcard $(XYPERF_SRC_DIR)/xy_perf/src/*.c) \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


SRCS_XYPERF_EXCLUDE_PRJ = \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


####################################################################################################
# source
####################################################################################################
TMP_XYPERF_FILE := $(filter-out $(SRCS_XYPERF_C_RAM), $(SRCS_XYPERF_EXCLUDE_LIB_FLASH))
TMP_XYPERF_FILE := $(filter-out $(SRCS_XYPERF_C_FLASH), $(TMP_XYPERF_FILE))
TMP_XYPERF_FILE := $(filter-out $(SRCS_XYPERF_EXCLUDE_LIB_RAM), $(TMP_XYPERF_FILE))
TMP_XYPERF_FILE := $(filter-out $(SRCS_XYPERF_EXCLUDE_PRJ), $(TMP_XYPERF_FILE))

SRCS_XYPERF_RAM   += $(SRCS_XYPERF_C_RAM)
SRCS_XYPERF_FLASH += $(SRCS_XYPERF_C_FLASH)

XYPERF_EXCLUDE_LIB_RAM   += $(SRCS_XYPERF_EXCLUDE_LIB_RAM)
XYPERF_EXCLUDE_LIB_FLASH += $(TMP_XYPERF_FILE)


####################################################################################################
# include path
####################################################################################################
DEPS_PATH_XYPERF += $(XYPERF_SRC_DIR)/xy_perf/inc


####################################################################################################
# add to sysapp.mk
####################################################################################################
SRCS_APPLIB_RAM   += $(SRCS_XYPERF_RAM)
SRCS_APPLIB_FLASH += $(SRCS_XYPERF_FLASH)

APPLIB_EXCLUDE_LIB_RAM   += $(XYPERF_EXCLUDE_LIB_RAM)
APPLIB_EXCLUDE_LIB_FLASH += $(XYPERF_EXCLUDE_LIB_FLASH)

DEPS_PATH_APPLIB += $(DEPS_PATH_XYPERF)