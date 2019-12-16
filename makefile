SOURCE_FS=fuse_filesystems/fuse_lz4.c
SOURCE_LZ4=lib/lz4.c lib/lz4frame.c lib/lz4hc.c lib/xxhash.c
C_INCLUDE_PATH=./lib
GCC_PARAMS=-I "$(C_INCLUDE_PATH)" -Wall


build:
	@echo "Building fuse-lz4 filesystem"
	gcc $(GCC_PARAMS) $(SOURCE_FS) $(SOURCE_LZ4) `pkg-config fuse3 --cflags --libs` -o fuse-lz4.fs

passthrough:
	@echo "Building passthrough_ll.fs filesystem"
	gcc $(GCC_PARAMS) fuse_filesystems/passthrough_ll.c `pkg-config fuse3 --cflags --libs` -o passthrough_ll.fs

ssfs:
	@echo "Building ssfs.fs -- Simple Stupid File System"
	gcc $(GCC_PARAMS) fuse_filesystems/ssfs.c `pkg-config fuse3 --cflags --libs` -o ssfs.fs

fuse_write_substituter:
	@echo "Building fuse_write_substituter.fs filesystem"
	gcc $(GCC_PARAMS) fuse_filesystems/fuse_write_substituter.c `pkg-config fuse3 --cflags --libs` -o fuse_write_substituter.fs

framecompress:
	@echo "Building framecompress.o -- executable for testing lz4 compression"
	gcc $(GCC_PARAMS) fuse_filesystems/frameCompress.c $(SOURCE_LZ4) -o frameCompress.o

clean:
	rm -f *.fs *.lz4 *.lz4.dec *.o *.out