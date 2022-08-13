# makefile for ddrutility
# version 2.0 20161123

CC= gcc
CFLAGS = -Wall -W
LDFLAGS = -lm
PROG00 = ddrutility
PROG05 = ddru_ntfsbitmap
PROG08 = ddru_ntfsfindbad
PROG09 = ddru_diskutility
PROG20 = ddru_findbad
PROG50 = ddru_ntfscommon
bindir = $(DESTDIR)/usr/local/bin/
infodir = $(DESTDIR)/usr/local/share/info/
mandir = $(DESTDIR)/usr/local/share/man/man1/
docdir= $(DESTDIR)/usr/local/share/doc/

all: $(PROG00) $(PROG05) $(PROG08) $(PROG09)


$(PROG00) : $(PROG00).c
	$(CC) $(CFLAGS) $(PROG00).c -o $(PROG00)

$(PROG05) : $(PROG05).c
	$(CC) $(CFLAGS) $(PROG50).c $(PROG05).c -o $(PROG05)

$(PROG08) : $(PROG08).c
	$(CC) $(CFLAGS) $(PROG50).c $(PROG08).c -o $(PROG08)

$(PROG09) : $(PROG09).c
	$(CC) $(CFLAGS) $(PROG09).c -o $(PROG09)


clean:
	rm -f $(PROG00)
	rm -f $(PROG05)
	rm -f $(PROG08)
	rm -f $(PROG09)

install:
	mkdir -p $(bindir)
	mkdir -p $(infodir)
	mkdir -p $(mandir)
	rm -f $(bindir)$(PROG00)
	rm -f $(mandir)$(PROG00).1
	rm -f $(infodir)$(PROG00).info
	rm -f $(bindir)$(PROG05)
	rm -f $(mandir)$(PROG05).1
	rm -f $(bindir)$(PROG08)
	rm -f $(mandir)$(PROG08).1
	rm -f $(bindir)$(PROG09)
	rm -f $(mandir)$(PROG09).1
	rm -f $(bindir)$(PROG20)
	rm -f $(mandir)$(PROG20).1
	cp $(PROG00) $(bindir)$(PROG00)
	cp $(PROG00).1 $(mandir)$(PROG00).1
	cp $(PROG00).info $(infodir)$(PROG00).info
	cp $(PROG05) $(bindir)$(PROG05)
	cp $(PROG05).1 $(mandir)$(PROG05).1
	cp $(PROG08) $(bindir)$(PROG08)
	cp $(PROG08).1 $(mandir)$(PROG08).1
	cp $(PROG09) $(bindir)$(PROG09)
	cp $(PROG09).1 $(mandir)$(PROG09).1
	cp $(PROG20).sh $(bindir)$(PROG20)
	cp $(PROG20).1 $(mandir)$(PROG20).1

uninstall:
	rm -f $(bindir)$(PROG00)
	rm -f /usr/share/man/man1/$(PROG00).1
	rm -f /usr/local/share/info/$(PROG00).info
	rm -f $(bindir)$(PROG05)
	rm -f /usr/share/man/man1/$(PROG05).1
	rm -f /usr/share/man/man1/$(PROG08).1
	rm -f $(bindir)$(PROG09)
	rm -f /usr/share/man/man1/$(PROG09).1
	rm -f $(bindir)$(PROG20)
	rm -f /usr/share/man/man1/$(PROG20).1
