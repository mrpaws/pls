/* pls.c - paw's 'ls' implementation
      just learning the VFS a bit by writing a tool with similar 
      functionality to the classic *NIX list tool
      TODO:
        file/dir decision
        consistent/proper error checking
        modularize
        stat files for perms (-l)
        data conversion
        output formatting
        optimization
        implement -l, -R, -1, -i, -d, -a, -h 
        sorting
      BUILD:
        gcc -o pls pls.c 
*/

#include <errno.h>
#include <stdio.h>
#include <dirent.h>

#define DEBUG 0

void debug_print_dirents (struct dirent *dentry) {
  printf(" +filename: %s\n +inodenum: %d\n +reclen: %d\n---\n", 
         dentry -> d_name,
         (int) dentry -> d_ino,
         (int) dentry -> d_reclen);
}

int ls (char *path) {
  DIR *dir_ptr;
  int retval = 0;
  int dirs_read = 0;
  struct dirent *cur_dentry;

  /* create a stream of dentries from path */
  if ((dir_ptr = opendir (path)) == NULL) {
      perror ("open error");
      return (1);
  }
 
  /* read each directory entry from the stream */
  do {
    if (cur_dentry = readdir(dir_ptr)) {
      dirs_read++;
      printf ("%s ", cur_dentry -> d_name);
      #if DEBUG > 0
        debug_print_dirents (cur_dentry);
      #endif /* debug for dir listing */
    }

    if (errno) {
      retval = 1;
      perror ("read error");
    }
  } while (cur_dentry);
  printf ("\n");

  /* close the directory stream */
  closedir (dir_ptr);
  if (errno) {
    retval = 1;
    perror ("close error");
  }

  return (retval);
}

int main (int argc, char **argv) {
  int retval = 0;

  --argc;
  ++argv;
  if (argc == 0)
    retval = ls (".");
  else
    while (argc-- > 0)
      retval |= ls(*argv++);

  return (retval);
}