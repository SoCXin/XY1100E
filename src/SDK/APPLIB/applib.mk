####################################################################################################
# directory, all the file in this directory
####################################################################################################
APPLIB_TOP_DIR := $(TOP_DIR)
APPLIB_LIB_DIR := $(LIBRARY_DIR)
APPLIB_SRC_DIR := $(APPLIB_TOP_DIR)/APPLIB
APPLIB_OBJ_DIR := $(OBJECT_DIR)/APPLIB


####################################################################################################
# library
####################################################################################################
LIB_APPLIB_NAME = applib
LIB_APPLIB_SRC  = $(APPLIB_LIB_DIR)/lib$(LIB_APPLIB_NAME).a


####################################################################################################
# include other makefile to include source
####################################################################################################
-include $(APPLIB_SRC_DIR)/cloud/cloud.mk
-include $(APPLIB_SRC_DIR)/lwip/lwip.mk
-include $(APPLIB_SRC_DIR)/xy_ps/xy_ps.mk

ifeq ($(XY_PING_SUPPORT),y)
-include $(APPLIB_SRC_DIR)/ping/ping.mk
endif

ifeq ($(XY_WIRESHARK_SUPPORT),y)
-include $(APPLIB_SRC_DIR)/wireshark/wireshark.mk
endif

-include $(APPLIB_SRC_DIR)/xy_fota/xy_fota.mk

ifeq ($(XY_PERF_SUPPORT),y)
-include $(APPLIB_SRC_DIR)/perf/xyperf.mk
endif

ifeq ($(AT_SOCKET_SUPPORT),y)
-include $(APPLIB_SRC_DIR)/at_socket/at_socket.mk
endif

ifeq ($(WAKAAMA_SUPPORT),y)
-include $(APPLIB_SRC_DIR)/wakaama/wakaama.mk
endif

ifeq ($(LIBCOAP_SUPPORT),y)
-include $(APPLIB_SRC_DIR)/libcoap/libcoap.mk
endif

ifeq ($(MQTT_SUPPORT),y)
-include $(APPLIB_SRC_DIR)/mqtt/mqtt.mk
endif

ifeq ($(XY_DTLS_SUPPORT),y)
-include $(APPLIB_SRC_DIR)/Dtls/dtls.mk
endif

ifeq ($(HTTP_VER_SUPPORT),y)
-include $(APPLIB_SRC_DIR)/http/http.mk
endif

ifeq ($(CJSON_SUPPORT),y)
-include $(APPLIB_SRC_DIR)/cJSON/cjson.mk
endif

ifeq ($(DM_SUPPORT),y)
-include $(APPLIB_SRC_DIR)/dm/dm.mk
endif

ifeq ($(ABUP_FOTA_SUPPORT),y)
-include $(APPLIB_SRC_DIR)/abup/abup.mk
endif

ifeq ($(XY_ASYNC_SOCKET_SUPPORT),y)
-include $(APPLIB_SRC_DIR)/xy_async_socket/xy_async_socket.mk
endif

####################################################################################################
# APPLIB_EXCLUDE_LIB   SRCS_APPLIB_FILE   DEPS_PATH_APPLIB
# these variable is defined and assignment in include makefile
####################################################################################################


####################################################################################################
# objects and depends
####################################################################################################
TMP_APPLIB_OBJ  += $(subst .S,.o.ram,$(subst .s,.o.ram,$(subst .c,.o.ram,$(SRCS_APPLIB_RAM))))
TMP_APPLIB_OBJ  += $(subst .S,.o.flash,$(subst .s,.o.flash,$(subst .c,.o.flash,$(SRCS_APPLIB_FLASH))))
SRCS_APPLIB_OBJ  = $(subst $(APPLIB_SRC_DIR),$(APPLIB_OBJ_DIR),$(TMP_APPLIB_OBJ))
SRCS_APPLIB_OBJ := $(sort $(SRCS_APPLIB_OBJ))

TMP_APPLIB_EXCLUDE_LIB_OBJ += $(subst .S,.o.ram,$(subst .s,.o.ram,$(subst .c,.o.ram,$(APPLIB_EXCLUDE_LIB_RAM))))
TMP_APPLIB_EXCLUDE_LIB_OBJ += $(subst .S,.o.flash,$(subst .s,.o.flash,$(subst .c,.o.flash,$(APPLIB_EXCLUDE_LIB_FLASH))))
APPLIB_EXCLUDE_LIB_OBJ      = $(subst $(APPLIB_SRC_DIR),$(APPLIB_OBJ_DIR),$(TMP_APPLIB_EXCLUDE_LIB_OBJ))
APPLIB_EXCLUDE_LIB_OBJ     := $(sort $(APPLIB_EXCLUDE_LIB_OBJ))

SRCS_APPLIB_DEP += $(patsubst %.o.ram,%.d,$(patsubst %.o.flash,%.d,$(SRCS_APPLIB_OBJ)))
SRCS_APPLIB_DEP += $(patsubst %.o.ram,%.d,$(patsubst %.o.flash,%.d,$(APPLIB_EXCLUDE_LIB_OBJ)))


####################################################################################################
# add to makefile calling, be included in other makefile
####################################################################################################
SRCS_DEPS_PATH += $(DEPS_PATH_APPLIB)

