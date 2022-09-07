
# add -g to CFLAGS for extra debugging
# use -k command line option to enable debugging

GTKVER = 2

OSVER=$(shell lsb_release -sr)
ifeq ($(OSVER),18.04)
OFFICEVER = libreoffice
else
OFFICEVER = soffice
endif

CC= gcc
ifneq (,$(findstring k,$(MAKEFLAGS)))
CFLAGS = -Wall -W -O0 -g
else
CFLAGS = -Wall -W -Wno-deprecated-declarations
endif
USBFLAGS = -lusb
CURLFLAGS = -DUSE_CURL -lcurl
GTKFLAGS = `pkg-config --cflags --libs gtk+-$(GTKVER).0` -export-dynamic
PROG00 = hddsupertool
PROG01 = commands
PROG02 = io
PROG03 = common
PROG04 = tool
PROG05 = usbrelay
PROG20 = hddsuperclone
PROG21 = clone_gui$(GTKVER)
PROG22 = clone_gui_language
PROG30 = hddscviewer
PROG11 = changecode
PROG12 = create_script_help
scripts = hddscripts
bindir = $(DESTDIR)/usr/local/bin/
infodir = $(DESTDIR)/usr/local/share/info/
oldmandir = $(DESTDIR)/usr/share/man/man1/
mandir = $(DESTDIR)/usr/local/share/man/man1/
docdir= $(DESTDIR)/usr/local/share/doc/
menudir= $(DESTDIR)/usr/local/share/applications/
driverdir= driver/

all:  $(PROG12) $(PROG11) $(PROG20) $(PROG30) $(PROG00)
change:  $(PROG11)
clone: $(PROG12) $(PROG20)
viewer: $(PROG30)


$(PROG00) : $(PROG00).c
	xxd -i $(PROG00)_help.txt $(PROG00)_help.h
	$(CC) $(CFLAGS) $(PROG00).c $(PROG01).c $(PROG02).c $(PROG03).c $(PROG05).c -o $(PROG00) $(CURLFLAGS) $(USBFLAGS)
	help2man ./$(PROG00) > $(PROG00).1
	sudo ./$(PROG12)
	makeinfo $(PROG00).texi
	makeinfo $(PROG00).texi --html --no-split
	makeinfo $(PROG00).texi --plaintext -o $(PROG00).txt

$(PROG11) : $(PROG11).c
	$(CC) $(CFLAGS) $(PROG11).c -o $(PROG11)

$(PROG12) : $(PROG12).c
	$(CC) $(CFLAGS) $(PROG12).c -o $(PROG12)

$(PROG20) : $(PROG20).c
	xxd -i $(PROG00)_help.txt $(PROG00)_help.h
	xxd -i $(PROG20)_help.txt $(PROG20)_help.h
	xxd -i $(PROG20)$(GTKVER).glade $(PROG20)$(GTKVER)_glade.h
	cp -f public/base/$(PROG20)/EULA $(PROG20)_EULA.txt
	xxd -i $(PROG20)_EULA.txt $(PROG20)_EULA.h
	xxd -i $(driverdir)$(PROG20)_driver.c $(PROG20)_driver.h
	$(CC) $(CFLAGS) $(PROG20).c $(PROG01).c $(PROG02).c $(PROG04).c $(PROG05).c $(PROG03).c $(PROG21).c $(PROG22).c -o $(PROG20) $(GTKFLAGS) $(CURLFLAGS) $(USBFLAGS)
	sudo ./$(PROG12)
	makeinfo $(PROG00).texi
	makeinfo $(PROG00).texi --html --no-split
	makeinfo $(PROG00).texi --plaintext -o $(PROG00).txt
	help2man ./$(PROG20) > $(PROG20).1
	#makeinfo $(PROG20).texi
	#makeinfo $(PROG20).texi --html --no-split
	#makeinfo $(PROG20).texi --plaintext -o $(PROG20).txt
	$(OFFICEVER) --headless --convert-to txt:Text hddsuperclone.odt
	$(OFFICEVER) --headless --convert-to html:HTML hddsuperclone.odt
	$(OFFICEVER) --headless --convert-to pdf:writer_pdf_Export hddsuperclone.odt
	# the following line only works on newer versions of libreoffice, only needed to embed images
	#$(OFFICEVER) --headless --convert-to html:HTML:EmbedImages hddsuperclone.odt

$(PROG30) : $(PROG30)$(GTKVER).c
	xxd -i $(PROG30)$(GTKVER).glade $(PROG30)$(GTKVER)_glade.h
	$(CC) $(CFLAGS) $(PROG30)$(GTKVER).c -o $(PROG30) $(GTKFLAGS) $(CURLFLAGS)
	#makeinfo $(PROG30).texi --html --no-split
	#makeinfo $(PROG30).texi --plaintext -o $(PROG30).txt
	$(OFFICEVER) --headless --convert-to txt:Text hddscviewer.odt
	$(OFFICEVER) --headless --convert-to html:HTML hddscviewer.odt
	$(OFFICEVER) --headless --convert-to pdf:writer_pdf_Export hddscviewer.odt

