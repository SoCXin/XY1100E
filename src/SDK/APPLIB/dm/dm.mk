####################################################################################################
# directory
####################################################################################################
DM_SRC_DIR := $(APPLIB_SRC_DIR)/dm


####################################################################################################
# source file
####################################################################################################
SRCS_DM_C_RAM = \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


SRCS_DM_C_FLASH = \

ifeq ($(XY_DTLS_SUPPORT),n)
SRCS_DM_C_FLASH+=$(APPLIB_SRC_DIR)/Dtls/mbedtls-2.6.0/library/aes.c \
	$(APPLIB_SRC_DIR)/Dtls/mbedtls-2.6.0/library/sha256.c 
endif
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


SRCS_DM_EXCLUDE_LIB_RAM = \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


SRCS_DM_EXCLUDE_LIB_FLASH = \
	$(wildcard $(DM_SRC_DIR)/xy_dm/src/*.c) \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


SRCS_DM_EXCLUDE_PRJ = \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


####################################################################################################
# source
####################################################################################################
TMP_DM_FILE := $(filter-out $(SRCS_DM_C_RAM), $(SRCS_DM_EXCLUDE_LIB_FLASH))
TMP_DM_FILE := $(filter-out $(SRCS_DM_C_FLASH), $(TMP_DM_FILE))
TMP_DM_FILE := $(filter-out $(SRCS_DM_EXCLUDE_LIB_RAM), $(TMP_DM_FILE))
TMP_DM_FILE := $(filter-out $(SRCS_DM_EXCLUDE_PRJ), $(TMP_DM_FILE))

SRCS_DM_RAM   += $(SRCS_DM_C_RAM)
SRCS_DM_FLASH += $(SRCS_DM_C_FLASH)

DM_EXCLUDE_LIB_RAM   += $(SRCS_DM_EXCLUDE_LIB_RAM)
DM_EXCLUDE_LIB_FLASH += $(TMP_DM_FILE)


####################################################################################################
# include path
####################################################################################################
DEPS_PATH_DM += $(DM_SRC_DIR)/inc
ifeq ($(XY_DTLS_SUPPORT),n)
DEPS_PATH_DM += $(APPLIB_SRC_DIR)/Dtls/mbedtls-2.6.0/include
endif
####################################################################################################
# add to sysapp.mk
####################################################################################################
SRCS_APPLIB_RAM   += $(SRCS_DM_RAM)
SRCS_APPLIB_FLASH += $(SRCS_DM_FLASH)

APPLIB_EXCLUDE_LIB_RAM   += $(DM_EXCLUDE_LIB_RAM)
APPLIB_EXCLUDE_LIB_FLASH += $(DM_EXCLUDE_LIB_FLASH)

DEPS_PATH_APPLIB += $(DEPS_PATH_DM)