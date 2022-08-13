# This has been tested on xubuntu 14.04 32 bit and xubuntu 18.04 64 bit


# install stuff needed for making and building
sudo apt-get install gcc -y
sudo apt-get install g++ -y
sudo apt-get install make -y
sudo apt-get install libusb-dev -y
sudo apt-get install libcurl4-gnutls-dev -y
sudo apt-get install libreoffice-base -y
sudo apt-get install libreoffice-common -y
sudo apt-get install libssl-dev -y
sudo apt-get install p7zip-full -y
sudo apt-get install dh-make -y
sudo apt-get install devscripts -y
sudo apt-get install help2man -y
sudo apt-get install texinfo -y
sudo apt-get install ruby-dev build-essential -y
# the ruby version is too low for fpm in 14.04
# this seems to not work anymore, you are on your own for updating ruby on 14.04
# it is only needed to install fpm for making .deb and .rpm packages
# sudo apt-add-repository ppa:brightbox/ruby-ng -y
# sudo apt-get update
# sudo apt-get install ruby2.3 ruby2.3-dev -y
sudo gem install fpm
sudo apt-get install rpm -y
sudo apt-get install gtk+-2.0 -y
sudo apt-get install gtk+2.0 -y
sudo apt install libgtk-3-dev -y
sudo apt-get install squashfs-tools -y
sudo apt-get install genisoimage -y


# install things for glade
sudo apt-get install intltool -y
sudo apt-get install libxml2-dev -y
#this fails on 18.04
sudo apt-get install libgladeui-1-dev -y
#this does succeed on 18.04 but does not install what is needed
sudo apt-get install libgladeui-dev -y
sudo apt-get install -y itstool -y
sudo apt-get install libpango1.0-0 -y
# need to install libgladeui-1-11 from source on 18.04 for glade 3.8
sudo dpkg -i libgladeui-1-11_3.8.0-0ubuntu6_amd64.deb
# sudo dpkg -i libgladeui-1-11_3.8.0-0ubuntu6_i386.deb
# now need to install glade 3.8 from source
# install 3.8.5 then 3.8.6
tar xf glade3-3.8.5.tar.xz
cd glade3-3.8.5
./configure
make
sudo make install
cd ..
tar xf glade3-3.8.6.tar.xz
cd glade3-3.8.6
./configure
make
sudo make install
cd ..
# now install regular up to date glade
sudo apt install glade -y
# this software is built usling gtk2.0 which requires using glade 3.8
# to run glade 3.8 the command is glade-3


