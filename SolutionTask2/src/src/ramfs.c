#include "ramfs.h"
#include "print.h"
#include "memory.h"
#include "alloc.h"
#include "string.h"
#include "stdint.h"
#include "file_system.h"

#define CHECK_FLAG(flags,bit)   ((flags) & (1 << (bit)))

struct mboot_info
 {
   /* Multiboot info version number */
   uint32_t flags;

  	uint8_t ignore0[16];

   /* Boot-Module list */
   uint32_t mods_count;
   uint32_t mods_addr;
 } __attribute__((packed));


struct multiboot_mod_list
 {
   /* the memory used goes from bytes 'mod_start' to 'mod_end-1' inclusive */
   uint32_t mod_start;
   uint32_t mod_end;

   /* Module command line */
   uint32_t cmdline;

   /* padding to take it to 16 bytes (must be zero) */
   uint32_t pad;	
 };
typedef struct multiboot_mod_list multiboot_module_t;


uintptr_t current_addr;

uintptr_t addr_start;
uintptr_t addr_end;

static uint32_t atoi_(char* s, size_t len) {
    uint32_t x = 0;
    for (size_t i = 0; i < len; ++i) {
        x = x * 16 ;
        if (s[i] >= '0' && s[i] <= '9') {
            x += s[i] - '0';
        } else 
        	if (s[i] >= 'a' && s[i] <= 'z'){
            	x += s[i] - 'a' + 10;
	        } else {
	            x += s[i] - 'A' + 10;	
        }
    }

    return x;
}

static  void aling(){
	if ((uint64_t)current_addr % 4 == 0)
	        	current_addr -= 4;
    current_addr += 4 - ((uint64_t)current_addr)%4;
}

static int add_file() {

    aling();

    if (current_addr >= addr_end) {
        return 0;
    }

    struct cpio_header* header = (struct cpio_header*)va(current_addr);
    current_addr += sizeof(struct cpio_header);

    uint32_t name_len = atoi_(header->namesize, 8);


    char* name = mem_alloc(name_len + 1);
    
    uintptr_t addr_for_free = (uintptr_t) name;

    memcpy(name, (char*)va(current_addr), name_len );
    name[name_len] = 0;

    current_addr += name_len;

    if (name_len == 11 && __strcmp(name, END_OF_ARCHIVE, 11) == 0) {
        return 0;
    }

    uint32_t mode = atoi_(header->mode, 8);

    if (S_ISDIR(mode)) {
        mkdir(name);
    } 
    else 
    	if (S_ISREG(mode)) {
	        int file = open_file(name, WRITE);

	        uint32_t filesize = atoi_(header->filesize, 8);

	       	aling();

	        write(file, va(current_addr), filesize);

	        close_file(file);

	        current_addr += filesize;
	    }

    mem_free((void*)addr_for_free);

    return 1;
}

void initramfs_to_fs() {
	printf("initramfs to fs\n");
    current_addr = addr_start;
    int i = add_file(); 
    while ( i) {
    	i = add_file();
    }	
	file_system_print();
	printf("succes to fs\n");
    
}


void init_ramfs(const struct mboot_info* mbi_, uintptr_t* addr, uintptr_t* end){
	struct mboot_info* mbi = (struct mboot_info*)mbi_;
	if (CHECK_FLAG (mbi->flags, 3)) {
	    multiboot_module_t *mod;
	    size_t i;

	    printf ("mods_count = %d, mods_addr = 0x%x\n",
	           (int) mbi->mods_count, (int) mbi->mods_addr);
	    for (i = 0, mod = (multiboot_module_t* )(uint64_t) mbi->mods_addr;
	        i < mbi->mods_count;
	        i++, mod++){
    	     printf (" mod_start = 0x%x, mod_end = 0x%x, cmdline = %s\n",
	             (unsigned) mod->mod_start,
	             (unsigned) mod->mod_end,
	             (char *)(uint64_t)  mod->cmdline);

            addr_start = ((uintptr_t) (uint64_t) mod->mod_start);
            addr_end =  ((uintptr_t) (uint64_t) mod->mod_end);
	        if ( (uint64_t)addr - (uint64_t)end >= 6) {

	            ((char*)addr_start)[1] = '7';
	            ((char*)addr_start)[2] = '0';
	            ((char*)addr_start)[3] = '7';
	            ((char*)addr_start)[0] = '0';
	            ((char*)addr_start)[4] = '0';
	            ((char*)addr_start)[5] = '1';
	            *addr =  ((uintptr_t ) (uint64_t) mod->mod_start);
            	*end =  (uintptr_t ) (uint64_t) mod->mod_end ;
	            return;
	        }
        }

	     
	}
 }


 
