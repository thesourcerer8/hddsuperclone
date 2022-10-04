
make clean

mkdir -p ./pro/hddsuperclone
rm -rf ./pro/hddsuperclone/*
cp *.* ./pro/hddsuperclone
cp makefile ./pro/hddsuperclone
cp code_change_words ./pro/hddsuperclone
cp -R public ./pro/hddsuperclone
cp -R languages ./pro/hddsuperclone
cp -R driver ./pro/hddsuperclone
cp -R hddscripts ./pro/hddsuperclone

cd ./pro/hddsuperclone

rm -f conditional.h
cp -f public-conditional.h conditional.h
#changecode

make clean
make clone
sudo make installclone
mkdir -p ./installers
make DESTDIR=./installers installclone

md5sum -b hddsuperclone > md5sum
sha1sum -b hddsuperclone > sha1sum

cd ..
cd ..

