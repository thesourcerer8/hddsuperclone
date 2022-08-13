#Make sure the prerequisite software is installed for unpacking and repacking the image. Open a terminal and run:
#sudo apt-get install squashfs-tools genisoimage

# The following 3 variables set the names and paths for the project files
# This example expects a directory of live_cd to exist in the home directory with source ubuntu.iso inside it
#set the working directory location
#don't add a / at the end here, that is taken care of in rest of the script
eval directory="~/live_cd"
#set the path to the source ISO
eval sourceiso="~/live_cd/ubuntu.iso"
#set the output ISO name (and path)
eval outputiso="~/live_cd/remastered.iso"

# A few things in case the build was aborted
sudo umount $directory/custom-img/mntiso
sudo umount $directory/custom-img/edit/dev

# Remove any files from previous build
sudo rm -rf $directory/custom-img
sudo rm $outputiso
sudo rm $outputiso.checksum

#Create a fresh folder to begin work.
mkdir -p $directory/custom-img
cd $directory/custom-img

#Next, extract the contents of disc image.
mkdir mntiso
sudo mount -o loop $sourceiso mntiso
mkdir extract
sudo rsync --exclude=/casper/filesystem.squashfs -a mntiso/ extract

#Extract the filesystem with the following commands:
sudo unsquashfs mntiso/casper/filesystem.squashfs
sudo mv squashfs-root edit

#You’re going to need network access from within the chroot environment to download and install updated/new packages. Copy resolv.conf from your system into the freshly unpacked fs.
sudo cp /etc/resolv.conf edit/etc/

#Mount a few important working directories, and perform a few commands will make sure that everything goes smoothly while modifying packages:
sudo mount --bind /dev/ edit/dev
sudo chroot edit /bin/bash -c "mount -t proc none /proc"
sudo chroot edit /bin/bash -c "mount -t sysfs none /sys"
sudo chroot edit /bin/bash -c "mount -t devpts none /dev/pts"
sudo chroot edit /bin/bash -c "export HOME=/root"
sudo chroot edit /bin/bash -c "export LC_ALL=C"
sudo chroot edit /bin/bash -c "dbus-uuidgen > /var/lib/dbus/machine-id"
sudo chroot edit /bin/bash -c "dpkg-divert --local --rename --add /sbin/initctl"
sudo chroot edit /bin/bash -c "ln -s /bin/true /sbin/initctl"

# copy any files needed to the home directory of the edit
# these could be third pary software to be installed
sudo cp path-to-a-file $directory/custom-img/edit/home
sudo cp -R path-to-a-directory/* $directory/custom-img/edit/home

# This next section allows for adding automated commands to run within chroot
cat << 'EOF' > $directory/custom-img/custom-commands.sh
###############################################################
###############################################################
###### PUT YOUR CUSTOMIZATION COMMANDS IN THE AREA BELOW ######
###############################################################

apt-get update

cd home
chmod -R 755 *
my-file-to-execute

###############################################################
###### PUT YOUR CUSTOMIZATION COMMANDS IN THE AREA ABOVE ######
###############################################################
###############################################################
EOF
sudo cp -f $directory/custom-img/custom-commands.sh $directory/custom-img/edit/
sudo chroot edit /bin/bash -c "chmod 755 custom-commands.sh"
sudo chroot edit /bin/bash -c "./custom-commands.sh"
sudo chroot edit /bin/bash -c "rm -f custom-commands.sh"

# Now we will open a chroot command prompt for any commands that you wish to enter manually
echo
echo "*************************************"
echo "You are now in chroot of the ISO."
echo "You can now run customization commands."
echo "When done type 'exit' and the build will continue."
echo "*************************************"
echo
sudo chroot edit

#You are almost there! Time to clean up:
sudo chroot edit /bin/bash -c "apt-get autoremove && apt-get autoclean"
sudo chroot edit /bin/bash -c "rm -rf /tmp/* ~/.bash_history"
sudo chroot edit /bin/bash -c "rm /var/lib/dbus/machine-id"
sudo chroot edit /bin/bash -c "rm /sbin/initctl"
sudo chroot edit /bin/bash -c "dpkg-divert --rename --remove /sbin/initctl"
sudo chroot edit /bin/bash -c "umount /proc || umount -lf /proc"
sudo chroot edit /bin/bash -c "umount /sys"
sudo chroot edit /bin/bash -c "umount /dev/pts"
sudo umount edit/dev

#These final steps will actually produce the ISO.
#Generate a new file manifest:
sudo chmod +w extract/casper/filesystem.manifest
sudo chroot edit dpkg-query -W --showformat='${Package} ${Version}n' > filesystem.manifest
sudo mv -f filesystem.manifest extract/casper/
sudo cp extract/casper/filesystem.manifest extract/casper/filesystem.manifest-desktop
sudo sed -i '/ubiquity/d' extract/casper/filesystem.manifest-desktop
sudo sed -i '/casper/d' extract/casper/filesystem.manifest-desktop

#Compress the filesystem, there are two options here, the first is faster, the second makes a smaller image:
#sudo mksquashfs edit extract/casper/filesystem.squashfs -b 1048576
sudo mksquashfs edit extract/casper/filesystem.squashfs -b 1M -comp xz -Xdict-size 100%

#Update filesystem size (needed by the installer):
printf $(sudo du -sx --block-size=1 edit | cut -f1) | sudo tee extract/casper/filesystem.size
echo

#Delete the old md5sum:
cd extract
sudo rm md5sum.txt

#…and generate a fresh one:
sudo su -c "find -type f -print0 | sudo xargs -0 md5sum | grep -v isolinux/boot.cat > md5sum.txt"

#And finally, create the ISO.
sudo genisoimage -D -r -V "$IMAGE_NAME" -cache-inodes -J -l -b isolinux/isolinux.bin -c isolinux/boot.cat -no-emul-boot -boot-load-size 4 -boot-info-table -o $outputiso .
sudo umount $directory/custom-img/mntiso

#create a checksum file for the iso, nice to have if you plan on sharing it
echo "md5sum:" > $outputiso.checksum
md5sum -b $outputiso >> $outputiso.checksum
echo "" >> $outputiso.checksum
echo "sha1sum:" >> $outputiso.checksum
sha1sum -b $outputiso >> $outputiso.checksum