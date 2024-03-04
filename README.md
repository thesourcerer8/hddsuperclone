# hddsuperclone
HDDSuperClone, HDDSuperTool

This repository continues the work on HDDSuperClone that was originally developed by (C) 2015-2022 Scott Dwyer and released under the GPL2 in 2022.
You can find the original website and more information here:
http://www.hddsuperclone.com/

Videos about HDDSuperClone and it's many features: https://www.youtube.com/channel/UCXfHI8OX-KE_1-Tc9aCkQTQ/videos

Other software that is used:
* SMTP Client: https://github.com/somnisoft/smtp-client
* GLADE: https://glade.gnome.org/


The Live-CD is using the following software:
* GNU ddrescue https://www.gnu.org/software/ddrescue/
* ddrescueview: https://sourceforge.net/p/ddrescueview
* ddrutility: https://sourceforge.net/projects/ddrutility/
* DMDE: https://dmde.com/
* hddscviewer: https://www.sdcomputingservice.com/hddscviewer
* hdparm: https://sourceforge.net/projects/hdparm/
* isolinux: https://wiki.syslinux.org/wiki/index.php?title=ISOLINUX


Building:
* sudo apt-get install git make gcc libusb-dev libgtk-3-dev libcurl4-openssl-dev libbsd-dev texi2html help2man
* git clone https://github.com/thesourcerer8/hddsuperclone
* cd hddsuperclone
* make
* sudo ./hddsuperclone
