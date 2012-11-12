
# 10 or 11
SOLARIS=11

export VERSION=1.1

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

ifeq ($(SOLARIS), 10)
BLKDEV=blkdev
else
BLKDEV=blkdev_illumos
endif

build: virtio vioblk $(BLKDEV)

virtio: virtio.c virtiovar.h virtioreg.h solaris-compat.h
	$(CC) $(CFLAGS) -c virtio.c -o virtio.o
#	$(CTFCONVERT) -i -L VERSION virtio.o
	$(LD) -r virtio.o -o virtio
#	$(CTFMERGE) -L VERSION -o virtio virtio.o

vioblk: vioblk.c virtiovar.h $(BLKDEV).h solaris-compat.h
	$(CC) $(CFLAGS) -c vioblk.c -o vioblk.o
#	$(CTFCONVERT) -i -L VERSION vioblk.o
	$(LD) -r -dy -N misc/virtio -N drv/$(BLKDEV) vioblk.o -o vioblk
#	$(CTFMERGE) -L VERSION -o vioblk vioblk.o


$(BLKDEV): $(BLKDEV).c $(BLKDEV).h solaris-compat.h cmlb_impl.h
	$(CC) $(CFLAGS) -c $(BLKDEV).c -o $(BLKDEV).o
#	$(CTFCONVERT) -i -L VERSION blkdev.o
	$(LD) -r -dy -Nmisc/cmlb $(BLKDEV).o -o $(BLKDEV)
#	$(CTFMERGE) -L VERSION -o blkdev blkdev.o

clean:
	rm -f virtio vioblk blkdev blkdev_illumos vioblk.o virtio.o blkdev.o blkdev_illumos.o
