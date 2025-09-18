#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <time.h>

int main(void)
{

    // This structure is defined in <sys/stat.h>
    // It contains information about a file
    struct stat sb;
    // It acts as a buffer to store the information read by the stat() system call

    // Get information about the file "11-stat.c"
    // and store it in the structure sb

    // int stat(const char *pathname, struct stat *statbuf);

    // This condition checks if the value of the systemcall variable is non-zero.
    // alt: if (stat(...) != 0)
    if (stat("./11-stat.c", &sb))
    {
        // Print to standard error
        perror("stat");
        // Terminate process with status EXIT_FAILURE (1)
        exit(EXIT_FAILURE);
    }

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
