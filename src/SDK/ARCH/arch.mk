####################################################################################################
# directory, all the file in this directory
####################################################################################################
ARCH_TOP_DIR := $(TOP_DIR)
ARCH_LIB_DIR := $(LIBRARY_DIR)
ARCH_SRC_DIR := $(ARCH_TOP_DIR)/ARCH
ARCH_OBJ_DIR := $(OBJECT_DIR)/ARCH


####################################################################################################
# library
####################################################################################################
LIB_ARCH_NAME = arch
LIB_ARCH_SRC  = $(ARCH_LIB_DIR)/lib$(LIB_ARCH_NAME).a


####################################################################################################
# include other makefile to include source
####################################################################################################
-include $(ARCH_SRC_DIR)/assert/assert.mk
-include $(ARCH_SRC_DIR)/core_cm3/core_cm3.mk
-include $(ARCH_SRC_DIR)/nvic/nvic.mk
-include $(ARCH_SRC_DIR)/startup/startup.mk


####################################################################################################
# ARCH_EXCLUDE_LIB   SRCS_ARCH_FILE   DEPS_PATH_ARCH
# these variable is defined and assignment in include makefile
####################################################################################################


####################################################################################################
# objects and depends
####################################################################################################
TMP_ARCH_OBJ  += $(subst .S,.o.ram,$(subst .s,.o.ram,$(subst .c,.o.ram,$(SRCS_ARCH_RAM))))
TMP_ARCH_OBJ  += $(subst .S,.o.flash,$(subst .s,.o.flash,$(subst .c,.o.flash,$(SRCS_ARCH_FLASH))))
SRCS_ARCH_OBJ  = $(subst $(ARCH_SRC_DIR),$(ARCH_OBJ_DIR),$(TMP_ARCH_OBJ))
SRCS_ARCH_OBJ := $(sort $(SRCS_ARCH_OBJ))

TMP_ARCH_EXCLUDE_LIB_OBJ += $(subst .S,.o.ram,$(subst .s,.o.ram,$(subst .c,.o.ram,$(ARCH_EXCLUDE_LIB_RAM))))
TMP_ARCH_EXCLUDE_LIB_OBJ += $(subst .S,.o.flash,$(subst .s,.o.flash,$(subst .c,.o.flash,$(ARCH_EXCLUDE_LIB_FLASH))))
ARCH_EXCLUDE_LIB_OBJ      = $(subst $(ARCH_SRC_DIR),$(ARCH_OBJ_DIR),$(TMP_ARCH_EXCLUDE_LIB_OBJ))
ARCH_EXCLUDE_LIB_OBJ     := $(sort $(ARCH_EXCLUDE_LIB_OBJ))

SRCS_ARCH_DEP += $(patsubst %.o.ram,%.d,$(patsubst %.o.flash,%.d,$(SRCS_ARCH_OBJ)))
SRCS_ARCH_DEP += $(patsubst %.o.ram,%.d,$(patsubst %.o.flash,%.d,$(ARCH_EXCLUDE_LIB_OBJ)))


####################################################################################################
# add to makefile calling, be included in other makefile
####################################################################################################
SRCS_DEPS_PATH += $(DEPS_PATH_ARCH)

ifeq ($(ARCH_LIBRARY),a)
SRCS_OBJ_FILES += $(ARCH_EXCLUDE_LIB_OBJ)
SRCS_OBJ_FILES += $(SRCS_ARCH_OBJ)
SRCS_LIBS_INC  += 
endif

ifeq ($(ARCH_LIBRARY),b)
SRCS_OBJ_FILES += $(ARCH_EXCLUDE_LIB_OBJ)
SRCS_LIBS_INC  += -l$(LIB_ARCH_NAME)
endif

ifeq ($(ARCH_LIBRARY),c)
SRCS_OBJ_FILES += $(ARCH_EXCLUDE_LIB_OBJ)
SRCS_LIBS_INC  += -l$(LIB_ARCH_NAME)
endif


####################################################################################################
# make target
####################################################################################################
MAIN_ALL += all_arch
MAIN_CLEAN += clean_arch

ifeq ($(ARCH_LIBRARY),a)
all_arch   : generate_arch_exclude_obj   generate_arch_obj
clean_arch : clear_arch_obj clear_arch_lib
endif

