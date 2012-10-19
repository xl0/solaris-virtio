#!/bin/sh

cp ../virtio .
cp ../blkdev .
cp ../vioblk .

pkgmk -o
pkgtrans /var/spool/pkg `pwd`/SUNWvioblk-$VERSION SUNWvioblk
