#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

/*
FD0 -> STDIN
FD1 -> STDOUT
FD2 -> STDERR
*/

/*
 `ls -1` lists files in a single column

    -> If we don't specify where to print, `printf` prints to STDOUT (file descriptor 1)
    -> With `dprintf`, we can specify the file descriptor to print to

    -> `ls -1 > temp.txt`
        -> The shell opens the file and creates a new file descriptor (fd) for it
        -> The shell redirects STDOUT (fd 1) to the new file descriptor
        -> The shell executes the `ls -1` command
        -> The `ls -1` command prints to STDOUT (fd 1), which is now the file descriptor for `temp.txt`
*/

/*
We can change the default standard input of wordcount `wc` by doing:
    -> `wc < temp.txt`
        -> The shell opens the file and creates a new file descriptor (fd) for it
        -> The shell redirects STDIN (fd 0) to the new file descriptor
        -> The shell executes the `wc` command
        -> The `wc` command reads from STDIN (fd 0), which is now the file descriptor for `temp.txt`
*/

int main(void)
{

  // ! dprintf is used to print to a specific file descriptor

  // STDOUT is file descriptor 1
  dprintf(STDOUT_FILENO, "[%d] I print this in STDOUT (fd = %d) \n", getpid(), STDOUT_FILENO);

  // ./a.out | ./a.out (PIPE)
  // The STDOUT of the first process is connected to the STDIN of the second process

  // STDERR is file descriptor 2
  dprintf(STDERR_FILENO, "[%d] I print this in STDERR (fd = %d) \n", getpid(), STDERR_FILENO);

  // Open (or create) a file for writing
  //  and get its file descriptor
  //  (a small integer that the kernel uses to identify the file)
  int fd = open("./temp.txt", O_WRONLY | O_CREAT, 0644);

  // the | operator is a bitwise OR
  //  it combines the flags O_WRONLY and O_CREAT

  // open(const char *pathname, int flags, mode_t mode);
  //
  //    -> pathname: path to the file
  //
  //    -> flags (O_ indicates it's a flag):
  //          -> O_WRONLY (open for writing only),
  //          -> O_CREAT (create the file if it doesn't exist)
  //
  //    -> mode: file permissions
  //          -> only used if the file is created (O_CREAT)
  //          -> in octal (base 8)
  //          -> translate to binary to see the permissions:
  //
  //                  3 bits for owner, 3 bits for group, 3 bits for others
  //
  //                  owner | group | others
  //                   xxx     xxx     xxx
  //                   |||     |||     |||
  //                   rwx     rwx     rwx
  //
  //          -> 0644 = 110 100 100 -> rw-r--r--
  //              |- owner can read and write
  //              |- group can read
  //              |- others can read

  dprintf(fd, "[%d] I print this in file (fd = %d) \n", getpid(), fd);

  close(fd);

  return 0;
}
