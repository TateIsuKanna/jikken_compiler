#include <stdlib.h>
#include <string.h>
#include"vector.h"
void new_vector(struct vector* a){
        a->size=0;
        a->memory_size=100;
        a->data=(char**)malloc(sizeof(char**)*a->memory_size);
}
void vector_push(struct vector* a,char* str,size_t str_size){
        a->data[a->size]=(char*)malloc(str_size+1);
	strncpy(a->data[a->size],str,str_size);
	a->data[a->size][str_size]='\0';
        a->size++;
        if(a->size>=a->memory_size){
                a->memory_size*=2;
                a->data=(char**)realloc(a->data,sizeof(char**)*a->memory_size);
        }
}
