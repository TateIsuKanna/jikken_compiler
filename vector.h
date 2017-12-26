struct token{
	char* name;
	size_t line;
	size_t column;
};
struct vector{
        size_t size;
        size_t memory_size;
        struct token** data;
};
void new_vector(struct vector* a);
void vector_push(struct vector* a,char* str,size_t str_size,size_t line,size_t column);
