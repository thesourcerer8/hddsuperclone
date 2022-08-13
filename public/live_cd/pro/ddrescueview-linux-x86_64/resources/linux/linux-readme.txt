Supplementary Linux files
=========================

The files in this folder (except this readme file) provide
additional integration into common Linux desktop environments.

They include
 - A manpage
 - Icons for the program in various sizes
 - A desktop file for starting the program through your main menu.
   To make this work, you need to have placed the ddrescueview
   executable in a directory on the $PATH, such as /usr/bin.

Copy the three directories - applications, icons, man - into your /usr/share directory.
These directories should already exist on your installation. Merge the new files into them.
You will need elevated privileges to perform this operation.
Afterwards, you may need to rebuild your icon cache, using
  update-icon-caches /usr/share/icons/hicolor
