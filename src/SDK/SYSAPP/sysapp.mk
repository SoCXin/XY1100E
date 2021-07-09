####################################################################################################
# directory
####################################################################################################
SYSAPP_TOP_DIR := $(TOP_DIR)
SYSAPP_LIB_DIR := $(LIBRARY_DIR)
SYSAPP_SRC_DIR := $(SYSAPP_TOP_DIR)/SYSAPP
SYSAPP_OBJ_DIR := $(OBJECT_DIR)/SYSAPP


####################################################################################################
# library
####################################################################################################
LIB_SYSAPP_NAME = sysapp
LIB_SYSAPP_SRC  = $(SYSAPP_LIB_DIR)/lib$(LIB_SYSAPP_NAME).a


####################################################################################################
# include other makefile to include source
####################################################################################################
-include $(SYSAPP_SRC_DIR)/at_cmd/at_cmd.mk
-include $(SYSAPP_SRC_DIR)/at_ctrl/at_ctrl.mk
-include $(SYSAPP_SRC_DIR)/at_uart/at_uart.mk
-include $(SYSAPP_SRC_DIR)/diag_ctrl/diag_ctrl.mk
-include $(SYSAPP_SRC_DIR)/flash/flash.mk
-include $(SYSAPP_SRC_DIR)/lpm/lpm.mk
-include $(SYSAPP_SRC_DIR)/redirect/redirect.mk
-include $(SYSAPP_SRC_DIR)/rtc_tmr/rtc_tmr.mk
-include $(SYSAPP_SRC_DIR)/shm_msg/shm_msg.mk
-include $(SYSAPP_SRC_DIR)/sock_proxy/sock_proxy.mk
-include $(SYSAPP_SRC_DIR)/system/system.mk
-include $(SYSAPP_SRC_DIR)/test/test.mk


####################################################################################################
# SRCS_SYSAPP_RAM   SRCS_SYSAPP_FLASH   DEPS_PATH_SYSAPP
# these variable is defined and assignment in include makefile
####################################################################################################


####################################################################################################
# objects and depends
####################################################################################################
TMP_SYSAPP_OBJ  += $(subst .S,.o.ram,$(subst .s,.o.ram,$(subst .c,.o.ram,$(SRCS_SYSAPP_RAM))))
TMP_SYSAPP_OBJ  += $(subst .S,.o.flash,$(subst .s,.o.flash,$(subst .c,.o.flash,$(SRCS_SYSAPP_FLASH))))
SRCS_SYSAPP_OBJ  = $(subst $(SYSAPP_SRC_DIR),$(SYSAPP_OBJ_DIR),$(TMP_SYSAPP_OBJ))
SRCS_SYSAPP_OBJ := $(sort $(SRCS_SYSAPP_OBJ))

TMP_SYSAPP_EXCLUDE_LIB_OBJ += $(subst .S,.o.ram,$(subst .s,.o.ram,$(subst .c,.o.ram,$(SYSAPP_EXCLUDE_LIB_RAM))))
TMP_SYSAPP_EXCLUDE_LIB_OBJ += $(subst .S,.o.flash,$(subst .s,.o.flash,$(subst .c,.o.flash,$(SYSAPP_EXCLUDE_LIB_FLASH))))
SYSAPP_EXCLUDE_LIB_OBJ      = $(subst $(SYSAPP_SRC_DIR),$(SYSAPP_OBJ_DIR),$(TMP_SYSAPP_EXCLUDE_LIB_OBJ))
SYSAPP_EXCLUDE_LIB_OBJ     := $(sort $(SYSAPP_EXCLUDE_LIB_OBJ))

SRCS_SYSAPP_DEP += $(patsubst %.o.ram,%.d,$(patsubst %.o.flash,%.d,$(SRCS_SYSAPP_OBJ)))
SRCS_SYSAPP_DEP += $(patsubst %.o.ram,%.d,$(patsubst %.o.flash,%.d,$(SYSAPP_EXCLUDE_LIB_OBJ)))


####################################################################################################
# add to makefile calling, be included in other makefile
####################################################################################################
SRCS_DEPS_PATH += $(DEPS_PATH_SYSAPP)

ifeq ($(SYSAPP_LIBRARY),a)
SRCS_OBJ_FILES += $(SYSAPP_EXCLUDE_LIB_OBJ)
SRCS_OBJ_FILES += $(SRCS_SYSAPP_OBJ)
SRCS_LIBS_INC  += 
endif

ifeq ($(SYSAPP_LIBRARY),b)
SRCS_OBJ_FILES += $(SYSAPP_EXCLUDE_LIB_OBJ)
SRCS_LIBS_INC  += -l$(LIB_SYSAPP_NAME)
endif

ifeq ($(SYSAPP_LIBRARY),c)
SRCS_OBJ_FILES += $(SYSAPP_EXCLUDE_LIB_OBJ)
SRCS_LIBS_INC  += -l$(LIB_SYSAPP_NAME)
endif


