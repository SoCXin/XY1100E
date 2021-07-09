####################################################################################################
# directory
####################################################################################################
SYSLIB_TOP_DIR := $(TOP_DIR)
SYSLIB_LIB_DIR := $(LIBRARY_DIR)
SYSLIB_SRC_DIR := $(SYSLIB_TOP_DIR)/SYSLIB
SYSLIB_OBJ_DIR := $(OBJECT_DIR)/SYSLIB


####################################################################################################
# library
####################################################################################################
LIB_SYSLIB_NAME = syslib
LIB_SYSLIB_SRC  = $(SYSLIB_LIB_DIR)/lib$(LIB_SYSLIB_NAME).a


####################################################################################################
# include other makefile to include source
####################################################################################################
-include $(SYSLIB_SRC_DIR)/at_cmd/at_cmd.mk
-include $(SYSLIB_SRC_DIR)/at_uart/at_uart.mk
-include $(SYSLIB_SRC_DIR)/diag_ctrl/diag_ctrl.mk
-include $(SYSLIB_SRC_DIR)/flash/flash.mk
-include $(SYSLIB_SRC_DIR)/lpm/lpm.mk
-include $(SYSLIB_SRC_DIR)/redirect/redirect.mk
-include $(SYSLIB_SRC_DIR)/rtc_tmr/rtc_tmr.mk
-include $(SYSLIB_SRC_DIR)/shm_msg/shm_msg.mk
-include $(SYSLIB_SRC_DIR)/sock_proxy/sock_proxy.mk
-include $(SYSLIB_SRC_DIR)/system/system.mk
-include $(SYSLIB_SRC_DIR)/test/test.mk


####################################################################################################
# SRCS_SYSLIB_RAM   SRCS_SYSLIB_FLASH   DEPS_PATH_SYSLIB
# these variable is defined and assignment in include makefile
####################################################################################################


####################################################################################################
# objects and depends
####################################################################################################
TMP_SYSLIB_OBJ  += $(subst .S,.o.ram,$(subst .s,.o.ram,$(subst .c,.o.ram,$(SRCS_SYSLIB_RAM))))
TMP_SYSLIB_OBJ  += $(subst .S,.o.flash,$(subst .s,.o.flash,$(subst .c,.o.flash,$(SRCS_SYSLIB_FLASH))))
SRCS_SYSLIB_OBJ  = $(subst $(SYSLIB_SRC_DIR),$(SYSLIB_OBJ_DIR),$(TMP_SYSLIB_OBJ))
SRCS_SYSLIB_OBJ := $(sort $(SRCS_SYSLIB_OBJ))

TMP_SYSLIB_EXCLUDE_LIB_OBJ += $(subst .S,.o.ram,$(subst .s,.o.ram,$(subst .c,.o.ram,$(SYSLIB_EXCLUDE_LIB_RAM))))
TMP_SYSLIB_EXCLUDE_LIB_OBJ += $(subst .S,.o.flash,$(subst .s,.o.flash,$(subst .c,.o.flash,$(SYSLIB_EXCLUDE_LIB_FLASH))))
SYSLIB_EXCLUDE_LIB_OBJ      = $(subst $(SYSLIB_SRC_DIR),$(SYSLIB_OBJ_DIR),$(TMP_SYSLIB_EXCLUDE_LIB_OBJ))
SYSLIB_EXCLUDE_LIB_OBJ     := $(sort $(SYSLIB_EXCLUDE_LIB_OBJ))

SRCS_SYSLIB_DEP += $(patsubst %.o.ram,%.d,$(patsubst %.o.flash,%.d,$(SRCS_SYSLIB_OBJ)))
SRCS_SYSLIB_DEP += $(patsubst %.o.ram,%.d,$(patsubst %.o.flash,%.d,$(SYSLIB_EXCLUDE_LIB_OBJ)))


####################################################################################################
# add to makefile calling, be included in other makefile
####################################################################################################
SRCS_DEPS_PATH += $(DEPS_PATH_SYSLIB)

ifeq ($(SYSLIB_LIBRARY),a)
SRCS_OBJ_FILES += $(SYSLIB_EXCLUDE_LIB_OBJ)
SRCS_OBJ_FILES += $(SRCS_SYSLIB_OBJ)
SRCS_LIBS_INC  += 
endif

ifeq ($(SYSLIB_LIBRARY),b)
SRCS_OBJ_FILES += $(SYSLIB_EXCLUDE_LIB_OBJ)
SRCS_LIBS_INC  += -l$(LIB_SYSLIB_NAME)
endif

ifeq ($(SYSLIB_LIBRARY),c)
SRCS_OBJ_FILES += $(SYSLIB_EXCLUDE_LIB_OBJ)
SRCS_LIBS_INC  += -l$(LIB_SYSLIB_NAME)
endif


