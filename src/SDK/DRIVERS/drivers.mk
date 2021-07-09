####################################################################################################
# directory, all the file in this directory
####################################################################################################
DRIVERS_TOP_DIR := $(TOP_DIR)
DRIVERS_LIB_DIR := $(LIBRARY_DIR)
DRIVERS_SRC_DIR := $(DRIVERS_TOP_DIR)/DRIVERS
DRIVERS_OBJ_DIR := $(OBJECT_DIR)/DRIVERS


####################################################################################################
# library
####################################################################################################
LIB_DRIVERS_NAME = drivers
LIB_DRIVERS_SRC  = $(DRIVERS_LIB_DIR)/lib$(LIB_DRIVERS_NAME).a


####################################################################################################
# include other makefile to include source
####################################################################################################
-include $(DRIVERS_SRC_DIR)/driverlib/driverlib.mk
-include $(DRIVERS_SRC_DIR)/hal/hal.mk
-include $(DRIVERS_SRC_DIR)/IpcMsg/IpcMsg.mk
-include $(DRIVERS_SRC_DIR)/mcnt/mcnt.mk
-include $(DRIVERS_SRC_DIR)/mpu/mpu.mk
-include $(DRIVERS_SRC_DIR)/smartcard/smartcard.mk


####################################################################################################
# DRIVERS_EXCLUDE_LIB   SRCS_DRIVERS_FILE   DEPS_PATH_DRIVERS
# these variable is defined and assignment in include makefile
####################################################################################################


####################################################################################################
# objects and depends
####################################################################################################
TMP_DRIVERS_OBJ  += $(subst .S,.o.ram,$(subst .s,.o.ram,$(subst .c,.o.ram,$(SRCS_DRIVERS_RAM))))
TMP_DRIVERS_OBJ  += $(subst .S,.o.flash,$(subst .s,.o.flash,$(subst .c,.o.flash,$(SRCS_DRIVERS_FLASH))))
SRCS_DRIVERS_OBJ  = $(subst $(DRIVERS_SRC_DIR),$(DRIVERS_OBJ_DIR),$(TMP_DRIVERS_OBJ))
SRCS_DRIVERS_OBJ := $(sort $(SRCS_DRIVERS_OBJ))

TMP_DRIVERS_EXCLUDE_LIB_OBJ += $(subst .S,.o.ram,$(subst .s,.o.ram,$(subst .c,.o.ram,$(DRIVERS_EXCLUDE_LIB_RAM))))
TMP_DRIVERS_EXCLUDE_LIB_OBJ += $(subst .S,.o.flash,$(subst .s,.o.flash,$(subst .c,.o.flash,$(DRIVERS_EXCLUDE_LIB_FLASH))))
DRIVERS_EXCLUDE_LIB_OBJ      = $(subst $(DRIVERS_SRC_DIR),$(DRIVERS_OBJ_DIR),$(TMP_DRIVERS_EXCLUDE_LIB_OBJ))
DRIVERS_EXCLUDE_LIB_OBJ     := $(sort $(DRIVERS_EXCLUDE_LIB_OBJ))

SRCS_DRIVERS_DEP += $(patsubst %.o.ram,%.d,$(patsubst %.o.flash,%.d,$(SRCS_DRIVERS_OBJ)))
SRCS_DRIVERS_DEP += $(patsubst %.o.ram,%.d,$(patsubst %.o.flash,%.d,$(DRIVERS_EXCLUDE_LIB_OBJ)))


####################################################################################################
# add to makefile calling, be included in other makefile
####################################################################################################
SRCS_DEPS_PATH += $(DEPS_PATH_DRIVERS)

ifeq ($(DRIVERS_LIBRARY),a)
SRCS_OBJ_FILES += $(DRIVERS_EXCLUDE_LIB_OBJ)
SRCS_OBJ_FILES += $(SRCS_DRIVERS_OBJ)
SRCS_LIBS_INC  += 
endif

ifeq ($(DRIVERS_LIBRARY),b)
SRCS_OBJ_FILES += $(DRIVERS_EXCLUDE_LIB_OBJ)
SRCS_LIBS_INC  += -l$(LIB_DRIVERS_NAME)
endif

ifeq ($(DRIVERS_LIBRARY),c)
SRCS_OBJ_FILES += $(DRIVERS_EXCLUDE_LIB_OBJ)
SRCS_LIBS_INC  += -l$(LIB_DRIVERS_NAME)
endif


####################################################################################################
# make target
####################################################################################################
MAIN_ALL += all_drivers
MAIN_CLEAN += clean_drivers