server : $(PROG20).c
	xxd -i $(PROG00)_help.txt $(PROG00)_help.h
	xxd -i $(PROG20)_help.txt $(PROG20)_help.h
	xxd -i $(PROG20)$(GTKVER).glade $(PROG20)$(GTKVER)_glade.h
	cp -f public/base/$(PROG20)/EULA $(PROG20)_EULA.txt
	xxd -i $(PROG20)_EULA.txt $(PROG20)_EULA.h
	xxd -i $(driverdir)$(PROG20)_driver.c $(PROG20)_driver.h
	$(CC) $(CFLAGS) $(PROG20).c $(PROG01).c $(PROG02).c $(PROG04).c $(PROG05).c $(PROG03).c $(PROG21).c $(PROG22).c -o $(PROG20) $(GTKFLAGS) $(CURLFLAGS) $(USBFLAGS)

installserver:
	rm -f $(bindir)$(PROG20)
	cp $(PROG20) $(bindir)$(PROG20)


clean:
	rm -f $(PROG00)
	rm -f $(PROG00)_help.h
	rm -f $(PROG00).1
	rm -f $(PROG00).info
	rm -f $(PROG00).html
	rm -f $(PROG00).txt
	rm -f $(PROG00).markdown
	rm -f $(PROG20)
	rm -f $(PROG20)_help.h
	rm -f $(PROG20)_glade.h
	rm -f $(PROG20)2_glade.h
	rm -f $(PROG20)3_glade.h
	rm -f $(PROG20)_EULA.h
	rm -f $(PROG20)_EULA.txt
	rm -f $(PROG20)_driver.h
	rm -f $(PROG20).1
	rm -f $(PROG20).info
	rm -f $(PROG20).html
	rm -f $(PROG20).htm
	rm -f $(PROG20).pdf
	rm -f $(PROG20).txt
	rm -f $(PROG20).markdown
	rm -f $(PROG30)
	rm -f $(PROG30)_help.h
	rm -f $(PROG30)2_glade.h
	rm -f $(PROG30)3_glade.h
	rm -f $(PROG30).1
	rm -f $(PROG30).info
	rm -f $(PROG30).html
	rm -f $(PROG30).htm
	rm -f $(PROG30).pdf
	rm -f $(PROG30).txt
	rm -f $(PROG30).markdown
	rm -f $(PROG11)
	rm -f $(PROG12)


install:
	mkdir -p $(bindir)
	mkdir -p $(infodir)
	mkdir -p $(mandir)
	rm -f $(bindir)$(PROG20)
	rm -f $(bindir)$(PROG20)-uninstall.sh
	rm -f $(mandir)$(PROG20).1
	rm -f $(oldmandir)$(PROG20).1
	rm -f $(infodir)$(PROG20).info
	cp $(PROG20) $(bindir)$(PROG20)
	cp $(PROG20)-uninstall.sh $(bindir)$(PROG20)-uninstall.sh
	cp $(PROG20).1 $(mandir)$(PROG20).1
	#cp $(PROG20).info $(infodir)$(PROG20).info
	rm -f $(bindir)$(PROG30)
	rm -f $(bindir)$(PROG30)-uninstall.sh
	cp $(PROG30) $(bindir)$(PROG30)
	cp $(PROG30)-uninstall.sh $(bindir)$(PROG30)-uninstall.sh
	rm -f $(bindir)$(PROG11)
	cp $(PROG11) $(bindir)$(PROG11)
	rm -f $(bindir)$(PROG12)
	cp $(PROG12) $(bindir)$(PROG12)
	rm -f -r $(bindir)$(scripts)
	cp -R $(scripts) $(bindir)
	rm -f -r $(docdir)$(PROG20)
	mkdir -p $(docdir)$(PROG20)
	cp $(PROG20).txt $(docdir)$(PROG20)
	cp $(PROG20).html $(docdir)$(PROG20)
	cp public/base/$(PROG20)/EULA $(docdir)$(PROG20)
	cp -R languages/$(PROG20)/Language $(docdir)$(PROG20)
	cp -R languages/$(PROG30)/Language $(docdir)$(PROG30)


installchange:
	rm -f $(bindir)$(PROG11)
	cp $(PROG11) $(bindir)$(PROG11)


