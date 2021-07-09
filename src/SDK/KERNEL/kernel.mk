####################################################################################################
# directory, all the file in this directory
####################################################################################################
KERNEL_TOP_DIR := $(TOP_DIR)
KERNEL_LIB_DIR := $(LIBRARY_DIR)
KERNEL_SRC_DIR := $(KERNEL_TOP_DIR)/KERNEL
KERNEL_OBJ_DIR := $(OBJECT_DIR)/KERNEL


####################################################################################################
# library
####################################################################################################
LIB_KERNEL_NAME = kernel
LIB_KERNEL_SRC  = $(KERNEL_LIB_DIR)/lib$(LIB_KERNEL_NAME).a


####################################################################################################
# include other makefile to include source
####################################################################################################
-include $(KERNEL_SRC_DIR)/OS/os.mk
-include $(KERNEL_SRC_DIR)/OSAL/osal.mk
-include $(KERNEL_SRC_DIR)/ZeusOS/ZeusOS.mk
ifeq ($(TRACEALYZER_SUPPORT),y)
-include $(KERNEL_SRC_DIR)/Tracealyzer/tracealyzer.mk
endif

####################################################################################################
# SRCS_KERNEL_RAM   SRCS_KERNEL_FLASH   DEPS_PATH_KERNEL
# these variable is defined and assignment in include makefile
####################################################################################################


####################################################################################################
# objects and depends
####################################################################################################
TMP_KERNEL_OBJ  += $(subst .S,.o.ram,$(subst .s,.o.ram,$(subst .c,.o.ram,$(SRCS_KERNEL_RAM))))
TMP_KERNEL_OBJ  += $(subst .S,.o.flash,$(subst .s,.o.flash,$(subst .c,.o.flash,$(SRCS_KERNEL_FLASH))))
SRCS_KERNEL_OBJ  = $(subst $(KERNEL_SRC_DIR),$(KERNEL_OBJ_DIR),$(TMP_KERNEL_OBJ))
SRCS_KERNEL_OBJ := $(sort $(SRCS_KERNEL_OBJ))

TMP_KERNEL_EXCLUDE_LIB_OBJ += $(subst .S,.o.ram,$(subst .s,.o.ram,$(subst .c,.o.ram,$(KERNEL_EXCLUDE_LIB_RAM))))
TMP_KERNEL_EXCLUDE_LIB_OBJ += $(subst .S,.o.flash,$(subst .s,.o.flash,$(subst .c,.o.flash,$(KERNEL_EXCLUDE_LIB_FLASH))))
KERNEL_EXCLUDE_LIB_OBJ      = $(subst $(KERNEL_SRC_DIR),$(KERNEL_OBJ_DIR),$(TMP_KERNEL_EXCLUDE_LIB_OBJ))
KERNEL_EXCLUDE_LIB_OBJ     := $(sort $(KERNEL_EXCLUDE_LIB_OBJ))

SRCS_KERNEL_DEP += $(patsubst %.o.ram,%.d,$(patsubst %.o.flash,%.d,$(SRCS_KERNEL_OBJ)))
SRCS_KERNEL_DEP += $(patsubst %.o.ram,%.d,$(patsubst %.o.flash,%.d,$(KERNEL_EXCLUDE_LIB_OBJ)))


####################################################################################################
# add to makefile calling, be included in other makefile
####################################################################################################
SRCS_DEPS_PATH += $(DEPS_PATH_KERNEL)

ifeq ($(KERNEL_LIBRARY),a)
SRCS_OBJ_FILES += $(KERNEL_EXCLUDE_LIB_OBJ)
SRCS_OBJ_FILES += $(SRCS_KERNEL_OBJ)
SRCS_LIBS_INC  += 
endif

ifeq ($(KERNEL_LIBRARY),b)
SRCS_OBJ_FILES += $(KERNEL_EXCLUDE_LIB_OBJ)
SRCS_LIBS_INC  += -l$(LIB_KERNEL_NAME)
endif

ifeq ($(KERNEL_LIBRARY),c)
SRCS_OBJ_FILES += $(KERNEL_EXCLUDE_LIB_OBJ)
SRCS_LIBS_INC  += -l$(LIB_KERNEL_NAME)
endif


####################################################################################################
# make target
####################################################################################################
MAIN_ALL += all_kernel
MAIN_CLEAN += clean_kernel

ifeq ($(KERNEL_LIBRARY),a)
all_kernel   : generate_kernel_exclude_obj generate_kernel_obj
clean_kernel : clear_kernel_obj   clear_kernel_lib
endif

