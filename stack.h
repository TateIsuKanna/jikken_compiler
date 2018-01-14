struct stack{
        size_t size;
        size_t memory_size;
        char** data;
};
void new_stack(struct stack* stack);
void stack_push(struct stack* stack,char* str,size_t len);
char* stack_pop(struct stack* stack);

char* stack_peek(struct stack* stack);

void stack_show_all(struct stack* stack);
