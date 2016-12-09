#include "file_system.h"
#include "alloc.h"
#include "threads.h"
#include "string.h"
#include "print.h"
#include "debug.h"


#define MAX_FILE_DIS_COUNT 256

struct file_descriptor {
    struct node* node;
    uint64_t current_position;
    int is_free;
};

struct file_descriptor file_descriptors[MAX_FILE_DIS_COUNT];

static struct node *root;

void file_system_init() {
	for (int i = 0; i < MAX_FILE_DIS_COUNT; ++i) {
		file_descriptors[i].is_free = 1;
	}

	root = (struct node*) mem_alloc(sizeof(struct node));
	root->neighbor = NULL;
	root->child = NULL;
    root->is_dir = 1;
    root->name = "";
    root->size = 0;
    root->file_start = NULL;

    printf("Initial FS\n");
}

static void parse_name_folder(char const *pathname, size_t *pos, size_t *new_len) {

	if (pathname[*pos] == '/' ) {
        ++(*pos);
    }

    *new_len = 0;
    while (pathname[*pos + *new_len] != 0) {
        if (pathname[*pos + *new_len] == '/') {
            break;
        }
        ++(*new_len);
    }
}

static struct node* create_new(char const *name, int flag){
	struct node *node =(struct node*) mem_alloc(sizeof(struct node));
	size_t size = strlen(name);
	if (flag == MKDIR)
		node->is_dir = 1;
	else 
		node->is_dir = 0;
	node->name = mem_alloc(size+1);
	node->name = memcpy(node->name, name, size);

	node->name[size] = 0;

	node->file_start = NULL;
	node->size = 0;

	node->child = NULL;
	node->neighbor = NULL;
	return node;

}

static struct node** __opendir(struct node *node){
	if (node == NULL || !node->is_dir )
		return NULL;
	struct node** res = mem_alloc(sizeof(struct node*));
	*res = node->child;
	return res;
}

static void __print(struct node* node, int v){

	for (int i = 0; i < v; ++i) {
		printf("    ");
	}
	printf("%s", node->name);
	if (node->is_dir){
		printf("/\n");
		struct node **dir = __opendir(node);

		struct node* val = readdir(dir);
		while(val != NULL){
			__print(val, v+1);
			val = readdir(dir);
		}
	}
	else
		printf("\n");
}

void file_system_print(){
	__print(root, 0);
}

int __strcmp(char const *s, char const *t, int len) {
    int i = 0;
    int size = strlen(s);
    for (; s[i] == t[i] && i<len; i++)
        if (s[i] == '\0')
            return 0;
    if (i == size )
    	return 0;
    else 
    	return 1;
}

static struct node* find_or_create(char const * pathname, struct node*(*create_new)(const char*, int flag), int flag){
	size_t pos = 0;
	size_t len = 0;
	parse_name_folder(pathname, &pos, &len);
	struct node* node = root;

	while(len != 0){
		if (!node->is_dir)
			return NULL;
		struct node* next = node->child;
	
		while(next != NULL  && 
			(strlen(next->name) != len  ||   
			(__strcmp(next->name, pathname + pos, len) == 1))) {

			next = next->neighbor;
		}
		if (!next){

			if (pathname[pos + len] == 0){
				if (flag == WRITE || flag == MKDIR){ 
					next = create_new(pathname + pos, flag);
					next->neighbor = node->child;
					node->child = next;
					return next; 
				}
				else {
					BUG("File not found!")
					return NULL;
				}

			}else{
				return NULL;
			}
		}
		node = next;
		int old_pos = pos;
        int old_len = len;
        pos = 0;
        parse_name_folder(pathname + old_pos + len , &pos, &len);
        pos = pos +  old_pos + old_len;
	}
	return node;
}



int open_file(char const *pathname, int flag) {
	thread_lock();
	int idx = 0;
	for (; idx < MAX_FILE_DIS_COUNT && 
		file_descriptors[idx].is_free == 0; ++idx) ;
	if (idx>=MAX_FILE_DIS_COUNT){
		thread_unlock();
		return -1;
	}
	file_descriptors[idx].node = find_or_create(pathname,create_new,flag);

	if (file_descriptors[idx].node == NULL){
		thread_unlock();
		return -1;
	}

	file_descriptors[idx].current_position = 0;
	file_descriptors[idx].is_free = 0;

	thread_unlock();
	return idx;

}

static int check_idx(int idx){
	if (idx < 0 || idx >= MAX_FILE_DIS_COUNT)
		return 0;
	if (file_descriptors[idx].is_free)
		return 0;
	return 1;
}

int close_file(int fd){
	thread_lock();
	if (!check_idx(fd)){
		thread_unlock();
		return -1;
	}
	file_descriptors[fd].is_free = 1;
	thread_unlock();
	return 0;
}

ssize_t read(int fd, void* buf, size_t nbyte){
	thread_lock();
	if (!check_idx(fd)){
		thread_unlock();
		return -1;
	}

	struct node* node = file_descriptors[fd].node;
	size_t i = 0;
	char* addres = node->file_start;
	for (; i < nbyte && file_descriptors[fd].current_position < node->size; ++i) {
		((char * )buf) [i] = *(addres + i);
		file_descriptors[fd].current_position++;
	}
	thread_unlock();
	return i;
}
ssize_t write(int fd, void const *buf, size_t nbyte){
	thread_lock();
	if (!check_idx(fd)){
		thread_unlock();
		return -1;
	}

	struct node* node = file_descriptors[fd].node;
	size_t i = 0;

	for (; i < nbyte; ++i){
		if (file_descriptors[fd].current_position == node->size){
			node->size = (node->size + 1 ) * 2;
			node->file_start = mem_realloc(node->file_start, node->size); 
		}
		int offset = file_descriptors[fd].current_position;
		*(((char*)node->file_start) + offset) = ((char*)buf)[i];
        file_descriptors[fd].current_position++;
	}

	thread_unlock();
	return nbyte;
}


int mkdir(char const *path){
	struct node* node = find_or_create(path, create_new, MKDIR);
	if (node == NULL)
		return -1;
	else 
		return 0;
}

struct node** opendir(char const * path){
	thread_lock();
	struct node* node = find_or_create(path, NULL,OPEN);
	struct node** dir = __opendir(node);
	thread_unlock();
	return dir;
}
struct node* readdir(struct node** dirp){
	thread_lock();
	struct node* dir = *dirp;
	if (dir == NULL){
		thread_unlock();
		return NULL;
	}
	*dirp = dir->neighbor;
	thread_unlock();
	return dir;
}
void closedir(struct node* dir){
	mem_free(dir);
}