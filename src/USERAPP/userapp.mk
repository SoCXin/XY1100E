####################################################################################################
# directory, all the file of sys_app in this directory
####################################################################################################
USERAPP_TOP_DIR := $(TOP_DIR)
USERAPP_LIB_DIR := $(LIBRARY_DIR)
USERAPP_SRC_DIR := $(USERAPP_TOP_DIR)/USERAPP
USERAPP_OBJ_DIR := $(OBJECT_DIR)/USERAPP


####################################################################################################
# library
####################################################################################################
LIB_USERAPP_NAME = userapp
LIB_USERAPP_SRC  = $(USERAPP_LIB_DIR)/lib$(LIB_USERAPP_NAME).a


####################################################################################################
# include other makefile to include source
####################################################################################################
-include $(USERAPP_SRC_DIR)/basic/basic.mk
-include $(USERAPP_SRC_DIR)/examples/examples.mk

####################################################################################################
# SRCS_USERAPP_RAM   SRCS_USERAPP_FLASH   DEPS_PATH_USERAPP
# these variable is defined and assignment in include makefile
####################################################################################################


####################################################################################################
# objects and depends
####################################################################################################
TMP_USERAPP_OBJ  += $(subst .S,.o.ram,$(subst .s,.o.ram,$(subst .c,.o.ram,$(SRCS_USERAPP_RAM))))
TMP_USERAPP_OBJ  += $(subst .S,.o.flash,$(subst .s,.o.flash,$(subst .c,.o.flash,$(SRCS_USERAPP_FLASH))))
SRCS_USERAPP_OBJ  = $(subst $(USERAPP_SRC_DIR),$(USERAPP_OBJ_DIR),$(TMP_USERAPP_OBJ))
SRCS_USERAPP_OBJ := $(sort $(SRCS_USERAPP_OBJ))

TMP_USERAPP_EXCLUDE_LIB_OBJ += $(subst .S,.o.ram,$(subst .s,.o.ram,$(subst .c,.o.ram,$(USERAPP_EXCLUDE_LIB_RAM))))
TMP_USERAPP_EXCLUDE_LIB_OBJ += $(subst .S,.o.flash,$(subst .s,.o.flash,$(subst .c,.o.flash,$(USERAPP_EXCLUDE_LIB_FLASH))))
USERAPP_EXCLUDE_LIB_OBJ      = $(subst $(USERAPP_SRC_DIR),$(USERAPP_OBJ_DIR),$(TMP_USERAPP_EXCLUDE_LIB_OBJ))
USERAPP_EXCLUDE_LIB_OBJ     := $(sort $(USERAPP_EXCLUDE_LIB_OBJ))

SRCS_USERAPP_DEP += $(patsubst %.o.ram,%.d,$(patsubst %.o.flash,%.d,$(SRCS_USERAPP_OBJ)))
SRCS_USERAPP_DEP += $(patsubst %.o.ram,%.d,$(patsubst %.o.flash,%.d,$(USERAPP_EXCLUDE_LIB_OBJ)))


####################################################################################################
# add to makefile calling, be included in other makefile
####################################################################################################
SRCS_DEPS_PATH += $(DEPS_PATH_USERAPP)

ifeq ($(USERAPP_LIBRARY),a)
SRCS_OBJ_FILES += $(USERAPP_EXCLUDE_LIB_OBJ)
SRCS_OBJ_FILES += $(SRCS_USERAPP_OBJ)
SRCS_LIBS_INC  += 
endif

ifeq ($(USERAPP_LIBRARY),b)
SRCS_OBJ_FILES += $(USERAPP_EXCLUDE_LIB_OBJ)
SRCS_LIBS_INC  += -l$(LIB_USERAPP_NAME)
endif

ifeq ($(USERAPP_LIBRARY),c)
SRCS_OBJ_FILES += $(USERAPP_EXCLUDE_LIB_OBJ)
SRCS_LIBS_INC  += -l$(LIB_USERAPP_NAME)
endif


####################################################################################################
# make target
####################################################################################################
MAIN_ALL += all_userapp
MAIN_CLEAN += clean_userapp

ifeq ($(USERAPP_LIBRARY),a)
all_userapp   : generate_userapp_exclude_obj generate_userapp_obj
clean_userapp : clear_userapp_obj clear_userapp_lib
endif

