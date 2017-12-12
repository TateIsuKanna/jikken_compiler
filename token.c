#include<stdio.h>
#include <ctype.h>
#include"vector.h"

struct vector tokens;
FILE *fi;

enum word_type{
        word_type_separator,
        word_type_1char,
        word_type_2char,
};

int check_word(char c){
	if(c=='\n' || c==' ' || c=='\t'){
		return word_type_separator;
        }else if((c>='(' && c<='/')||c==';'||c=='='){
		return word_type_1char;
        }else if(c==':'){
		return word_type_2char;
        }else if(c=='<' || c=='>'){
		c=fgetc(fi);
		fseek(fi,-1,1);
		if(c=='=' || c=='>'){
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
        if(fi==NULL){
                return -1;
        }
	while(1){
		char c=fgetc(fi);
		if(c==EOF){
                        break;
                }
		printf("%c\n",c);
		if(isdigit(c)){
                        int char_p=0;
                        vector_push(&tokens);
                        do{
                                tokens.data[tokens.size-1][char_p++]=c;
                                c=fgetc(fi);
                        }while(isdigit(c));
                        tokens.data[tokens.size-1][char_p]='\0';
                        fseek(fi,-1,SEEK_CUR);
		}
		else if(isalpha(c)){
                        int char_p=0;
                        vector_push(&tokens);
                        do{
                                tokens.data[tokens.size-1][char_p++]=c;
                                c=fgetc(fi);
                        }while(isdigit(c)||isalpha(c));
                        tokens.data[tokens.size-1][char_p]='\0';
                        fseek(fi,-1,SEEK_CUR);
		}else if(check_word(c)==word_type_1char){
                        vector_push(&tokens);
                        tokens.data[tokens.size-1][0]=c;
                        tokens.data[tokens.size-1][1]='\0';
		}else if(check_word(c)==word_type_2char){
                        vector_push(&tokens);
                        tokens.data[tokens.size-1][0]=c;
                        tokens.data[tokens.size-1][1]=fgetc(fi);
                        tokens.data[tokens.size-1][2]='\0';
                }
	}
	
        for(int i=0;i<tokens.size;i++){
                printf("%3d  %s\t",i,tokens.data[i]);
                int is_reserved_word=0;
                for(int word_search_i=0;word_search_i<sizeof(reserved_words)/sizeof(char*);++word_search_i){
                        if(strcmp(reserved_words[word_search_i],tokens.data[i])==0){
                                printf("予約語\n");
                                is_reserved_word=1;
                                break;
                        }
                }
                if(is_reserved_word){
                        continue;
                }
                if(isdigit(tokens.data[i][0])){
                        printf("整数");
                }else if(check_word(tokens.data[i][0])>=word_type_1char){
                        printf("記号");
                }else{
                        printf("名前");
                }
                puts("");
        }
}
