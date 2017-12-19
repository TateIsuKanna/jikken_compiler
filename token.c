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
	word_type_2char,
};

int check_word(char* c){
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

int main(int argc,char *argv[]){
	if(argc!=2){
		return -1;
	}
	new_vector(&tokens);

	fi=fopen(argv[1],"r");
	if(!fi){
		return -1;
	}
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
			}else if(check_word(line_str+line_p)==word_type_1char){
				vector_push(&tokens,line_str+line_p,1);
				line_p++;
			}else if(check_word(line_str+line_p)==word_type_2char){
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
		switch(check_word(tokens.data[i])){
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
		}
		puts("");
	}
}