ifeq ($(APPLIB_LIBRARY),a)
SRCS_OBJ_FILES += $(APPLIB_EXCLUDE_LIB_OBJ)
SRCS_OBJ_FILES += $(SRCS_APPLIB_OBJ)
SRCS_LIBS_INC  += 
endif

ifeq ($(APPLIB_LIBRARY),b)
SRCS_OBJ_FILES += $(APPLIB_EXCLUDE_LIB_OBJ)
SRCS_LIBS_INC  += -l$(LIB_APPLIB_NAME)
endif

ifeq ($(APPLIB_LIBRARY),c)
SRCS_OBJ_FILES += $(APPLIB_EXCLUDE_LIB_OBJ)
SRCS_LIBS_INC  += -l$(LIB_APPLIB_NAME)
endif


####################################################################################################
# make target
####################################################################################################
MAIN_ALL += all_applib
MAIN_CLEAN += clean_applib

ifeq ($(APPLIB_LIBRARY),a)
all_applib   : generate_applib_exclude_obj   generate_applib_obj
clean_applib : clear_applib_obj   clear_applib_lib
endif

ifeq ($(APPLIB_LIBRARY),b)
all_applib   : generate_applib_exclude_obj   generate_applib_lib
clean_applib : clear_applib_obj   clear_applib_lib
endif

ifeq ($(APPLIB_LIBRARY),c)
all_applib   : generate_applib_exclude_obj
clean_applib : clear_applib_srcs   clear_applib_obj
endif


generate_applib_obj: $(SRCS_APPLIB_OBJ)

generate_applib_exclude_obj: $(APPLIB_EXCLUDE_LIB_OBJ)

generate_applib_lib: $(LIB_APPLIB_SRC)

clear_applib_srcs:
#	@echo "delete applib source files..."
	@$(RM) $(SRCS_APPLIB_RAM) $(SRCS_APPLIB_FLASH)

clear_applib_obj:
#	@echo "delete applib objects..."
	@$(RM) $(APPLIB_OBJ_DIR)

clear_applib_lib:
#	@echo "delete applib library..."
	@$(RM) $(LIB_APPLIB_SRC)


####################################################################################################
# foler definition, include public definition and private definition
####################################################################################################
APPLIB_DEFINES += $(DEFINES) $(DEFINE_FOR_APPLIB)


####################################################################################################
# every file's compiling rules
####################################################################################################
$(APPLIB_OBJ_DIR)/%.o.ram: $(APPLIB_SRC_DIR)/%.c
	@$(MKDIR) $(dir $@)
	@echo compiling $(subst $(APPLIB_TOP_DIR)/,,$<)
	@$(CC) -w $(COMPILE_OPTION) $(COMP_DEPS_PATH) $(GENNER_DEPS_RAM) $(APPLIB_DEFINES) -c -o $@ $<
	
$(APPLIB_OBJ_DIR)/%.o.flash: $(APPLIB_SRC_DIR)/%.c
	@$(MKDIR) $(dir $@)
	@echo compiling $(subst $(APPLIB_TOP_DIR)/,,$<)
	@$(CC) -w $(COMPILE_OPTION) $(COMP_DEPS_PATH) $(GENNER_DEPS_FLASH) $(APPLIB_DEFINES) -c -o $@ $<
	
$(APPLIB_OBJ_DIR)/%.o.ram: $(APPLIB_SRC_DIR)/%.s
	@$(MKDIR) $(dir $@)
	@echo assembling $(subst $(APPLIB_TOP_DIR)/,,$<)
	@$(CC) $(COMPILE_OPTION) $(COMP_DEPS_PATH) $(GENNER_DEPS_RAM) $(APPLIB_DEFINES) -c -o $@ $<
	
$(APPLIB_OBJ_DIR)/%.o.flash: $(APPLIB_SRC_DIR)/%.s
	@$(MKDIR) $(dir $@)
	@echo assembling $(subst $(APPLIB_TOP_DIR)/,,$<)
	@$(CC) $(COMPILE_OPTION) $(COMP_DEPS_PATH) $(GENNER_DEPS_FLASH) $(APPLIB_DEFINES) -c -o $@ $<

$(APPLIB_OBJ_DIR)/%.o.ram : $(APPLIB_SRC_DIR)/%.S
	@$(MKDIR) $(dir $@)
	@echo assembling $(subst $(APPLIB_TOP_DIR)/,,$<)
	@$(CC) $(COMPILE_OPTION) $(COMP_DEPS_PATH) $(GENNER_DEPS_RAM) $(APPLIB_DEFINES) -c -o $@ $<

$(APPLIB_OBJ_DIR)/%.o.flash : $(APPLIB_SRC_DIR)/%.S
	@$(MKDIR) $(dir $@)
	@echo assembling $(subst $(APPLIB_TOP_DIR)/,,$<)
	@$(CC) $(COMPILE_OPTION) $(COMP_DEPS_PATH) $(GENNER_DEPS_FLASH) $(APPLIB_DEFINES) -c -o $@ $<

# RANLIB: update the symbolic index table of the static library
$(LIB_APPLIB_SRC) : $(SRCS_APPLIB_OBJ)
	@$(MKDIR) $(dir $@)
	@echo "generating library applib..."
	@$(AR) -cru  $@ $(SRCS_APPLIB_OBJ)
	@$(RANLIB) $@


####################################################################################################
# include .d files, refactoring dependency rule for every file
####################################################################################################
-include $(SRCS_APPLIB_DEP)