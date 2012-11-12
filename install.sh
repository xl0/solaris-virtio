#!/bin/sh

# This script is intended for quick testing, use the
# generated package to install the driver.


sudo cp vioblk /kernel/drv/amd64/
sudo cp virtio /kernel/misc/amd64/

if [ -e blkdev ]
then
	sudo cp blkdev /kernel/drv/amd64/
fi

if [ -e blkdev_illumos ]
then
	sudo cp blkdev_illumos /kernel/drv/amd64/
fi
