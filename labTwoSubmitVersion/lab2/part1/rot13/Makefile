CC      = gcc
OBJCOPY = objcopy
CFLAGS  = -O2 -ffreestanding -Wall -Werror -nostdinc -I ../libc/include
LDFLAGS = -nostdlib -Wl,-Ttext,$(LOAD_ADDR) 

LOAD_ADDR = 0xa2000000

CRT0    = ../libc/crt0.o
LDLIBS  = ../libc/libc.a

objects = rot13_verTwo.o

default: rot13_verTwo.bin

.PHONY: default clean clobber

rot13_verTwo.bin: rot13_verTwo
	$(OBJCOPY) -O binary $< $@

rot13_verTwo: $(objects)
	$(CC) $(LDFLAGS) -o $@ $(CRT0) $^ $(LDLIBS)

%.o: %.c
	$(CC) -c $(CFLAGS) -o $@ $<

clean:
	rm -f rot13_verTwo $(objects)

clobber: clean
	rm -f rot13_verTwo.bin
