####################################################################################################
# directory
####################################################################################################
DTLS_SRC_DIR := $(APPLIB_SRC_DIR)/Dtls


####################################################################################################
# source file
####################################################################################################
SRCS_DTLS_C_RAM = \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


SRCS_DTLS_C_FLASH = \
	$(wildcard $(DTLS_SRC_DIR)/mbedtls-2.6.0/library/*.c) \
	$(wildcard $(DTLS_SRC_DIR)/xy_dtls/src/*.c) \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


SRCS_DTLS_EXCLUDE_LIB_RAM = \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


SRCS_DTLS_EXCLUDE_LIB_FLASH = \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


SRCS_DTLS_EXCLUDE_PRJ = \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


####################################################################################################
# source
####################################################################################################
TMP_DTLS_FILE := $(filter-out $(SRCS_DTLS_C_RAM), $(SRCS_DTLS_C_FLASH))
TMP_DTLS_FILE := $(filter-out $(SRCS_DTLS_EXCLUDE_LIB_FLASH), $(TMP_DTLS_FILE))
TMP_DTLS_FILE := $(filter-out $(SRCS_DTLS_EXCLUDE_LIB_RAM), $(TMP_DTLS_FILE))
TMP_DTLS_FILE := $(filter-out $(SRCS_DTLS_EXCLUDE_PRJ), $(TMP_DTLS_FILE))

SRCS_DTLS_RAM   += $(SRCS_DTLS_C_RAM)
SRCS_DTLS_FLASH += $(TMP_DTLS_FILE)

DTLS_EXCLUDE_LIB_RAM   += $(SRCS_DTLS_EXCLUDE_LIB_RAM)
DTLS_EXCLUDE_LIB_FLASH += $(SRCS_DTLS_EXCLUDE_LIB_FLASH)


####################################################################################################
# include path
####################################################################################################
DEPS_PATH_DTLS += $(DTLS_SRC_DIR)/mbedtls-2.6.0/include
DEPS_PATH_DTLS += $(DTLS_SRC_DIR)/xy_dtls/inc

####################################################################################################
# add to sysapp.mk
####################################################################################################
SRCS_APPLIB_RAM   += $(SRCS_DTLS_RAM)
SRCS_APPLIB_FLASH += $(SRCS_DTLS_FLASH)

APPLIB_EXCLUDE_LIB_RAM   += $(DTLS_EXCLUDE_LIB_RAM)
APPLIB_EXCLUDE_LIB_FLASH += $(DTLS_EXCLUDE_LIB_FLASH)

DEPS_PATH_APPLIB += $(DEPS_PATH_DTLS)