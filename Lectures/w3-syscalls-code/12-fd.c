#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main(void) {

  dprintf(STDOUT_FILENO, "[%d] I print this in STDOUT (fd = %d) \n", getpid(), STDOUT_FILENO);

  dprintf(STDERR_FILENO, "[%d] I print this in STDERR (fd = %d) \n", getpid(), STDERR_FILENO);

  int fd = open("./temp.txt", O_WRONLY | O_CREAT, 0644);

  dprintf(fd, "[%d] I print this in file (fd = %d) \n", getpid(), fd);

  close(fd);

  return 0;
}
