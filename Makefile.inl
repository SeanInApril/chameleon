#######################################################
#                                                     #
# Makefile                                            #
#                                                     #
# Sean<SeanInApril@163.com>                           #
#                                                     #
# Usage: this file is part of project Chameleon       #
#                                                     #
#######################################################
include global.cfg

CCFLAGS += $(OPTIONS)
CPFLAGS += $(OPTIONS)
ASFLAGS += $(OPTIONS)
LDFLAGS += $(OPTIONS)

DEF_MACROS  := $(foreach n,$(PREDEF),-D$(n))
INC_MODULES := $(foreach n,$(MODULES),-I$(top_dir)/$(n)/inc)
CCFLAGS += $(DEF_MACROS) $(INC_MODULES)

LIB := $(top_dir)/lib/lib$(module_name).a

.PHONY: clean

CSRCS := $(wildcard *.c)
COBJS := $(subst .c,.o,$(CSRCS))
OBJS  := $(COBJS)

CPPSRCS := $(wildcard *.cpp)
CPPOBJS := $(subst .cpp,.o,$(CPPSRCS))
OBJS    += $(CPPOBJS)

ASRCS := $(wildcard *.s)
AOBJS := $(subst .s,.o,$(ASRCS))
OBJS  += $(AOBJS)

DEPS := $(subst .c,.c.d,$(CSRCS))
DEPS += $(subst .cpp,.cpp.d,$(CPPSRCS))
DEPS += $(subst .s,.s.d,$(ASRCS))

$(LIB): $(OBJS)
#for empty directory
ifneq ($(filter %.o,$(OBJS)),)
	$(AR) rcs $@ $^
endif

define gen_deps
	@set -e; rm -f $<.d; \
	$(CC) -MM $(CCFLAGS) $< > $<.d.tmp; \
	sed 's,$*\.o[ :]*,$*.o $<.d: ,g' <$<.d.tmp >$<.d; \
	rm -f $<.d.tmp
endef

%.o: %.c
	$(CC) $(CCFLAGS) -c $< -o $@
	$(gen_deps)

%.o: %.cpp
	$(CC) $(CCFLAGS) -c $< -o $@
	$(gen_deps)

%.o: %.s
	$(CC) $(CCFLAGS) -c $< -o $@
	$(gen_deps)

clean:
	rm -f *.o $(DEPS) $(LIB)

-include $(DEPS)
