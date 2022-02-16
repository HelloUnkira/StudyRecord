
#include "memory_manage.h"

int main(int argc, char *argv[]) {
    
    test_memory_allocate_slab();
    test_memory_allocate_buddy();
    
	return 0;
}