ifeq ($(ARCH_LIBRARY),b)
all_arch   : generate_arch_exclude_obj   generate_arch_lib
clean_arch : clear_arch_obj clear_arch_lib
endif

ifeq ($(ARCH_LIBRARY),c)
all_arch   : generate_arch_exclude_obj
clean_arch : clear_arch_srcs clear_arch_obj
endif


generate_arch_obj: $(SRCS_ARCH_OBJ)

generate_arch_exclude_obj: $(ARCH_EXCLUDE_LIB_OBJ)

generate_arch_lib: $(LIB_ARCH_SRC)

clear_arch_srcs:
#	@echo "delete arch source files..."
	@$(RM) $(SRCS_ARCH_RAM) $(SRCS_ARCH_FLASH)

clear_arch_obj:
#	@echo "delete arch objects..."
	@$(RM) $(ARCH_OBJ_DIR)

clear_arch_lib:
#	@echo "delete arch library..."
	@$(RM) $(LIB_ARCH_SRC)


####################################################################################################
# foler definition, include public definition and private definition
####################################################################################################
ARCH_DEFINES += $(DEFINES) $(DEFINE_FOR_ARCH)


####################################################################################################
# every file's compiling rules
####################################################################################################
$(ARCH_OBJ_DIR)/%.o.ram: $(ARCH_SRC_DIR)/%.c
	@$(MKDIR) $(dir $@)
	@echo compiling $(subst $(ARCH_TOP_DIR)/,,$<)
	@$(CC) $(COMPILE_OPTION) $(COMP_DEPS_PATH) $(GENNER_DEPS_RAM) $(ARCH_DEFINES) -c -o $@ $<
	
$(ARCH_OBJ_DIR)/%.o.flash: $(ARCH_SRC_DIR)/%.c
	@$(MKDIR) $(dir $@)
	@echo compiling $(subst $(ARCH_TOP_DIR)/,,$<)
	@$(CC) $(COMPILE_OPTION) $(COMP_DEPS_PATH) $(GENNER_DEPS_FLASH) $(ARCH_DEFINES) -c -o $@ $<
	
$(ARCH_OBJ_DIR)/%.o.ram: $(ARCH_SRC_DIR)/%.s
	@$(MKDIR) $(dir $@)
	@echo assembling $(subst $(ARCH_TOP_DIR)/,,$<)
	@$(CC) $(COMPILE_OPTION) $(COMP_DEPS_PATH) $(GENNER_DEPS_RAM) $(ARCH_DEFINES) -c -o $@ $<
	
$(ARCH_OBJ_DIR)/%.o.flash: $(ARCH_SRC_DIR)/%.s
	@$(MKDIR) $(dir $@)
	@echo assembling $(subst $(ARCH_TOP_DIR)/,,$<)
	@$(CC) $(COMPILE_OPTION) $(COMP_DEPS_PATH) $(GENNER_DEPS_FLASH) $(ARCH_DEFINES) -c -o $@ $<

$(ARCH_OBJ_DIR)/%.o.ram : $(ARCH_SRC_DIR)/%.S
	@$(MKDIR) $(dir $@)
	@echo assembling $(subst $(ARCH_TOP_DIR)/,,$<)
	@$(CC) $(COMPILE_OPTION) $(COMP_DEPS_PATH) $(GENNER_DEPS_RAM) $(ARCH_DEFINES) -c -o $@ $<

$(ARCH_OBJ_DIR)/%.o.flash : $(ARCH_SRC_DIR)/%.S
	@$(MKDIR) $(dir $@)
	@echo assembling $(subst $(ARCH_TOP_DIR)/,,$<)
	@$(CC) $(COMPILE_OPTION) $(COMP_DEPS_PATH) $(GENNER_DEPS_FLASH) $(ARCH_DEFINES) -c -o $@ $<

# RANLIB: update the symbolic index table of the static library
$(LIB_ARCH_SRC) : $(SRCS_ARCH_OBJ)
	@$(MKDIR) $(dir $@)
	@echo "generating library arch..."
	@$(AR) -cru  $@ $(SRCS_ARCH_OBJ)
	@$(RANLIB) $@


####################################################################################################
# include .d files, refactoring dependency rule for every file
####################################################################################################
-include $(SRCS_ARCH_DEP)