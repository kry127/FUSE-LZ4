// напишем простейшую файловую систему по туториалу
// https://www.maastaar.net/fuse/linux/filesystem/c/2016/05/21/writing-a-simple-filesystem-using-fuse/

// Этот пример написан для осознания, как что работает, кое-что переделано
// Код очень багованный, но на то это Simple STUPID File System
#define FUSE_USE_VERSION 31

#include <fuse.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

int min(int a, int b) {
    if (a < b) return a;
    return b;
}

static int do_getattr( const char *path, struct stat *st )
{
	printf( "[getattr] Called\n" );
	printf( "\tAttributes of %s requested\n", path );

    st->st_uid = getuid();
	st->st_gid = getgid();
	st->st_atime = time( NULL );
	st->st_mtime = time( NULL );
    if ( strcmp( path, "/" ) == 0 )
	{
		st->st_mode = S_IFDIR | 0755;
		st->st_nlink = 2;
	}
	else if (strcmp( path, "/file54" ) == 0 || strcmp( path, "/file349" ) == 0 )
	{
		st->st_mode = S_IFREG | 0644;
		st->st_nlink = 1;
		st->st_size = 1024;
	} else {
		return -ENOENT; // оповещаем, что файлика всё-таки нет :)
	}
		
	return 0;
}

static int do_readdir( const char *path, void *buffer, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi )
{
	printf( "--> Getting The List of Files of %s\n", path );
	
	filler( buffer, ".", NULL, 0, 0); // Current Directory
	filler( buffer, "..", NULL, 0, 0); // Parent Directory
	
	if ( strcmp( path, "/" ) == 0 ) // If the user is trying to show the files/directories of the root directory show the following
	{
		filler( buffer, "file54", NULL, 0, 0);
		filler( buffer, "file349", NULL, 0, 0);
	}
	
	return 0;
}

static int do_read( const char *path, char *buffer, size_t size, off_t offset, struct fuse_file_info *fi )
{
    char file54Text[] = "Hello World From File54!";
	char file349Text[] = "Hello World From File349!";
	char *selectedText = NULL;

    if ( strcmp( path, "/file54" ) == 0 )
		selectedText = file54Text;
	else if ( strcmp( path, "/file349" ) == 0 )
		selectedText = file349Text;
	else
		return -1;

    size_t actually_read = min(strlen(selectedText + offset), size);
    
    memcpy( buffer, selectedText + offset, actually_read );
		
	return actually_read;
}

// notify fuse with our implemented functons
static struct fuse_operations operations = {
    .getattr	= do_getattr,
    .readdir	= do_readdir,
    .read	= do_read,
};

int main( int argc, char *argv[] )
{
	return fuse_main( argc, argv, &operations, NULL );
}