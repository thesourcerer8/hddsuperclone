# makefile for installing hddsuperclone
# sudo make install       (to install)
# sudo make uninstall     (to uninstall)

PROG00 = hddsuperclone
PROG01 = hddsupertool

bindir = $(DESTDIR)/usr/local/bin/
oldinfodir = $(DESTDIR)/usr/share/info/
infodir = $(DESTDIR)/usr/local/share/info/
oldmandir = $(DESTDIR)/usr/share/man/man1/
mandir = $(DESTDIR)/usr/local/share/man/man1/
docdir = $(DESTDIR)/usr/local/share/doc/
menudir = $(DESTDIR)/usr/local/share/applications/
scripts = hddscripts

all:

install:
	rm -f $(bindir)$(PROG00)
	rm -f $(bindir)$(PROG00)-uninstall.sh
	rm -f $(oldmandir)$(PROG00).1
	rm -f $(oldinfodir)$(PROG00).info
	rm -f $(mandir)$(PROG00).1
	rm -f $(infodir)$(PROG00).info
	rm -f -r $(docdir)$(PROG00)
	rm -f -r $(bindir)$(scripts)
	rm -f $(menudir)$(PROG00).desktop
	rm -f $(menudir)$(PROG01).desktop
	mkdir -p $(bindir)
	mkdir -p $(infodir)
	mkdir -p $(mandir)
	mkdir -p $(menudir)
	mkdir -p $(docdir)$(PROG00)
	cp $(PROG00) $(bindir)$(PROG00)
	chmod a+x /usr/local/bin/$(PROG00)
	cp $(PROG00)-uninstall.sh $(bindir)$(PROG00)-uninstall.sh
	chmod a+x /usr/local/bin/$(PROG00)-uninstall.sh
	cp man.1 $(mandir)$(PROG00).1
	#cp help.info $(infodir)$(PROG00).info
	cp $(PROG00).txt $(docdir)$(PROG00)
	cp $(PROG00).html $(docdir)$(PROG00)
	cp $(PROG00).odt $(docdir)$(PROG00)
	cp $(PROG01).txt $(docdir)$(PROG00)
	cp $(PROG01).html $(docdir)$(PROG00)
	cp EULA.txt $(docdir)$(PROG00)
	cp -R Language $(docdir)$(PROG00)
	cp -R $(scripts) $(bindir)
	cp $(PROG00).desk $(menudir)$(PROG00).desktop
	cp $(PROG01).desk $(menudir)$(PROG01).desktop

uninstall:
	rm -f $(bindir)$(PROG00)
	rm -f $(bindir)$(PROG00)-uninstall.sh
	rm -f $(oldmandir)$(PROG00).1
	rm -f $(oldinfodir)$(PROG00).info
	rm -f $(mandir)$(PROG00).1
	rm -f $(infodir)$(PROG00).info
	rm -f -r $(docdir)$(PROG00)
	rm -f -r $(bindir)$(scripts)
	rm -f $(menudir)$(PROG00).desktop
	rm -f $(menudir)$(PROG01).desktop
