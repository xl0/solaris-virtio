#!/bin/sh

cp ../virtio .
cp ../blkdev .
cp ../vioblk .

pkgmk -o
pkgtrans /var/spool/pkg `pwd`/SUNWvioblk-$VERSION SUNWvioblk
itu makedu -r 5.10 -o `pwd`/vioblk-$VERSION-itu.iso /var/spool/pkg/SUNWvioblk
