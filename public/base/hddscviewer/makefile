# makefile for installing hddviewer
# sudo make install       (to install)
# sudo make uninstall     (to uninstall)

PROG00 = hddscviewer
PROG01 = hddviewer

bindir = $(DESTDIR)/usr/local/bin/
infodir = $(DESTDIR)/usr/local/share/info/
oldmandir = $(DESTDIR)/usr/share/man/man1/
mandir = $(DESTDIR)/usr/local/share/man/man1/
docdir = $(DESTDIR)/usr/local/share/doc/
menudir = $(DESTDIR)/usr/local/share/applications/

all:

install:
	rm -f $(bindir)$(PROG01)
	rm -f $(bindir)$(PROG01)-uninstall.sh
	rm -f -r $(docdir)$(PROG01)
	rm -f $(bindir)$(PROG00)
	rm -f $(bindir)$(PROG00)-uninstall.sh
	rm -f -r $(docdir)$(PROG00)
	rm -f $(menudir)$(PROG00).desktop
	mkdir -p $(bindir)
	mkdir -p $(docdir)$(PROG00)
	mkdir -p $(menudir)
	cp $(PROG00) $(bindir)$(PROG00)
	cp $(PROG00)-uninstall.sh $(bindir)$(PROG00)-uninstall.sh
	cp help.txt $(docdir)$(PROG00)
	cp help.html $(docdir)$(PROG00)
	cp EULA.txt $(docdir)$(PROG00)
	chmod a+x $(bindir)$(PROG00)
	cp -R Language $(docdir)$(PROG00)
	cp $(PROG00).desk $(menudir)$(PROG00).desktop

uninstall:
	rm -f $(bindir)$(PROG01)
	rm -f $(bindir)$(PROG01)-uninstall.sh
	rm -f -r $(docdir)$(PROG01)
	rm -f $(bindir)$(PROG00)
	rm -f $(bindir)$(PROG00)-uninstall.sh
	rm -f -r $(docdir)$(PROG00)
	rm -f $(menudir)$(PROG00).desktop


