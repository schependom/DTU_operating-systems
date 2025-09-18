#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <time.h>

int main(void)
{

  struct stat sb;

  stat("./11-stat.c", &sb);

  printf("File size:                %lld bytes\n", (long long)sb.st_size);
  printf("Last file access:         %s", ctime(&sb.st_atime));
  printf("Last file modification:   %s", ctime(&sb.st_mtime));

  /*
  Access to files is restricted by the FILE PERMISSIONS
     -> Have the form of a BITMAP of 9 bits
          -> 3 bits for the OWNER of the file
          -> 3 bits for the GROUP of the file
          -> 3 bits for OTHERS (everyone else)
  */

  return 0;
}
