PROGS_TEST_OBJS := test2.o
PROGS_TEST_OBJS := $(PROGS_TEST_OBJS:%=$(TDIR)/testWriteTime/%)
ALL_OBJS += $(PROGS_TEST_OBJS)

$(TDIR)/bin/testWriteTime : $(TSTART) $(PROGS_TEST_OBJS) $(TLIBC)