installtool:
	mkdir -p $(bindir)
	mkdir -p $(infodir)
	mkdir -p $(mandir)
	rm -f $(bindir)$(PROG00)
	rm -f $(bindir)$(PROG00)-uninstall.sh
	rm -f $(mandir)$(PROG00).1
	rm -f $(infodir)$(PROG00).info
	cp $(PROG00) $(bindir)$(PROG00)
	cp $(PROG00)-uninstall.sh $(bindir)$(PROG00)-uninstall.sh
	cp $(PROG00).1 $(mandir)$(PROG00).1
	cp $(PROG00).info $(infodir)$(PROG00).info
	rm -f -r $(bindir)$(scripts)
	cp -R $(scripts) $(bindir)
	rm -f -r $(docdir)$(PROG00)
	mkdir -p $(docdir)$(PROG00)
	cp $(PROG00).txt $(docdir)$(PROG00)
	cp $(PROG00).html $(docdir)$(PROG00)
	cp public/base/$(PROG00)/EULA $(docdir)$(PROG00)


installclone:
	rm -f $(bindir)$(PROG20)
	rm -f $(bindir)$(PROG20)-uninstall.sh
	rm -f $(bindir)$(PROG20)-install-depends-deb.sh
	rm -f $(bindir)$(PROG20)-install-depends-red.sh
	rm -f $(mandir)$(PROG20).1
	rm -f -r $(bindir)$(scripts)
	rm -f $(infodir)$(PROG20).info
	rm -f $(menudir)$(PROG00).desktop
	rm -f $(menudir)$(PROG20).desktop
	rm -f -r $(docdir)$(PROG20)
	mkdir -p $(bindir)
	#mkdir -p $(infodir)
	mkdir -p $(mandir)
	mkdir -p $(menudir)
	mkdir -p $(docdir)$(PROG20)
	cp $(PROG20) $(bindir)$(PROG20)
	cp $(PROG20)-uninstall.sh $(bindir)$(PROG20)-uninstall.sh
	cp $(PROG20)-install-depends-deb.sh $(bindir)$(PROG20)-install-depends-deb.sh
	cp $(PROG20)-install-depends-red.sh $(bindir)$(PROG20)-install-depends-red.sh
	cp $(PROG20).1 $(mandir)$(PROG20).1
	cp -R $(scripts) $(bindir)
	#cp $(PROG20).info $(infodir)$(PROG20).info
	cp $(PROG20).txt $(docdir)$(PROG20)
	cp $(PROG20).html $(docdir)$(PROG20)
	cp $(PROG00).txt $(docdir)$(PROG20)
	cp $(PROG00).html $(docdir)$(PROG20)
	cp public/base/$(PROG20)/EULA $(docdir)$(PROG20)
	cp -R languages/$(PROG20)/Language $(docdir)$(PROG20)
	cp $(PROG00).desk $(menudir)$(PROG00).desktop
	cp $(PROG20).desk $(menudir)$(PROG20).desktop


uninstall:
	rm -f $(bindir)$(PROG00)
	rm -f $(bindir)$(PROG00)-uninstall.sh
	rm -f $(mandir)$(PROG00).1
	rm -f $(infodir)$(PROG00).info
	rm -f $(menudir)$(PROG00).desktop
	rm -f $(bindir)$(PROG20)
	rm -f $(bindir)$(PROG20)-uninstall.sh
	rm -f $(mandir)$(PROG20).1
	rm -f $(infodir)$(PROG20).info
	rm -f $(menudir)$(PROG20).desktop
	rm -f $(bindir)$(PROG30)
	rm -f $(bindir)$(PROG30)-uninstall.sh
	rm -f $(menudir)$(PROG30).desktop
	rm -f $(bindir)$(PROG11)
	rm -f $(bindir)$(PROG12)
	rm -f -r $(bindir)$(scripts)
	rm -f -r $(docdir)$(PROG00)
	rm -f -r $(docdir)$(PROG20)


installviewer:
	rm -f $(bindir)$(PROG30)
	rm -f $(bindir)$(PROG30)-uninstall.sh
	rm -f -r $(docdir)$(PROG30)
	mkdir -p $(bindir)
	mkdir -p $(menudir)
	mkdir -p $(docdir)$(PROG30)
	cp $(PROG30) $(bindir)$(PROG30)
	cp $(PROG30)-uninstall.sh $(bindir)$(PROG30)-uninstall.sh
	cp $(PROG30).txt $(docdir)$(PROG30)
	cp $(PROG30).html $(docdir)$(PROG30)
	cp public/base/$(PROG30)/EULA $(docdir)$(PROG30)
	cp -R languages/$(PROG30)/Language $(docdir)$(PROG30)
	cp $(PROG30).desk $(menudir)$(PROG30).desktop

uninstallviewer:
	rm -f $(bindir)$(PROG30)
	rm -f $(bindir)$(PROG30)-uninstall.sh
	rm -f -r $(docdir)$(PROG30)
	rm -f $(menudir)$(PROG30).desktop
