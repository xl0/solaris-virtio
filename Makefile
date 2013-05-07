# 10 or 11
# XXX Solaris 11 support is not really there. More info: http://zaytsev.su/solaris-virtio/
SOLARIS=10

export VERSION=1.3

#CFLAGS=-D_KERNEL -DDEBUG -m64 -xarch=sse2a -xmodel=kernel -g
CFLAGS= -D_KERNEL -DDEBUG -m64 -mcmodel=kernel -mno-red-zone -ffreestanding
CFLAGS+= -nodefaultlibs -Wall -g
CFLAGS+= -DSOLARIS_COMPAT_VERSION=$(SOLARIS)

LDFLAGS=-r

ONDIR=/opt/onbld/bin/i386
CTFMERGE=$(ONDIR)/ctfmerge
CTFCONVERT=$(ONDIR)/ctfconvert


#CC=/opt/SUNWspro/bin/cc
CC=gcc
LD=/usr/ccs/bin/ld

all: build package

package:
	cd pkg-sol$(SOLARIS); ./mkpkg.sh

build: virtio vioblk blkdev dsktest dsktrial

virtio: virtio.c virtiovar.h virtioreg.h solaris-compat.h
	$(CC) $(CFLAGS) -c virtio.c -o virtio.o
#	$(CTFCONVERT) -i -L VERSION virtio.o
	$(LD) -r virtio.o -o virtio
#	$(CTFMERGE) -L VERSION -o virtio virtio.o

vioblk: vioblk.c virtiovar.h blkdev.h solaris-compat.h
	$(CC) $(CFLAGS) -c vioblk.c -o vioblk.o
#	$(CTFCONVERT) -i -L VERSION vioblk.o
	$(LD) -r -dy -N misc/virtio -N drv/blkdev vioblk.o -o vioblk
#	$(CTFMERGE) -L VERSION -o vioblk vioblk.o


blkdev: blkdev.c blkdev.h solaris-compat.h cmlb_impl.h
	$(CC) $(CFLAGS) -c blkdev.c -o blkdev.o
#	$(CTFCONVERT) -i -L VERSION blkdev.o
	$(LD) -r -dy -Nmisc/cmlb blkdev.o -o blkdev
#	$(CTFMERGE) -L VERSION -o blkdev blkdev.o

dsktest: dsktest.c
	$(CC) -Wall -m64 dsktest.c -o dsktest

dsktrial: dsktrial.c
	$(CC) -Wall -m64 dsktrial.c -o dsktrial

clean:
	rm -f virtio vioblk blkdev vioblk.o virtio.o blkdev.o
