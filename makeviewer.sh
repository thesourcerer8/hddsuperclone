# version 1.1

make clean

mkdir -p ./pro/hddscviewer
rm -rf ./pro/hddscviewer/*
cp *.* ./pro/hddscviewer
cp makefile ./pro/hddscviewer
cp -R public ./pro/hddscviewer
cp -R languages ./pro/hddscviewer

cd ./pro/hddscviewer

rm -f conditional.h
cp -f public-conditional.h conditional.h


make clean
make viewer
sudo make installviewer
mkdir -p ./installers
make DESTDIR=./installers installviewer

md5sum -b hddscviewer > md5sum
sha1sum -b hddscviewer > sha1sum

cd ..
cd ..