ifeq ($(KERNEL_LIBRARY),b)
all_kernel   : generate_kernel_exclude_obj generate_kernel_lib
clean_kernel : clear_kernel_obj   clear_kernel_lib
endif

ifeq ($(KERNEL_LIBRARY),c)
all_kernel   : generate_kernel_exclude_obj
clean_kernel : clear_kernel_srcs   clear_kernel_obj
endif


generate_kernel_obj: $(SRCS_KERNEL_OBJ)

generate_kernel_exclude_obj: $(KERNEL_EXCLUDE_LIB_OBJ)

generate_kernel_lib: $(LIB_KERNEL_SRC)

clear_kernel_srcs:
#	@echo "delete kernel source files..."
	@$(RM) $(SRCS_KERNEL_RAM) $(SRCS_KERNEL_FLASH)

clear_kernel_obj:
#	@echo "delete kernel objects..."
	@$(RM) $(KERNEL_OBJ_DIR)

clear_kernel_lib:
#	@echo "delete kernel library..."
	@$(RM) $(LIB_KERNEL_SRC)


####################################################################################################
# foler definition, include public definition and private definition
####################################################################################################
KERNEL_DEFINES += $(DEFINES) $(DEFINE_FOR_KERNEL)


####################################################################################################
# every file's compiling rules
####################################################################################################
$(KERNEL_OBJ_DIR)/%.o.ram: $(KERNEL_SRC_DIR)/%.c
	@$(MKDIR) $(dir $@)
	@echo compiling $(subst $(KERNEL_TOP_DIR)/,,$<)
	@$(CC) $(COMPILE_OPTION) $(COMP_DEPS_PATH) $(KERNEL_DEFINES) $(GENNER_DEPS_RAM) -c -o $@ $<
	
$(KERNEL_OBJ_DIR)/%.o.flash: $(KERNEL_SRC_DIR)/%.c
	@$(MKDIR) $(dir $@)
	@echo compiling $(subst $(KERNEL_TOP_DIR)/,,$<)
	@$(CC) $(COMPILE_OPTION) $(COMP_DEPS_PATH) $(KERNEL_DEFINES) $(GENNER_DEPS_FLASH) -c -o $@ $<
	
$(KERNEL_OBJ_DIR)/%.o.ram: $(KERNEL_SRC_DIR)/%.s
	@$(MKDIR) $(dir $@)
	@echo assembling $(subst $(KERNEL_TOP_DIR)/,,$<)
	@$(CC) $(COMPILE_OPTION) $(COMP_DEPS_PATH) $(KERNEL_DEFINES) $(GENNER_DEPS_RAM) -c -o $@ $<
	
$(KERNEL_OBJ_DIR)/%.o.flash: $(KERNEL_SRC_DIR)/%.s
	@$(MKDIR) $(dir $@)
	@echo assembling $(subst $(KERNEL_TOP_DIR)/,,$<)
	@$(CC) $(COMPILE_OPTION) $(COMP_DEPS_PATH) $(KERNEL_DEFINES) $(GENNER_DEPS_FLASH) -c -o $@ $<

$(KERNEL_OBJ_DIR)/%.o.ram : $(KERNEL_SRC_DIR)/%.S
	@$(MKDIR) $(dir $@)
	@echo assembling $(subst $(KERNEL_TOP_DIR)/,,$<)
	@$(CC) $(COMPILE_OPTION) $(COMP_DEPS_PATH) $(KERNEL_DEFINES) $(GENNER_DEPS_RAM) -c -o $@ $<

$(KERNEL_OBJ_DIR)/%.o.flash : $(KERNEL_SRC_DIR)/%.S
	@$(MKDIR) $(dir $@)
	@echo assembling $(subst $(KERNEL_TOP_DIR)/,,$<)
	@$(CC) $(COMPILE_OPTION) $(COMP_DEPS_PATH) $(KERNEL_DEFINES) $(GENNER_DEPS_FLASH) -c -o $@ $<

# RANLIB: update the symbolic index table of the static library
$(LIB_KERNEL_SRC) : $(SRCS_KERNEL_OBJ)
	@$(MKDIR) $(dir $@)
	@echo "generating library kernel..."
	@$(AR) -cru  $@ $(SRCS_KERNEL_OBJ)
	@$(RANLIB) $@

####################################################################################################
# include .d files, refactoring dependency rule for every file
####################################################################################################
-include $(SRCS_KERNEL_DEP)