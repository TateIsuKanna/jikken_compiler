#define _GNU_SOURCE
#include<stdio.h>
#include<ctype.h>
#include<string.h>
#include<stdlib.h>
#include<stdbool.h>
#include"vector.h"

struct vector tokens;
FILE *fi;

enum word_type{
	word_type_digit,
	word_type_alpha,
	word_type_separator,
	word_type_1char,
	word_type_2char
};

enum token_type{
        token_type_reserved,
        token_type_mark,
        token_type_ident,
        token_type_integer
};

enum word_type get_word_type(char* c){
	if(isdigit(*c)){
		return word_type_digit;
	}else if(isalpha(*c)){
		return word_type_alpha;
	}else if(*c=='\n' || *c==' ' || *c=='\t'){
		return word_type_separator;
	}else if((*c>='(' && *c<='/')||*c==';'||*c=='='){
		return word_type_1char;
	}else if(*c==':'){
		return word_type_2char;
	}else if(*c=='<' || *c=='>'){
		if(c[1]=='=' || c[1]=='>'){
			return word_type_2char;
		}else{
			return word_type_1char;
		}
	}
	return -1;
}

char* reserved_words[]={"begin","end","if","then","while","do","return","function","var","const","odd","write","writeln"};

void get_token(){
	char* line_str=NULL;
	size_t line_str_size=0;
	while(1){
		if(getline(&line_str,&line_str_size,fi)==EOF){
			break;
		}
		int line_p=0;
		//TODO:文字数取得を関数で切り分け?
		//switch
		while(1){
			if(isdigit(line_str[line_p])){
				int word_end_g=line_p;
				while(isdigit(line_str[++word_end_g]));
				vector_push(&tokens,line_str+line_p,word_end_g-line_p);
				line_p+=word_end_g-line_p;
			}else if(isalpha(line_str[line_p])){
				int word_end_g=line_p;
				//数字か文字か両方確認してから++
				while(isdigit(line_str[word_end_g])||isalpha(line_str[++word_end_g]));
				vector_push(&tokens,line_str+line_p,word_end_g-line_p);
				line_p+=word_end_g-line_p;
			}else if(get_word_type(line_str+line_p)==word_type_1char){
				vector_push(&tokens,line_str+line_p,1);
				line_p++;
			}else if(get_word_type(line_str+line_p)==word_type_2char){
				vector_push(&tokens,line_str+line_p,2);
				line_p+=2;
			}else if(line_str[line_p]=='\n'||line_str[line_p]=='\r'){
				break;
			}else{
				line_p++;
			}
		}
	}
	free(line_str);

	//TODO:この辺関数で切り分け，あと全体的にHACK
	for(int i=0;i<tokens.size;i++){
		printf("%3d  %s\t",i,tokens.data[i]);
		bool is_reserved_word=false;
		for(int word_search_i=0;word_search_i<sizeof(reserved_words)/sizeof(char*);++word_search_i){
			if(strcmp(reserved_words[word_search_i],tokens.data[i])==0){
				printf("予約語\n");
				is_reserved_word=true;
				break;
			}
		}
		if(is_reserved_word){
			continue;
		}
		switch(get_word_type(tokens.data[i])){
			case word_type_digit:
				printf("整数");
				break;
			case word_type_alpha:
				printf("名前");
				break;
			case word_type_1char:
			case word_type_2char:
				printf("記号");
				break;
                        case word_type_separator:
                                break;
		}
		puts("");
	}
}

//TODO:signとget_word_type
char* sign_words[]={"+","-","*","/","(",")","=","<",">","<>","<=",">=",",",".",";",":="};
enum token_type get_token_type(char* token){
        if(isdigit(*token)){
                return token_type_integer;
        }
        for(int word_search_i=0;word_search_i<sizeof(reserved_words)/sizeof(char*);++word_search_i){
                if(strcmp(reserved_words[word_search_i],token)==0){
                        printf("予約語\n");
                        return token_type_reserved;
                }
        }
        for(int word_search_i=0;word_search_i<sizeof(sign_words)/sizeof(char*);++word_search_i){
                if(strcmp(sign_words[word_search_i],token)==0){
                        printf("記号\n");
                        return token_type_mark;
                }
        }
        return token_type_ident;
}

bool iseqstr(char* a,char* b){
        return strcmp(a,b)==0;
}

int current_token_i;

void exit_by_error(char* str){
        printf("error %s",str);
        exit(-1);
}

void const_decl(){
        printf("begin const_decl\n");
        do{
                current_token_i++;
                if(get_token_type(tokens.data[current_token_i])!=token_type_ident){
                        exit_by_error("const_decl token_type_ident");
                }
                current_token_i++;
                if(!iseqstr(tokens.data[current_token_i],"=")){
                        exit_by_error("const_decl =");
                }
                current_token_i++;
                if(get_token_type(tokens.data[current_token_i])!=token_type_integer){
                        exit_by_error("const_decl token_type_integer");
                }
                current_token_i++;
        }while(iseqstr(tokens.data[current_token_i],","));
        if(iseqstr(tokens.data[current_token_i],";")){
                printf("end const_decl\n");
                current_token_i++;
        }else{
                exit_by_error("const_decl ;");
        }
}
void var_decl(){
}
void func_decl(){
}
void statement(){
}

void block(){
        while(1){
                if(iseqstr(tokens.data[current_token_i],"const")){
                        const_decl();
                }else if(iseqstr(tokens.data[current_token_i],"var")){
                        var_decl();
                }else if(iseqstr(tokens.data[current_token_i],"function")){
                        func_decl();
                }else{
                        break;
                }
        }
        statement();
}

int main(int argc,char *argv[]){
	if(argc!=2){
		return -1;
	}
	new_vector(&tokens);

	fi=fopen(argv[1],"r");
	if(!fi){
		return -1;
	}
        get_token();

        block();
}
