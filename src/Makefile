.PHONY: all clean showv shown

OS_NAME := $(shell uname -o)
LC_OS_NAME := $(shell echo $(OS_NAME) | tr A-Z a-z)

# Determine current platform
ifeq ($(LC_OS_NAME),cygwin)
TIME = time
else
TIME = time -p
endif


# See folder "TARGETS", set a project

MAKEFILE_DIR = ./TARGETS/GCC-ARM/make


all:
	@cd $(MAKEFILE_DIR);$(TIME) make --no-print-directory -j 10 -f Makefile all

clean:
	@cd $(MAKEFILE_DIR);$(TIME) make --no-print-directory -f Makefile clean
	
shows:
	@cd $(MAKEFILE_DIR);make --no-print-directory -f Makefile shows
	
shown:
	@cd $(MAKEFILE_DIR);make --no-print-directory -f Makefile shown
