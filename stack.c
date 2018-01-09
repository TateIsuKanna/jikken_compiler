#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include"stack.h"
void new_stack(struct stack* stack){
        stack->size=0;
        stack->memory_size=100;//この数に深い理由は無い
        stack->data=(char*)malloc(stack->memory_size);
}
void stack_push(struct stack* stack,char c){
        stack->data[stack->size]=c;
        stack->size++;
        if(stack->size>=stack->memory_size){
                stack->memory_size+=100;
                stack->data=(char*)realloc(stack->data,stack->memory_size);
        }
}
char stack_pop(struct stack* stack){
        if(stack->size<=0){
                fprintf(stderr,"Assertion Error out of stack\n");
                exit(-1);
        }
        stack->size--;
        return stack->data[stack->size];
}
char stack_peek(struct stack* stack){
        return stack->data[stack->size-1];
}
void stack_show_all(struct stack* stack){
	for(int i=0;i<stack->size;i++){
		printf("%c\n",stack->data[i]);
	}
}
