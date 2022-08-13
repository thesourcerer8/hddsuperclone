
program="hddsuperclone"
scripts="hddscripts"
bindir="/usr/local/bin/"
infodir="/usr/local/share/info/"
mandir="/usr/local/share/man/man1/"
docdir="/usr/local/share/doc/"
menudir="/usr/local/share/applications/"

rm -f $bindir$program
rm -f $mandir$program.1
rm -f $infodir$program.info
rm -f -r $bindir$scripts
rm -f -r $docdir$program
rm -f $bindir$program-uninstall.sh
rm -f $bindir$program-install-depends-deb.sh
rm -f $bindir$program-install-depends-red.sh
rm -f $menudir$program.desktop


program="hddsupertool"

rm -f $menudir$program.desktop