####################################################################################################
# make target
####################################################################################################
MAIN_ALL += all_syslib
MAIN_CLEAN += clean_syslib

ifeq ($(SYSLIB_LIBRARY),a)
all_syslib   : generate_syslib_exclude_obj generate_syslib_obj
clean_syslib : clear_syslib_obj clear_syslib_lib
endif

ifeq ($(SYSLIB_LIBRARY),b)
all_syslib   : generate_syslib_exclude_obj generate_syslib_lib
clean_syslib : clear_syslib_obj clear_syslib_lib
endif

ifeq ($(SYSLIB_LIBRARY),c)
all_syslib   : generate_syslib_exclude_obj
clean_syslib : clear_syslib_srcs clear_syslib_obj
endif


generate_syslib_obj: $(SRCS_SYSLIB_OBJ)

generate_syslib_exclude_obj: $(SYSLIB_EXCLUDE_LIB_OBJ)

generate_syslib_lib: $(LIB_SYSLIB_SRC)

clear_syslib_srcs:
#	@echo "delete syslib source files..."
	@$(RM) $(SRCS_SYSLIB_RAM) $(SRCS_SYSLIB_FLASH)

clear_syslib_obj:
#	@echo "delete syslib objects..."
	@$(RM) $(SYSLIB_OBJ_DIR)

clear_syslib_lib:
#	@echo "delete SYSLIB library..."
	@$(RM) $(LIB_SYSLIB_SRC)


####################################################################################################
# foler definition, include public definition and private definition
####################################################################################################
SYSLIB_DEFINES += $(DEFINES) $(DEFINE_FOR_SYSLIB)


####################################################################################################
# every file's compiling rules
####################################################################################################
$(SYSLIB_OBJ_DIR)/%.o.ram: $(SYSLIB_SRC_DIR)/%.c
	@$(MKDIR) $(dir $@)
	@echo compiling $(subst $(SYSLIB_TOP_DIR)/,,$<)
	@$(CC) $(COMPILE_OPTION) $(COMP_DEPS_PATH) $(SYSLIB_DEFINES) $(GENNER_DEPS_RAM) -c -o $@ $<
	
$(SYSLIB_OBJ_DIR)/%.o.flash: $(SYSLIB_SRC_DIR)/%.c
	@$(MKDIR) $(dir $@)
	@echo compiling $(subst $(SYSLIB_TOP_DIR)/,,$<)
	@$(CC) $(COMPILE_OPTION) $(COMP_DEPS_PATH) $(SYSLIB_DEFINES) $(GENNER_DEPS_FLASH) -c -o $@ $<
	
$(SYSLIB_OBJ_DIR)/%.o.ram: $(SYSLIB_SRC_DIR)/%.s
	@$(MKDIR) $(dir $@)
	@echo assembling $(subst $(SYSLIB_TOP_DIR)/,,$<)
	@$(CC) $(COMPILE_OPTION) $(COMP_DEPS_PATH) $(SYSLIB_DEFINES) $(GENNER_DEPS_RAM) -c -o $@ $<
	
$(SYSLIB_OBJ_DIR)/%.o.flash: $(SYSLIB_SRC_DIR)/%.s
	@$(MKDIR) $(dir $@)
	@echo assembling $(subst $(SYSLIB_TOP_DIR)/,,$<)
	@$(CC) $(COMPILE_OPTION) $(COMP_DEPS_PATH) $(SYSLIB_DEFINES) $(GENNER_DEPS_FLASH) -c -o $@ $<

$(SYSLIB_OBJ_DIR)/%.o.ram : $(SYSLIB_SRC_DIR)/%.S
	@$(MKDIR) $(dir $@)
	@echo assembling $(subst $(SYSLIB_TOP_DIR)/,,$<)
	@$(CC) $(COMPILE_OPTION) $(COMP_DEPS_PATH) $(SYSLIB_DEFINES) $(GENNER_DEPS_RAM) -c -o $@ $<

$(SYSLIB_OBJ_DIR)/%.o.flash : $(SYSLIB_SRC_DIR)/%.S
	@$(MKDIR) $(dir $@)
	@echo assembling $(subst $(SYSLIB_TOP_DIR)/,,$<)
	@$(CC) $(COMPILE_OPTION) $(COMP_DEPS_PATH) $(SYSLIB_DEFINES) $(GENNER_DEPS_FLASH) -c -o $@ $<

# RANLIB: update the symbolic index table of the static library
$(LIB_SYSLIB_SRC) : $(SRCS_SYSLIB_OBJ)
	@$(MKDIR) $(dir $@)
	@echo "generating library syslib..."
	@$(AR) -cru  $@ $(SRCS_SYSLIB_OBJ)
	@$(RANLIB) $@


####################################################################################################
# include .d files, refactoring dependency rule for every file
####################################################################################################
-include $(SRCS_SYSLIB_DEP)