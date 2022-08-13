Readme for HDDSuperClone:

HDDSuperClone is an advanced tool for Linux to clone/image a drive.

HDDSuperClone is supplied as an executable, with separate ones for 32bit
and 64bit.  You CANNOT run the 32bit on a 64 bit system and vice versa
(you may get an odd file not found error if you try).

   HDDSuperClone now has DEB (Debian/Ubuntu) and RPM (RedHat/Fedora)
installers available.  If you have downloaded the appropriate installer
then you should just be able to double click on it to start the
installation process.  This method should even work on many live CDs,
but not all.  If you are unable to use one of the installers, then you
will need to follow the instructions below to install from the tar.gz
file or to run without installing.

   If you have downloaded the tar.gz file, then the easiest way to get
started is to copy the tar.gz file to a flash drive, and then use simple
copy and paste to put it in the Home folder that can be found on the
desktop of the Linux install.  When you open a terminal it should
default to the same Home folder that is on the desktop.

   To extract hddsuperclone, open a terminal and use the following
commands (replacing the -x.x-x.x.-xxx with proper version number and
architecture):
     gunzip hddsuperclone-x.x-x.x.-xxx-free.tar.gz
     tar xf hddsuperclone-x.x-x.x.-xxx-free.tar

   Then navigate to the proper directory:
     cd hddsuperclone-x.x-x.x.-xxx-free

   The following method to install HDDSuperClone will not only work on a
Linux installation, but you can use the same method when booting from a
live CD. The only difference is that every time you use a live CD, you
will need to perform these steps after each boot.

   To install hddsuperclone, use the following command:
     sudo make install

   The "make install" command needs to be run as root, which is why
"sudo" is included in this example.  Your sysem may use a different
command, or you may already be root.  If it is not ran as root, then you
will likely get permission errors and the install will not be complete.
Note that you can also uninstall it with the command "sudo make
uninstall".  There is now also an uninstaller script that can be ran by
typing "sudo hddsuperclone-uninstall.sh".

   You must run hddsuperclone as root.  For Ubuntu you would use "sudo"
in front of the command:
     sudo hddsuperclone
   Please consult the documentation for your version of Linux for
information on how to run as root.

   To run it without installing, you must be in the same directory as
hddsuperclone.  Note that some versions of Linux will not allow you to
run a program from certain external devices (such as a FAT32 USB drive).
Example to run it from the current directory:
     sudo ./hddsuperclone

   You may need to change the permissions on the file so that you have
the rights to run it.  The following command should do that:
     sudo chmod a+x hddsuperclone

   If you are booted from a live CD that does not allow installing with
make (maybe make does not exist) and you are trying to run it from a USB
drive and are getting a permission error, you can copy the executable to
the home folder and run it from there.  Note that if using a live CD the
home folder exists in ram and will be cleared on a reboot.  The
following example assumes you are in the folder on the USB drive that
contains hddsuperclone.  The first command copies it to the home folder,
the second command gives permission to execute, and the third command
runs it:
     sudo cp hddsuperclone /home
     sudo chmod a+x /home/hddsuperclone
     sudo /home/hddsuperclone

Please see the Installing and Usage sections of the manual for more info.
