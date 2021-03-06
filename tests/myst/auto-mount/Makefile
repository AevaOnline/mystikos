TOP=$(abspath ../../..)
include $(TOP)/defs.mak

APPDIR = $(SUBOBJDIR)/appdir
ROOTFS = $(SUBOBJDIR)/rootfs
RAMFSDIR = $(SUBOBJDIR)/ramfsdir
RAMFS = $(APPDIR)/ramfs
EXT2FSDIR = $(SUBOBJDIR)/ext2fsdir
EXT2FS = $(SUBOBJDIR)/ext2
EXT2FS_SIZE=134217728
HOSTFSDIR = $(SUBOBJDIR)/hostfsdir
HOSTFSDIR2 = $(SUBOBJDIR)/hostfsdir2
HOSTFSDIR3 = $(SUBOBJDIR)/hostfsdir3
CFLAGS = -fPIC -g
LDFLAGS = -Wl,-rpath=$(MUSL_LIB)

ifdef STRACE
OPTS = --strace
endif

all: rootfs ramfs ext2fs hostfs

build:	main.c
	mkdir -p $(APPDIR)/bin
	$(MUSL_GCC) $(CFLAGS) -o $(APPDIR)/bin/test main.c $(LDFLAGS)

rootfs: build ramfs
	mkdir -p $(APPDIR)/ramfs-target
	mkdir -p $(APPDIR)/ext2-target
	mkdir -p $(APPDIR)/hostfs-target
	mkdir -p $(APPDIR)/etc
	touch $(APPDIR)/etc/resolv.conf
	$(MYST) mkcpio $(APPDIR) $(ROOTFS)

ramfs:
	mkdir -p $(RAMFSDIR)
	mkdir -p $(APPDIR)
	touch $(RAMFSDIR)/ramfs-file
	$(MYST) mkcpio $(RAMFSDIR) $(RAMFS)

ext2fs:
	mkdir -p $(EXT2FSDIR)
	touch $(EXT2FSDIR)/ext2-file
	sudo $(MYST) mkext2 --force --size=$(EXT2FS_SIZE) $(EXT2FSDIR) $(EXT2FS) 
	truncate --size=-4096 $(EXT2FS)

hostfs:
	mkdir -p $(HOSTFSDIR)
	mkdir -p $(HOSTFSDIR2)
	mkdir -p $(HOSTFSDIR3)
	touch $(HOSTFSDIR)/hostfs-file
	echo "1.2.3.4" > $(HOSTFSDIR2)/resolv.conf
	touch $(HOSTFSDIR3)/testfile1
	touch $(HOSTFSDIR3)/testfile2

# This updates the config file with the roothash.
# currently not in use until the processing of the roothash is updated within the kernel
update-config: ramfs ext2fs hostfs
	$(MYST) fssig $(EXT2FS) > sig-info
	cat config1.json | sed 's/MOUNTEXT2ROOTHASH/$(shell grep root_hash= sig-info | cut -d= -f 2)/' > config.json
	rm sig-info

tests:
	$(MAKE) test-auto-mount

test-auto-mount: rootfs ext2fs hostfs
	$(RUNTEST) $(MYST_EXEC) $(ROOTFS) $(OPTS) --mount /ramfs=/ramfs-target --mount $(EXT2FS)=/ext2-target --mount $(HOSTFSDIR)=/hostfs-target \
	--mount $(HOSTFSDIR2)/resolv.conf=/etc/resolv.conf --mount $(HOSTFSDIR3)/testfile1=/targetfile1 --mount $(HOSTFSDIR3)/testfile2=/targetfile2 --app-config-path config1.json /bin/test test1

gdb: rootfs ext2fs hostfs
	$(MYST_GDB) --args $(MYST_EXEC) $(ROOTFS) $(OPTS) --mount /ramfs=/ramfs-target --mount $(EXT2FS)=/ext2-target --mount $(HOSTFSDIR)=/hostfs-target \
	--mount $(HOSTFSDIR2)/resolv.conf=/etc/resolv.conf --mount $(HOSTFSDIR3)/testfile1=/targetfile1 --mount $(HOSTFSDIR3)/testfile2=/targetfile2 --app-config-path config1.json /bin/test test1

clean:
	sudo rm -rf $(APPDIR) $(RAMFSDIR) $(EXT2FSDIR) $(HOSTFSDIR) $(HOSTFSDIR2) $(HOSTFSDIR3) $(RAMFS) $(EXT2FS) $(ROOTFS) sig-info config.json
