#define BASE_IMPLEMENTATION
#include "base.h"

int main(void){
	char* test = malloc(sizeof(char) * 2000);
	AsanPoisonMemoryRegion(test, 1000);
	char byte = (char) test[500];
	printf("test: %x", byte);
	return 0;
}
