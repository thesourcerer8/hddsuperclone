
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
./cleanpublicclone.sh
make clean
make
sudo make installall
if [ "$FAST" = "false" ]; then
  rm -f ./languages/hddsuperclone/Language/*
  cp ./languages/hddsuperclone/Translated/* ./languages/hddsuperclone/Language/
  cd ./languages/hddsuperclone/Language
  sudo hddsuperclone --translate 80000
  cd ..
  cd ..
  cd ..
fi
make clean
./makeclone.sh

make clean
make clone

mkdir -p ./public/files_for_posting/hddsuperclone/pro
mkdir -p ./public/files_for_posting/hddsuperclone/installers

if [ "$TEST" = "false" ]; then
  rm -fr ./public/live_cd/pro/hddsuperclone
  mkdir -p ./public/live_cd/pro/hddsuperclone
fi

cp ./public/base/hddsuperclone/changelog ./public/files_for_posting/hddsuperclone/changelog.txt
cp ./public/base/hddsuperclone/readme ./public/files_for_posting/hddsuperclone/readme.txt
cp ./public/base/hddsuperclone/EULA ./public/files_for_posting/hddsuperclone/EULA.txt
cp ./public/base/hddsuperclone/news ./public/files_for_posting/hddsuperclone/news.txt
cp ./public/base/hddsuperclone/makefile ./public/files_for_posting/hddsuperclone

cp hddsuperclone.odt ./public/files_for_posting/hddsuperclone/hddsuperclone.odt
cp hddsuperclone.txt ./public/files_for_posting/hddsuperclone/hddsuperclone.txt
cp hddsuperclone.html ./public/files_for_posting/hddsuperclone/hddsuperclone.html
cp hddsuperclone.pdf ./public/files_for_posting/hddsuperclone/hddsuperclone.pdf
cp hddsupertool.txt ./public/files_for_posting/hddsuperclone/hddsupertool.txt
cp hddsupertool.html ./public/files_for_posting/hddsuperclone/hddsupertool.html
cp hddsuperclone.1 ./public/files_for_posting/hddsuperclone/man.1
cp hddsuperclone-uninstall.sh ./public/files_for_posting/hddsuperclone/hddsuperclone-uninstall.sh
cp hddsuperclone-install-depends-deb.sh ./public/files_for_posting/hddsuperclone/hddsuperclone-install-depends-deb.sh
cp hddsuperclone-install-depends-red.sh ./public/files_for_posting/hddsuperclone/hddsuperclone-install-depends-red.sh
cp -R languages/hddsuperclone/Language ./public/files_for_posting/hddsuperclone
cp hddsuperclone.desk ./public/files_for_posting/hddsuperclone/hddsuperclone.desk
cp hddsupertool.desk ./public/files_for_posting/hddsuperclone/hddsupertool.desk

#cp install.sh ./public/files_for_posting/hddsuperclone/install.sh
#cp uninstall.sh ./public/files_for_posting/hddsuperclone/uninstall.sh

cp ./public/files_for_posting/hddsuperclone/*.* ./public/files_for_posting/hddsuperclone/pro
cp ./public/files_for_posting/hddsuperclone/makefile ./public/files_for_posting/hddsuperclone/pro
cp -R ./public/files_for_posting/hddsuperclone/Language ./public/files_for_posting/hddsuperclone/pro

cp ./pro/hddsuperclone/hddsuperclone ./public/files_for_posting/hddsuperclone/pro/hddsuperclone
cp ./pro/hddsuperclone/md5sum ./public/files_for_posting/hddsuperclone/pro/md5sum
cp ./pro/hddsuperclone/sha1sum ./public/files_for_posting/hddsuperclone/pro/sha1sum
cp -R ./pro/hddsuperclone/installers ./public/files_for_posting/hddsuperclone/installers/pro

cp -r hddscripts ./public/files_for_posting/hddsuperclone/pro/hddscripts

if [ "$TEST" = "false" ]; then
  cp -R ./public/files_for_posting/hddsuperclone/pro/* ./public/live_cd/pro/hddsuperclone
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

version=$(head -1 ./public/base/hddsuperclone/version)

basename="hddsuperclone"
maintainer="sdcomputintservice <sdcomputintservice@gmail.com>"

cd ./public/files_for_posting/hddsuperclone

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
fpm -s dir -t deb -m "$maintainer" -C ./installers/pro --name $name --version $version --iteration 1 --depends "$libcdepends $libcver" --description "HDDSuperClone - An advanced hard drive cloning/imaging tool." --url "http://www.sdcomputingservice.com" --vendor "sdcomputingservice" --license "PROPRIETARY" --before-install hddsuperclone-uninstall.sh
fpm -s dir -t rpm -m "$maintainer" -C ./installers/pro --name $name --version $version --iteration 1 --depends "$glibdepends $libcver" --description "HDDSuperClone - An advanced hard drive cloning/imaging tool." --url "http://www.sdcomputingservice.com" --vendor "sdcomputingservice" --license "PROPRIETARY" --before-install hddsuperclone-uninstall.sh

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
chmod -R 777 public/files_for_posting/hddsuperclone