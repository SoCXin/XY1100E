####################################################################################################
# global definition
####################################################################################################

# use for replace __FILE__
#DEFINES += -D__FILENAME__='"$(subst $(dir $<),,$<)"'
#DEFINES += -D__FILENAME__='"$(subst .c,,$(subst $(dir $<),,$<))"'
# C lib _reent
DEFINES += -D_REENT_SMALL -D_REENT_GLOBAL_ATEXIT
# use to define hardware platform
DEFINES += -D__SOC_XINYI_1100__

DEFINES+= -DMODULE_VER=\"BC28\"
DEFINES+= -DPRODUCT_VER=\"BC28JDR01A04_ONT\"
DEFINES+= -DCOMPILE_TIME='"$(shell date +"%Y-%m-%d %H:%M:%S")"'

####################################################################################################
#                    Configurable Module Provides Additional Functions                             #
#                    Could be modify by user if necessary                                          #													
####################################################################################################

ifeq ($(AT_SOCKET_SUPPORT),y)
	DEFINES += -DAT_SOCKET=1
endif

ifeq ($(XY_SOCKET_PROXY_SUPPORT),y)
	DEFINES += -DXY_SOCKET_PROXY=1
endif

ifeq ($(MOBILE_VER_SUPPORT),y)
	DEFINES += -DMOBILE_VER=1
	DEFINES += -DLWM2M_LITTLE_ENDIAN=1  #LWM2M字节序大小端模式：Little-endian，模式与芯片架构有关
endif

ifeq ($(TELECOM_VER_SUPPORT),y)
	DEFINES += -DTELECOM_VER=1
	DEFINES += -DWITH_LWIP=1            #开源库中走LWIP流程
	DEFINES += -DLWM2M_CLIENT_MODE=1    #LWM2M客户端模式，共有三种模式分别对应服务器、客户端还是启动服务器：LWM2M_SERVER_MODE、LWM2M_CLIENT_MODE、LWM2M_BOOTSTRAP_SERVER_MODE
	DEFINES += -DLWM2M_LITTLE_ENDIAN=1  #LWM2M字节序大小端模式：Little-endian，模式与芯片架构有关
endif

ifeq ($(WAKAAMA_SUPPORT),y)
	DEFINES += -DWAKAAMA=1
	DEFINES += -DXINYI_LWM2M_CLIENT_MODE=1
	DEFINES += -DXINYI_LWM2M_LITTLE_ENDIAN=1
endif

ifeq ($(LIBCOAP_SUPPORT),y)
	DEFINES += -DLIBCOAP=1
	DEFINES += -DWITH_LWIP=1            #开源库中走LWIP流程
endif

ifeq ($(MQTT_SUPPORT),y)
	DEFINES += -DMQTT=1
endif

ifeq ($(XY_PING_SUPPORT),y)
	DEFINES += -DXY_PING=1
endif

ifeq ($(XY_PERF_SUPPORT),y)
	DEFINES += -DXY_PERF=1
endif

ifeq ($(DM_SUPPORT),y)
	DEFINES += -DXY_DM=1
endif

ifeq ($(XY_DTLS_SUPPORT),y)
	DEFINES += -DXY_DTLS=1
	DEFINES += -DMBEDTLS_CONFIG_FILE=\<los_mbedtls_config.h\>
endif

ifeq ($(XY_WIRESHARK_SUPPORT),y)
	DEFINES += -DXY_WIRESHARK=1
endif

ifeq ($(DEMO_SUPPORT),y)
	DEFINES += -DDEMO_TEST=1
endif

ifeq ($(BLE_SUPPORT),y)
	DEFINES += -DBLE_ENABLE=1
else
	DEFINES += -DBLE_ENABLE=0
endif

ifeq ($(XY_FOTA_SUPPORT),y)
	DEFINES += -DXY_FOTA=1
endif

ifeq ($(ABUP_FOTA_SUPPORT),y)
	DEFINES += -DABUP_FOTA=1
endif

ifeq ($(HTTP_VER_SUPPORT),y)
	DEFINES += -DXY_HTTP=1
endif

ifeq ($(FLASH_4M),y)
	DEFINES += -DFLASH_GIGA_4M=1
endif

ifeq ($(XY_ASYNC_SOCKET_SUPPORT),y)
	DEFINES += -DASYNC_SKT_ENABLE=1
endif

ifeq ($(TRACEALYZER_SUPPORT),y)
	DEFINES += -DTRACEALYZER_ENABLE=1
endif


####################################################################################################
# export variable to use sub makefile
####################################################################################################
export DEFINES