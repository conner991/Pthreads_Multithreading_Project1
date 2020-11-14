#default: sumsq par_sumsq

#sumsq: sumsq.c
#	gcc -g -O2 --std=c99 -Wall -o sumsq sumsq.c

#par_sumsq: par_sumsq.c
#	gcc -g -O2 --std=c99 -Wall -o par_sumsq par_sumsq.c -lpthread

#clean:
#	rm -f sumsq par_sumsq

# Edit this file to compile extra C files into their own programs.
# TARGETS = sum_on_thread sum_on_many_threads sum_malloc
TARGETS = par_sumsq

CROSS_TOOL = 
CC_CPP = $(CROSS_TOOL)g++
CC_C = $(CROSS_TOOL)gcc

CFLAGS = -Wall -g -std=c99 -pthread

all: clean $(TARGETS)

$(TARGETS):
		$(CC_C) $(CFLAGS) $@.c -o $@
	
clean:
		rm -f $(TARGETS)