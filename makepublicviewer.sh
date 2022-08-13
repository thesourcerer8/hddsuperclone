
FAST="false"
if [ "$1" = "fast" ]; then
  FAST="true"
  echo "fast mode active"
fi

TEST="false"
if [ "$2" = "test" ]; then
  TEST="true"
  echo "test mode active"
fi


./cleanpro.sh
./cleanpublicviewer.sh
make clean
make viewer
sudo make installviewer
if [ "$FAST" = "false" ]; then
  rm -f ./languages/hddscviewer/Language/*
  cd ./languages/hddscviewer/Language
  sudo hddscviewer --translate 80000
  cd ..
  cd ..
  cd ..
fi
./makeviewer.sh

make clean
make viewer

mkdir -p ./public/files_for_posting/hddscviewer/pro
mkdir -p ./public/files_for_posting/hddscviewer/installers

if [ "$TEST" = "false" ]; then
  rm -fr ./public/live_cd/pro/hddscviewer
  mkdir -p ./public/live_cd/pro/hddscviewer
fi

cp ./public/base/hddscviewer/changelog ./public/files_for_posting/hddscviewer/changelog.txt
cp ./public/base/hddscviewer/readme ./public/files_for_posting/hddscviewer/readme.txt
cp ./public/base/hddscviewer/EULA ./public/files_for_posting/hddscviewer/EULA.txt
cp ./public/base/hddscviewer/news ./public/files_for_posting/hddscviewer/news.txt
cp ./public/base/hddscviewer/makefile ./public/files_for_posting/hddscviewer

cp hddscviewer.odt ./public/files_for_posting/hddscviewer/help.odt
cp hddscviewer.txt ./public/files_for_posting/hddscviewer/help.txt
cp hddscviewer.html ./public/files_for_posting/hddscviewer/help.html
cp hddscviewer.pdf ./public/files_for_posting/hddscviewer/help.pdf

cp hddscviewer-uninstall.sh ./public/files_for_posting/hddscviewer/hddscviewer-uninstall.sh
cp -R languages/hddscviewer/Language ./public/files_for_posting/hddscviewer
cp hddscviewer.desk ./public/files_for_posting/hddscviewer/hddscviewer.desk

#cp install.sh ./public/files_for_posting/hddscviewer/install.sh
#cp uninstall.sh ./public/files_for_posting/hddscviewer/uninstall.sh

cp ./public/files_for_posting/hddscviewer/*.* ./public/files_for_posting/hddscviewer/pro
cp ./public/files_for_posting/hddscviewer/makefile ./public/files_for_posting/hddscviewer/pro
cp -R ./public/files_for_posting/hddscviewer/Language ./public/files_for_posting/hddscviewer/pro

cp ./pro/hddscviewer/hddscviewer ./public/files_for_posting/hddscviewer/pro/hddscviewer
cp ./pro/hddscviewer/md5sum ./public/files_for_posting/hddscviewer/pro/md5sum
cp ./pro/hddscviewer/sha1sum ./public/files_for_posting/hddscviewer/pro/sha1sum
cp -R ./pro/hddscviewer/installers ./public/files_for_posting/hddscviewer/installers/pro

if [ "$TEST" = "false" ]; then
  cp -R ./public/files_for_posting/hddscviewer/pro/* ./public/live_cd/pro/hddscviewer
fi


if [ "$(uname -m | grep '64')" != "" ]; then
  arch="x64"
  libcver=">= 2.14"
else
  arch="x86"
  libcver=">= 2.7"
fi
libcdepends="libc6"
glibdepends="glibc"

gtklibdepends="libgtk2.0-0"
ggtklibdepends="gtk2"

cairolibdepends="libcairo2"
gcairolibdepends="cairo"

version=$(head -1 ./public/base/hddscviewer/version)

basename="hddscviewer"
maintainer="sdcomputintservice <sdcomputintservice@gmail.com>"

cd ./public/files_for_posting/hddscviewer

name="$basename"
fullname="$name.$version.$arch"
cp -fr pro $fullname
tar -zcvf $fullname.tar.gz $fullname
rm -fr $fullname
#echo "md5sum:" > $fullname.tar.gz.checksum
#md5sum -b $fullname.tar.gz >> $fullname.tar.gz.checksum
#echo "" >> $fullname.tar.gz.checksum
#echo "sha1sum:" >> $fullname.tar.gz.checksum
#sha1sum -b $fullname.tar.gz >> $fullname.tar.gz.checksum
fpm -s dir -t deb -m "$maintainer" -C ./installers/pro --name $name --version $version --iteration 1 --depends "$libcdepends $libcver" --depends "$gtklibdepends" --depends "$cairolibdepends" --description "HDDSCViewer - A grahpical progress log viewer for HDDSuperClone." --url "http://www.sdcomputingservice.com" --vendor "sdcomputingservice" --license "FREEWARE" --before-install hddscviewer-uninstall.sh
fpm -s dir -t rpm -m "$maintainer" -C ./installers/pro --name $name --version $version --iteration 1 --depends "$glibdepends $libcver" --depends "$ggtklibdepends" --depends "$gcairolibdepends" --description "HDDSCViewer - A grahpical progress log viewer for HDDSuperClone." --url "http://www.sdcomputingservice.com" --vendor "sdcomputingservice" --license "FREEWARE" --before-install hddscviewer-uninstall.sh

echo "md5sum (binary):" > $fullname.checksums.txt
md5sum -b *.gz *.deb *.rpm >> $fullname.checksums.txt
echo "" >> $fullname.checksums.txt
echo "sha1sum (binary):" >> $fullname.checksums.txt
sha1sum -b *.gz *.deb *.rpm >> $fullname.checksums.txt

cd ..
cd ..
cd ..
./cleanpro.sh
make clean
chmod -R 777 public/files_for_posting/hddscviewer