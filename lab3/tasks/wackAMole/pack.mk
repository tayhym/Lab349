PROGS_WACKAMOLE_OBJS := wackAMole.o
PROGS_WACKAMOLE_OBJS := $(PROGS_WACKAMOLE_OBJS:%=$(TDIR)/wackAMole/%)
ALL_OBJS += $(PROGS_WACKAMOLE_OBJS)

$(TDIR)/bin/wackAMole : $(TSTART) $(PROGS_WACKAMOLE_OBJS) $(TLIBC)

