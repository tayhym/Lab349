PROGS_PERIODIC_OBJS := periodic.o
PROGS_PERIODIC_OBJS := $(PROGS_PERIODIC_OBJS:%=$(TDIR)/periodic/%)
ALL_OBJS += $(PROGS_PERIODIC_OBJS)

$(TDIR)/bin/periodic : $(TSTART) $(PROGS_PERIODIC_OBJS) $(TLIBC)
