.PHONY: all clean shows shown

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

JOBS := $(shell cat /proc/cpuinfo| grep "processor"| wc -l)


all:
	@cd $(MAKEFILE_DIR);$(TIME) make --no-print-directory -j $(JOBS) all

clean:
	@cd $(MAKEFILE_DIR);$(TIME) make --no-print-directory clean
	
shows:
	@cd $(MAKEFILE_DIR);make --no-print-directory shows
	
shown:
	@cd $(MAKEFILE_DIR);make --no-print-directory shown
