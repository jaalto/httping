/* Released under GPLv2 with exception for the OpenSSL library. See license.txt */

ssize_t read_to(int fd, char *whereto, size_t len, int timeout);
ssize_t myread(int fd, char *whereto, size_t len, int timeout);
ssize_t mywrite(int fd, char *wherefrom, size_t len, int timeout);
int set_fd_nonblocking(int fd);
int set_fd_blocking(int fd);