ifeq ($(USERAPP_LIBRARY),b)
all_userapp   : generate_userapp_exclude_obj generate_userapp_lib
clean_userapp : clear_userapp_obj clear_userapp_lib
endif

ifeq ($(USERAPP_LIBRARY),c)
all_userapp   : generate_userapp_exclude_obj
clean_userapp : clear_userapp_srcs clear_userapp_obj
endif


generate_userapp_obj: $(SRCS_USERAPP_OBJ)

generate_userapp_exclude_obj: $(USERAPP_EXCLUDE_LIB_OBJ)

generate_userapp_lib: $(LIB_USERAPP_SRC)

clear_userapp_srcs:
#	@echo "delete userapp source files..."
	@$(RM) $(SRCS_USERAPP_RAM) $(SRCS_USERAPP_FLASH)

clear_userapp_obj:
#	@echo "delete userapp objects..."
	@$(RM) $(USERAPP_OBJ_DIR)

clear_userapp_lib:
#	@echo "delete userapp library..."
	@$(RM) $(LIB_USERAPP_SRC)


####################################################################################################
# foler definition, include public definition and private definition
####################################################################################################
USERAPP_DEFINES += $(DEFINES) $(DEFINE_FOR_USERAPP)


####################################################################################################
# every file's compiling rules
####################################################################################################
$(USERAPP_OBJ_DIR)/%.o.ram: $(USERAPP_SRC_DIR)/%.c
	@$(MKDIR) $(dir $@)
	@echo compiling $(subst $(USERAPP_TOP_DIR)/,,$<)
	@$(CC) $(COMPILE_OPTION) $(COMP_DEPS_PATH) $(USERAPP_DEFINES) $(GENNER_DEPS_RAM) -c -o $@ $<
	
$(USERAPP_OBJ_DIR)/%.o.flash: $(USERAPP_SRC_DIR)/%.c
	@$(MKDIR) $(dir $@)
	@echo compiling $(subst $(USERAPP_TOP_DIR)/,,$<)
	@$(CC) $(COMPILE_OPTION) $(COMP_DEPS_PATH) $(USERAPP_DEFINES) $(GENNER_DEPS_FLASH) -c -o $@ $<
	
$(USERAPP_OBJ_DIR)/%.o.ram: $(USERAPP_SRC_DIR)/%.s
	@$(MKDIR) $(dir $@)
	@echo assembling $(subst $(USERAPP_TOP_DIR)/,,$<)
	@$(CC) $(COMPILE_OPTION) $(COMP_DEPS_PATH) $(USERAPP_DEFINES) $(GENNER_DEPS_RAM) -c -o $@ $<
	
$(USERAPP_OBJ_DIR)/%.o.flash: $(USERAPP_SRC_DIR)/%.s
	@$(MKDIR) $(dir $@)
	@echo assembling $(subst $(USERAPP_TOP_DIR)/,,$<)
	@$(CC) $(COMPILE_OPTION) $(COMP_DEPS_PATH) $(USERAPP_DEFINES) $(GENNER_DEPS_FLASH) -c -o $@ $<

$(USERAPP_OBJ_DIR)/%.o.ram : $(USERAPP_SRC_DIR)/%.S
	@$(MKDIR) $(dir $@)
	@echo assembling $(subst $(USERAPP_TOP_DIR)/,,$<)
	@$(CC) $(COMPILE_OPTION) $(COMP_DEPS_PATH) $(USERAPP_DEFINES) $(GENNER_DEPS_RAM) -c -o $@ $<

$(USERAPP_OBJ_DIR)/%.o.flash : $(USERAPP_SRC_DIR)/%.S
	@$(MKDIR) $(dir $@)
	@echo assembling $(subst $(USERAPP_TOP_DIR)/,,$<)
	@$(CC) $(COMPILE_OPTION) $(COMP_DEPS_PATH) $(USERAPP_DEFINES) $(GENNER_DEPS_FLASH) -c -o $@ $<

# RANLIB: update the symbolic index table of the static library
$(LIB_USERAPP_SRC) : $(SRCS_USERAPP_OBJ)
	@$(MKDIR) $(dir $@)
	@echo "generating library userapp..."
	@$(AR) -cru  $@ $(SRCS_USERAPP_OBJ)
	@$(RANLIB) $@


####################################################################################################
# include .d files, refactoring dependency rule for every file
####################################################################################################
-include $(SRCS_USERAPP_DEP)