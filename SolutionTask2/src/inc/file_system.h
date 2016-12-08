#ifndef __FILE_SYSTEM__
#define __FILE_SYSTEM__

#include <stdint.h>
#include <sys/types.h>

#define READ   0
#define WRITE  1
#define APPEND 3
#define MKDIR  4
#define OPEN   5

#define MAX_FILE_DIS_COUNT 256

struct node {
    char* name;
    uint64_t size;
    struct node* neighbor;
    struct node* child;
    void* file_start;
    uint8_t is_dir;
    uint64_t act_size;
};

struct file_descriptor {
    struct node* node;
    uint64_t current_position;
    int is_free;
};

struct file_descriptor file_descriptors[MAX_FILE_DIS_COUNT];

typedef struct node* DIR;

void file_system_init();
void file_system_print();

int open_file(const char* pathname , int flag);
int close_file(int fd);

ssize_t read(int fd, void* buf, size_t nbyte);
ssize_t write(int fd, const void* buf, size_t nbyte);


int mkdir(const char *path);
DIR* opendir(const char * path);
struct node* readdir(DIR* dirp);
void closedir(DIR* dir);


#endif /*__FILE_SYSTEM__*/