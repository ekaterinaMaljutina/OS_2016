#ifndef __FILE_SYSTEM__
#define __FILE_SYSTEM__

#include <stdint.h>
#include <sys/types.h>

#define READ   0
#define WRITE  1
#define APPEND 3
#define MKDIR  4
#define OPEN   5

struct node {
    char* name;
    uint64_t size;
    struct node* neighbor;
    struct node* child;
    void* file_start;
    uint8_t is_dir;
};

void file_system_init();
void file_system_print();

int open_file(const char* pathname , int flag);
int close_file(int fd);

ssize_t read(int fd, void* buf, size_t nbyte);
ssize_t write(int fd, const void* buf, size_t nbyte);

int mkdir(const char *path);
struct node** opendir(const char * path);
struct node* readdir(struct node** dirp);
void closedir(struct node* dir);


#endif /*__FILE_SYSTEM__*/