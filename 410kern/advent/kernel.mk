################### P1 SPECIFIC BUILD TARGETS ########################
# This is similar to 410kern/kernel.mk and specifies how to build a
# P1-kernel version of the Crowther&Woods/Gillogly "adventure" game,
# ported from the DragonFlyBSD version by Cyrus Bomi Daruwala, S'21.

# I consume from config.mk:
#   COMMON_OBJS

410ADVENT_LOCAL_OBJS = \
	fake.o \
	p1main.o \
	io.o \
	data.o \
	main.o \
	init.o \
	wizard.o \
	subr.o \
	vocab.o \
	save.o \
	done.o

######################################################################
# Build 410kern/partial_advent.o and kern/partial_advent.o.
#
# They will be linked together by the "FINALTARGETS" machinery
# in 410kern/toplevel.mk.
######################################################################

######################################################################
# build the "410kern/ half": 410kern/partial_advent.o
######################################################################
410ADVENT_OBJS = $(410KDIR)/entry.o $(410ADVENT_LOCAL_OBJS:%=$(410KDIR)/advent/%)
ALL_410KOBJS += $(410ADVENT_OBJS)
410KCLEANS += $(410KDIR)/partial_advent.o

$(410KDIR)/partial_advent.o : $(410ADVENT_OBJS)
	$(LD) -r $(LDFLAGS) -o $@ $^

######################################################################
# build the "kern/ half": kern/partial_advent.o
######################################################################
STUADVENT_OBJS = $(COMMON_OBJS:%=$(STUKDIR)/%)
ALL_STUKOBJS += $(STUADVENT_OBJS)
STUKCLEANS += $(STUKDIR)/partial_advent.o

ifeq ($(STUADVENT_OBJS),)
$(STUKDIR)/partial_advent.o :
	touch $@
else
$(STUKDIR)/partial_advent.o : $(STUADVENT_OBJS)
	$(LD) -r $(LDFLAGS) -o $@ $^
endif
