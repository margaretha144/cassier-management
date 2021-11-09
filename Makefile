CFLAGS	+= -std=99 -Wall -Wextra -pedantic -Wold-style-declaration
CFLAGS	+= -Wmissing-prototypes -Wno-ununsed-parameter
CC		?= cc
PREFIX	?= /usr
BINDIR	?= $(PREFIX)/bin


all: hmi

config.h:
	cp config.def.h config.h

hmi: hmi.c hmi.h config.h Makefile
	$(CC) -O3 $(CFLAGS) -o $@ $< -luuid $(LDFLAGS)

install: all
	install -Dm755 hmi $(DESTDIR)$(BINDIR)/hmi

uninstall:
	rm -f $(DESTDIR)$(BINDIR)/hmi

clean:
	rm -r hmi *.o

.PHONY: all install uninstall clean
