int fs_open(const char *pathname, int flags);
int fs_read(int fd, void *buf, int len);
int fs_write(int fd, const void *buf, int len);
//off_t fs_lseek(int fd, int offset, int whence);
int fs_close(int fd);