ifeq ($(DRIVERS_LIBRARY),a)
all_drivers   : generate_drivers_exclude_obj   generate_drivers_obj
clean_drivers : clear_drivers_obj   clear_drivers_lib
endif

ifeq ($(DRIVERS_LIBRARY),b)
all_drivers   : generate_drivers_exclude_obj generate_drivers_lib
clean_drivers : clear_drivers_obj   clear_drivers_lib
endif

ifeq ($(DRIVERS_LIBRARY),c)
all_drivers   : generate_drivers_exclude_obj
clean_drivers : clear_drivers_srcs   clear_drivers_obj
endif


generate_drivers_obj: $(SRCS_DRIVERS_OBJ)

generate_drivers_exclude_obj: $(DRIVERS_EXCLUDE_LIB_OBJ)

generate_drivers_lib: $(LIB_DRIVERS_SRC)

clear_drivers_srcs:
#	@echo "delete drivers source files..."
	@$(RM) $(SRCS_DRIVERS_RAM) $(SRCS_DRIVERS_FLASH)

clear_drivers_obj:
#	@echo "delete drivers objects..."
	@$(RM) $(DRIVERS_OBJ_DIR)

clear_drivers_lib:
#	@echo "delete drivers library..."
	@$(RM) $(LIB_DRIVERS_SRC)


####################################################################################################
# foler definition, include public definition and private definition
####################################################################################################
DRIVERS_DEFINES += $(DEFINES) $(DEFINE_FOR_DRIVERS)


####################################################################################################
# every file's compiling rules
####################################################################################################
$(DRIVERS_OBJ_DIR)/%.o.ram: $(DRIVERS_SRC_DIR)/%.c
	@$(MKDIR) $(dir $@)
	@echo compiling $(subst $(DRIVERS_TOP_DIR)/,,$<)
	@$(CC) $(COMPILE_OPTION) $(COMP_DEPS_PATH) $(GENNER_DEPS_RAM) $(DRIVERS_DEFINES) -c -o $@ $<
	
$(DRIVERS_OBJ_DIR)/%.o.flash: $(DRIVERS_SRC_DIR)/%.c
	@$(MKDIR) $(dir $@)
	@echo compiling $(subst $(DRIVERS_TOP_DIR)/,,$<)
	@$(CC) $(COMPILE_OPTION) $(COMP_DEPS_PATH) $(GENNER_DEPS_FLASH) $(DRIVERS_DEFINES) -c -o $@ $<
	
$(DRIVERS_OBJ_DIR)/%.o.ram: $(DRIVERS_SRC_DIR)/%.s
	@$(MKDIR) $(dir $@)
	@echo assembling $(subst $(DRIVERS_TOP_DIR)/,,$<)
	@$(CC) $(COMPILE_OPTION) $(COMP_DEPS_PATH) $(GENNER_DEPS_RAM) $(DRIVERS_DEFINES) -c -o $@ $<
	
$(DRIVERS_OBJ_DIR)/%.o.flash: $(DRIVERS_SRC_DIR)/%.s
	@$(MKDIR) $(dir $@)
	@echo assembling $(subst $(DRIVERS_TOP_DIR)/,,$<)
	@$(CC) $(COMPILE_OPTION) $(COMP_DEPS_PATH) $(GENNER_DEPS_FLASH) $(DRIVERS_DEFINES) -c -o $@ $<

$(DRIVERS_OBJ_DIR)/%.o.ram : $(DRIVERS_SRC_DIR)/%.S
	@$(MKDIR) $(dir $@)
	@echo assembling $(subst $(DRIVERS_TOP_DIR)/,,$<)
	@$(CC) $(COMPILE_OPTION) $(COMP_DEPS_PATH) $(GENNER_DEPS_RAM) $(DRIVERS_DEFINES) -c -o $@ $<

$(DRIVERS_OBJ_DIR)/%.o.flash : $(DRIVERS_SRC_DIR)/%.S
	@$(MKDIR) $(dir $@)
	@echo assembling $(subst $(DRIVERS_TOP_DIR)/,,$<)
	@$(CC) $(COMPILE_OPTION) $(COMP_DEPS_PATH) $(DRIVERS_DEFINES) $(GENNER_DEPS_FLASH) -c -o $@ $<

# RANLIB: update the symbolic index table of the static library
$(LIB_DRIVERS_SRC) : $(SRCS_DRIVERS_OBJ)
	@$(MKDIR) $(dir $@)
	@echo "generating library drivers..."
	@$(AR) -cru  $@ $(SRCS_DRIVERS_OBJ)
	@$(RANLIB) $@


####################################################################################################
# include .d files, refactoring dependency rule for every file
####################################################################################################
-include $(SRCS_DRIVERS_DEP)