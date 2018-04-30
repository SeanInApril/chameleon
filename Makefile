#######################################################
#                                                     #
# Makefile                                            #
#                                                     #
# Sean<SeanInApril@163.com>                           #
#                                                     #
# Usage: this file is part of project Chameleon       #
#                                                     #
#######################################################

#######################################################
# entry of all                                        #
#######################################################
include global.cfg

LIB_MODULES := $(foreach n,$(MODULES),lib/lib$(n).a)

TOPDIR := $(shell pwd)

.PHONY: all clean modules link

all: modules link

ifneq ($(modules),)
MODULES_TOBE_BUILT := $(modules)
else
MODULES_TOBE_BUILT := $(MODULES)
endif

modules:
	@echo "build modules start..."
	@for each_module in $(MODULES_TOBE_BUILT); \
	do \
		make -w -C $$each_module/src module_name=$$each_module top_dir=$(TOPDIR) -I$(TOPDIR); \
	done
	@echo "build modules end!"

ifneq ($(modules),)
link:
	@echo "build modules only"
else
link: $(TARGET)
endif

$(TARGET): $(LIB_MODULES)
	@echo "link start..."
	$(LD) $^ -o $@
	@echo "link end!"

clean:
	@echo "clean start..."
	@for each_module in $(MODULES); \
	do \
		make -w -C $$each_module/src clean module_name=$$each_module top_dir=$(TOPDIR) -I$(TOPDIR); \
	done
	rm -f $(TARGET)
	@echo "clean end!"
