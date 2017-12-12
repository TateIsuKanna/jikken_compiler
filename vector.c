#include <stdlib.h>
#include"vector.h"
void new_vector(struct vector* a){
        a->size=0;
        a->memory_size=100;
        a->data=(char**)malloc(sizeof(char**)*a->memory_size);
}
void vector_push(struct vector* a){
        a->data[a->size]=(char*)malloc(50);
        a->size++;
        if(a->size>=a->memory_size){
                a->data=(char**)realloc(a->data,sizeof(char**)*a->memory_size*2);
                a->memory_size*=2;
        }
}
