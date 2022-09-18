Cc=gcc
CCOPTS=--std=gnu99 -g -Wall 



HEADERS=top.h

OBJS=top_func.c

LIBS=

BINS=top

		
.phony: clean all

all: $(BINS) $(LIBS)

%.o: %.c $(HEADERS)
	$(CC) $(CCOPTS) -c -o $@ $<

		
top: 	top.c $(OBJS)
		$(CC) $(CCOPTS) -o $@ $^
				

clean:
	rm -rf *.o *~ $(LIBS) $(BINS)
