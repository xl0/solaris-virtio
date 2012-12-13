#!/bin/sh

cp ../virtio .
cp ../blkdev .
cp ../vioblk .
cp ../dsktest .
cp ../dsktrial .

rm -f SUNWvioblk vioblk-$VERSION-itu.iso
rm -fr /var/spool/pkg/SUNWviobl

pkgmk -o
pkgtrans /var/spool/pkg `pwd`/SUNWvioblk-$VERSION SUNWvioblk
itu makedu -r 5.10 -o `pwd`/vioblk-$VERSION-itu.iso /var/spool/pkg/SUNWvioblk
