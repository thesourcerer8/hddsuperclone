
program="hddscviewer"
bindir="/usr/local/bin/"
infodir="/usr/local/share/info/"
oldmandir="/usr/share/man/man1/"
mandir="/usr/local/share/man/man1/"
docdir="/usr/local/share/doc/"
menudir="/usr/local/share/applications/"

rm -f $bindir$program
rm -f $mandir$program.1
rm -f $infodir$program.info
rm -f -r $docdir$program
rm -f $bindir$program-uninstall.sh
rm -f $menudir$program.desktop

# remove old version if it exists
program="hddviewer"
rm -f $bindir$program
rm -f $mandir$program.1
rm -f $infodir$program.info
rm -f -r $docdir$program
rm -f $bindir$program-uninstall.sh