####################################################################################################
# make target
####################################################################################################
MAIN_ALL += all_sysapp
MAIN_CLEAN += clean_sysapp

ifeq ($(SYSAPP_LIBRARY),a)
all_sysapp   : generate_sysapp_exclude_obj generate_sysapp_obj
clean_sysapp : clear_sysapp_obj clear_sysapp_lib
endif

ifeq ($(SYSAPP_LIBRARY),b)
all_sysapp   : generate_sysapp_exclude_obj generate_sysapp_lib
clean_sysapp : clear_sysapp_obj clear_sysapp_lib
endif

ifeq ($(SYSAPP_LIBRARY),c)
all_sysapp   : generate_sysapp_exclude_obj
clean_sysapp : clear_sysapp_srcs clear_sysapp_obj
endif


generate_sysapp_obj: $(SRCS_SYSAPP_OBJ)

generate_sysapp_exclude_obj: $(SYSAPP_EXCLUDE_LIB_OBJ)

generate_sysapp_lib: $(LIB_SYSAPP_SRC)

clear_sysapp_srcs:
#	@echo "delete sysapp source files..."
	@$(RM) $(SRCS_SYSAPP_RAM) $(SRCS_SYSAPP_FLASH)

clear_sysapp_obj:
#	@echo "delete sysapp objects..."
	@$(RM) $(SYSAPP_OBJ_DIR)

clear_sysapp_lib:
#	@echo "delete sysapp library..."
	@$(RM) $(LIB_SYSAPP_SRC)


####################################################################################################
# foler definition, include public definition and private definition
####################################################################################################
SYSAPP_DEFINES += $(DEFINES) $(DEFINE_FOR_SYSAPP)


####################################################################################################
# every file's compiling rules
####################################################################################################
$(SYSAPP_OBJ_DIR)/%.o.ram: $(SYSAPP_SRC_DIR)/%.c
	@$(MKDIR) $(dir $@)
	@echo compiling $(subst $(SYSAPP_TOP_DIR)/,,$<)
	@$(CC) $(COMPILE_OPTION) $(COMP_DEPS_PATH) $(SYSAPP_DEFINES) $(GENNER_DEPS_RAM) -c -o $@ $<
	
$(SYSAPP_OBJ_DIR)/%.o.flash: $(SYSAPP_SRC_DIR)/%.c
	@$(MKDIR) $(dir $@)
	@echo compiling $(subst $(SYSAPP_TOP_DIR)/,,$<)
	@$(CC) $(COMPILE_OPTION) $(COMP_DEPS_PATH) $(SYSAPP_DEFINES) $(GENNER_DEPS_FLASH) -c -o $@ $<
	
$(SYSAPP_OBJ_DIR)/%.o.ram: $(SYSAPP_SRC_DIR)/%.s
	@$(MKDIR) $(dir $@)
	@echo assembling $(subst $(SYSAPP_TOP_DIR)/,,$<)
	@$(CC) $(COMPILE_OPTION) $(COMP_DEPS_PATH) $(SYSAPP_DEFINES) $(GENNER_DEPS_RAM) -c -o $@ $<
	
$(SYSAPP_OBJ_DIR)/%.o.flash: $(SYSAPP_SRC_DIR)/%.s
	@$(MKDIR) $(dir $@)
	@echo assembling $(subst $(SYSAPP_TOP_DIR)/,,$<)
	@$(CC) $(COMPILE_OPTION) $(COMP_DEPS_PATH) $(SYSAPP_DEFINES) $(GENNER_DEPS_FLASH) -c -o $@ $<

$(SYSAPP_OBJ_DIR)/%.o.ram : $(SYSAPP_SRC_DIR)/%.S
	@$(MKDIR) $(dir $@)
	@echo assembling $(subst $(SYSAPP_TOP_DIR)/,,$<)
	@$(CC) $(COMPILE_OPTION) $(COMP_DEPS_PATH) $(SYSAPP_DEFINES) $(GENNER_DEPS_RAM) -c -o $@ $<

$(SYSAPP_OBJ_DIR)/%.o.flash : $(SYSAPP_SRC_DIR)/%.S
	@$(MKDIR) $(dir $@)
	@echo assembling $(subst $(SYSAPP_TOP_DIR)/,,$<)
	@$(CC) $(COMPILE_OPTION) $(COMP_DEPS_PATH) $(SYSAPP_DEFINES) $(GENNER_DEPS_FLASH) -c -o $@ $<

# RANLIB: update the symbolic index table of the static library
$(LIB_SYSAPP_SRC) : $(SRCS_SYSAPP_OBJ)
	@$(MKDIR) $(dir $@)
	@echo "generating library sysapp..."
	@$(AR) -cru  $@ $(SRCS_SYSAPP_OBJ)
	@$(RANLIB) $@


####################################################################################################
# include .d files, refactoring dependency rule for every file
####################################################################################################
-include $(SRCS_SYSAPP_DEP)