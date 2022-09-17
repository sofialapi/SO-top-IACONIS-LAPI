Cc=gcc
CCOPTS=--std=gnu99 
Ar=ar

HEADERS=top.h

OBJS=top.o\

LIBS=libtop.a

BINS=top_test
		
.phony: clean all

all: $(LIBS) $(BINS)

%.o: %.c $(HEADERS)
	$(CC) $(CCOPTS) -c -o $@ $<
	
libtop.a: $(OBJS) $(HEADERS)
			$(AR) -rcs $@ $^
			$(RM) $(OBJS)
		
top_test: 	top.c $(LIBS)
		$(CC) $(CCOPTS) -o $@ $^
		

clean:
	rm -rf *.o *~ $(LIBS) $(BINS)
