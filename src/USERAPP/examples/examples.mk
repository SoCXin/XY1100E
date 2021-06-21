####################################################################################################
# directory
####################################################################################################
EXAMPLES_SRC_DIR := $(USERAPP_SRC_DIR)/examples


####################################################################################################
# adapt source file
####################################################################################################
SRCS_EXAMPLES_C_RAM = \

#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


SRCS_EXAMPLES_C_FLASH = \


#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


SRCS_EXAMPLES_EXCLUDE_LIB_RAM = \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


SRCS_EXAMPLES_EXCLUDE_LIB_FLASH = \
	$(wildcard $(EXAMPLES_SRC_DIR)/external_mcu_demo/*.c) \
	$(wildcard $(EXAMPLES_SRC_DIR)/xy_at_demo/*.c) \
	$(wildcard $(EXAMPLES_SRC_DIR)/xy_opencpu_demo/*.c) \
	$(wildcard $(EXAMPLES_SRC_DIR)/xy_cloud_demo/cdp/*.c) \
	$(wildcard $(EXAMPLES_SRC_DIR)/xy_cloud_demo/onenet/*.c) \
	$(wildcard $(EXAMPLES_SRC_DIR)/xy_cloud_demo/*.c) \
	$(wildcard $(EXAMPLES_SRC_DIR)/xy_peripheral_demo/flash/*.c) \
	$(wildcard $(EXAMPLES_SRC_DIR)/xy_peripheral_demo/hal_adc/*.c) \
	$(wildcard $(EXAMPLES_SRC_DIR)/xy_peripheral_demo/hal_csp/*.c) \
	$(wildcard $(EXAMPLES_SRC_DIR)/xy_peripheral_demo/hal_gpio/*.c) \
	$(wildcard $(EXAMPLES_SRC_DIR)/xy_peripheral_demo/hal_i2c/*.c) \
	$(wildcard $(EXAMPLES_SRC_DIR)/xy_peripheral_demo/hal_ipc/*.c) \
	$(wildcard $(EXAMPLES_SRC_DIR)/xy_peripheral_demo/hal_spi/*.c) \
	$(wildcard $(EXAMPLES_SRC_DIR)/xy_peripheral_demo/hal_tim/*.c) \
	$(wildcard $(EXAMPLES_SRC_DIR)/xy_peripheral_demo/hal_uart/*.c) \
	$(wildcard $(EXAMPLES_SRC_DIR)/xy_peripheral_demo/gpio_wakeup/*.c) \
	$(wildcard $(EXAMPLES_SRC_DIR)/user_shmmsg_demo/*.c) \
	$(wildcard $(EXAMPLES_SRC_DIR)/NB+MCU_SPI_DEMO/COMMON_PROTOCOL/*.c) \
	$(wildcard $(EXAMPLES_SRC_DIR)/NB+MCU_SPI_DEMO/NB_SPI_MASTER/*.c) \
	$(wildcard $(EXAMPLES_SRC_DIR)/NB+MCU_SPI_DEMO/MCU_SPI_SLAVE/*.c) \
	$(wildcard $(EXAMPLES_SRC_DIR)/external_devices_demo/BY25D80AS/*.c) \
	$(wildcard $(EXAMPLES_SRC_DIR)/external_devices_demo/GC6153/*.c) \
	$(wildcard $(EXAMPLES_SRC_DIR)/external_devices_demo/SHT30/*.c) \
	$(wildcard $(EXAMPLES_SRC_DIR)/external_devices_demo/ST7735/*.c) \
	$(wildcard $(EXAMPLES_SRC_DIR)/xy_socket_demo/*.c) \
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


SRCS_EXAMPLES_EXCLUDE_PRJ = \
	$(EXAMPLES_SRC_DIR)/xy_cloud_demo/xy_fota_demo.c
#----------------------------------------------------------------------- \
# if need add more file, move the bottom row to the top \
#----------------------------------------------------------------------- \


####################################################################################################
# source
####################################################################################################
TMP_EXAMPLES_FILE := $(filter-out $(SRCS_EXAMPLES_C_RAM), $(SRCS_EXAMPLES_C_FLASH))
TMP_EXAMPLES_FILE := $(filter-out $(SRCS_EXAMPLES_EXCLUDE_LIB_RAM), $(TMP_EXAMPLES_FILE))
TMP_EXAMPLES_FILE := $(filter-out $(SRCS_EXAMPLES_EXCLUDE_LIB_FLASH), $(TMP_EXAMPLES_FILE))
TMP_EXAMPLES_FILE := $(filter-out $(SRCS_EXAMPLES_EXCLUDE_PRJ), $(TMP_EXAMPLES_FILE))

SRCS_EXAMPLES_RAM   += $(SRCS_EXAMPLES_C_RAM)
SRCS_EXAMPLES_FLASH += $(TMP_EXAMPLES_FILE)

EXAMPLES_EXCLUDE_LIB_RAM   += $(SRCS_EXAMPLES_EXCLUDE_LIB_RAM)
EXAMPLES_EXCLUDE_LIB_FLASH += $(SRCS_EXAMPLES_EXCLUDE_LIB_FLASH)


####################################################################################################
# include path
####################################################################################################
DEPS_PATH_EXAMPLES += $(EXAMPLES_SRC_DIR)/external_devices_demo/GC6153
DEPS_PATH_EXAMPLES += $(EXAMPLES_SRC_DIR)/external_devices_demo/ST7735
DEPS_PATH_EXAMPLES += $(EXAMPLES_SRC_DIR)/NB+MCU_SPI_DEMO/MCU_SPI_SLAVE
DEPS_PATH_EXAMPLES += $(EXAMPLES_SRC_DIR)/NB+MCU_SPI_DEMO/NB_SPI_MASTER
DEPS_PATH_EXAMPLES += $(EXAMPLES_SRC_DIR)/NB+MCU_SPI_DEMO/COMMON_PROTOCOL


####################################################################################################
# add to sysapp.mk
####################################################################################################
SRCS_USERAPP_RAM   += $(SRCS_EXAMPLES_RAM)
SRCS_USERAPP_FLASH += $(SRCS_EXAMPLES_FLASH)

USERAPP_EXCLUDE_LIB_RAM   += $(EXAMPLES_EXCLUDE_LIB_RAM)
USERAPP_EXCLUDE_LIB_FLASH += $(EXAMPLES_EXCLUDE_LIB_FLASH)

DEPS_PATH_USERAPP += $(DEPS_PATH_EXAMPLES)