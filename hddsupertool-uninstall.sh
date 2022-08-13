
program="hddsupertool"
scripts="hddscripts"
bindir="/usr/local/bin/"
infodir="/usr/local/share/info/"
oldmandir="/usr/share/man/man1/"
mandir="/usr/local/share/man/man1/"
docdir="/usr/local/share/doc/"

rm -f $bindir$program
rm -f $mandir$program.1
rm -f $infodir$program.info
rm -f -r $bindir$scripts
rm -f -r $docdir$program
rm -f $bindir$program-uninstall.sh