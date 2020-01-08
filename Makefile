SRCS = $(wildcard *.c)
PROGS = $(patsubst %.c,%,$(SRCS))
#CFLAGS = -Wall -L/share/apps/papi/5.4.1/lib -I/share/apps/papi/5.4.1/include -O2 -lpapi
CFLAGS = -Wall -O2

all: $(PROGS)

%: %.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f $(PROGS)
