CC     = gcc
CFLAGS = -O2 -Wall -Werror 

objects = math.o calc.o

default: calc

.PHONY: default clean clobber

calc: $(objects)
	$(CC) -o $@ $^

%.o: %.c
	$(CC) -c $(CFLAGS) -o $@ $<
	
math.o: math.c math.h
calc.o: calc.c math.h
 
clean:
	rm -f $(objects)

clobber: clean
	rm -f calc
