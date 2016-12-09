#include <serial.h>
#include <memory.h>
#include <balloc.h>
#include <paging.h>
#include <debug.h>
#include <alloc.h>
#include <print.h>
#include <ints.h>
#include <time.h>

#include "lock.h"

#include "threads.h"

#include "file_system.h"

// #define DEBUG
static void qemu_gdb_hang(void)
{
#ifdef DEBUG
	static volatile int wait = 1;

	while (wait);
#endif
}

static void test_kmap(void)
{
	const size_t count = 1024;
	struct page **pages = mem_alloc(sizeof(*pages) * count);
	size_t i;

	BUG_ON(!pages);
	for (i = 0; i != count; ++i) {
		pages[i] = __page_alloc(0);
		if (!pages[i])
			break;
	}

	char *ptr = kmap(pages, i);

	BUG_ON(!ptr);
	BUG_ON((uintptr_t)ptr < HIGHER_BASE);

	for (size_t j = 0; j != i * PAGE_SIZE; ++j)
		ptr[i] = 13;

	for (size_t j = 0; j != i * PAGE_SIZE; ++j)
		BUG_ON(ptr[i] != 13);

	kunmap(ptr);
	mem_free(pages);
}

static void test_alloc(void)
{
	struct list_head head;
	unsigned long count = 0;

	list_init(&head);
	int i=0;
	while (i<100) {
		struct list_head *node = mem_alloc(sizeof(*node));

		if (!node)
			break;
		BUG_ON((uintptr_t)node < HIGHER_BASE);
		++count;
		list_add(node, &head);
		i++;
	}

	printf("Allocated %lu bytes\n", count * sizeof(head));

	while (!list_empty(&head)) {
		struct list_head *node = head.next;

		BUG_ON((uintptr_t)node < HIGHER_BASE);
		list_del(node);
		mem_free(node);
	}

	mem_alloc_shrink();
}

static void test_slab(void)
{
	struct list_head head;
	struct mem_cache cache;
	unsigned long count = 0;

	list_init(&head);
	mem_cache_setup(&cache, sizeof(head), sizeof(head));
	int i=0;
	while (i <100) {
		struct list_head *node = mem_cache_alloc(&cache);

		if (!node)
			break;
		BUG_ON((uintptr_t)node < HIGHER_BASE);
		++count;
		list_add(node, &head);
		i++;
	}

	printf("Allocated %lu bytes\n", count * sizeof(head));

	while (!list_empty(&head)) {
		struct list_head *node = head.next;

		BUG_ON((uintptr_t)node < HIGHER_BASE);
		list_del(node);
		mem_cache_free(&cache, node);
	}

	mem_cache_release(&cache);
}

static void test_buddy(void)
{
	struct list_head head;
	unsigned long count = 0;

	list_init(&head);
	int i = 0;
	while (i<100) {
	// while(1){
		struct page *page = __page_alloc(0);

		if (!page)
			break;
		++count;
		list_add(&page->ll, &head);
		i++;
	}

	printf("Allocated %lu pages\n", count);

	while (!list_empty(&head)) {
		struct list_head *node = head.next;
		struct page *page = CONTAINER_OF(node, struct page, ll);

		list_del(&page->ll);
		__page_free(page, 0);
	}
}

int use[(1 << 16)];

int value = 0;

void lock_1(void *arg) {
	int current = *(int*)arg;

	printf("lock1 current = %d\n",current );

	use[current] = 1;
	printf("%d %d\n", use[1], use[2]);

    printf("functon 1: %d; value = %d\n", current, value);

    use[current] = 2;
    printf("%d %d\n", use[1], use[2]);
    value = 1;

    printf("functon 1: %d; value =  %d\n",current, value);

}


void lock_2(void *arg) {
	int current = *(int*)arg;
	printf("lock2 current = %d\n",current );
    use[current] = 1;
    printf("%d %d\n", use[1], use[2]);
    use[current] = 2;

    printf("%d %d\n", use[1], use[2]);

    printf("functon 2: %d; value = %d  \n",current, value);

    value = 2;

    printf("functon 2:  %d; value =  %d\n",current, value);
}


void test_thread(){

	printf("create thread \n");

	int n1 = 1;
	int n2 = 2;

    use[n1] = 0;
    use[n2] = 0;
	thread_t *t1 = thread_create(lock_1, &n1);
    thread_t *t2 = thread_create(lock_2, &n2);


    printf(" thread created ... ok \n");

    printf("run join\n");
    thread_join(t1);
	thread_join(t2);
	printf("Finished join\n");

    printf("%d %d\n", use[1], use[2]);

    if (use[n1] == 2 && use[n2] == 2)
    	printf("OK\n");
    else
    	printf(" WRONG!!!\n");

    printf("return to main thread\n");
}

void main(void *bootstrap_info)
{
	qemu_gdb_hang();
	serial_setup();
	ints_setup();
	// time_setup();
	balloc_setup(bootstrap_info);
	paging_setup();
	page_alloc_setup();
	mem_alloc_setup();
	kmap_setup();
	// enable_ints();


	printf("Tests Begin\n");
	test_buddy();
	test_slab();
	test_alloc();
	test_kmap();
	printf("Tests Finished\n");

	init_threads();
	time_setup();
	

	// printf("test_thread .. \n");
	// test_thread();
	// printf("test_thread .. ok \n");



	printf("test file system\n");
	file_system_init();
	file_system_print();

	printf("check open file \n");
	
	int id = open_file("/file.txt", WRITE);
	printf("created file with id = %d\n",id );
	file_system_print();

	int id1 = open_file("/file1.txt", WRITE);
	printf("created file with id = %d\n",id1 );
	file_system_print();

	int dir = mkdir("/test_dir");
	printf("created dir with id = %d\n",dir);
	file_system_print();
	// printf("\n\n\n\n");
	int id2 = open_file("/test_dir/file2.txt", WRITE);
	printf("created file with id = %d\n",id2 );
	file_system_print();

	int dir_2 = mkdir("/test_dir_2");
	printf("created dir with id = %d\n",dir_2);
	file_system_print();

	int id3 = open_file("/test_dir_2/file3.txt", WRITE);
	printf("created file with id = %d\n",id3 );
	file_system_print();

	printf("\n");

	printf("write file \n");

	char val[3] = {'a','b', 'c'};
	ssize_t bytes = write(id,val,3);
	printf("write bytes = %d \n", bytes);

	close_file(id);
    id = open_file("/file.txt", READ);

	char buf[3];
	ssize_t bytes__ = read(id, buf, 3);
	printf("readed bytes__ = %d \n", bytes__);
	for (int i = 0; i < bytes__; ++i) {
		printf("%c",buf[i] );
	}
	printf("\n\n");

	close_file(id);
    id = open_file("/file.txt", WRITE);

	printf("write new value to file\n");
	char value[10] = "abcdefaaab";
	bytes = write(id, value,10);
	printf("write bytes = %d \n", bytes);
	
	close_file(id);
	id = open_file("/file.txt", READ);

	char buf_new[10];
	bytes__ = read(id, buf_new, bytes);
	printf("readed bytes__ = %d \n", bytes__);
	for (int i = 0; i < bytes__; ++i) {
		printf("%c",buf_new[i] );
	}
	printf("\n\n");

	printf("end of test\n");

	while (1);
}
