struct vector{
        int size;
        int memory_size;
        char** data;
};
void new_vector(struct vector* a);
void vector_push(struct vector* a,char* str,size_t str_size);
