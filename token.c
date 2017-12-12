#include<stdio.h>
#include <ctype.h>

char tokens[200][50];
FILE *fi;

enum word_type{
        word_type_separator,
};

int check_word(char c){
	if(c=='\n' || c==' ' || c=='\t'){
		return word_type_separator;
        }else if((c>='(' && c<='/')||c==';'||c=='='){
		return 3;
        }else if(c==':'){      //コロンは'='と必ず一緒になるので
		return 4;
        }else if(c=='<' || c=='>'){  //この場合は次の文字を見る必要がある
		c=fgetc(fi);
		fseek(fi,-1,1);
		if(c=='=' || c=='>'){
			return 4;
		}else{
			return 3;
		}
	}
        return -1;
}

char* reserved_words[]={"begin","end","if","then","while","do","return","function","var","const","odd","write","writeln"};

int main(){
	fi=fopen("p1","r");
	int token_num=0;
	while(1){
		char c=fgetc(fi);
		if(c==EOF){
                        break;
                }
		printf("%c\n",c);
		if(isdigit(c)){
                        int char_p=0;
                        int string_length=0;
                        do{
                                string_length++;
                                tokens[token_num][char_p]=c;
                                char_p++;
                                c=fgetc(fi);
                        }while(isdigit(c));
                        tokens[token_num][char_p]='\0';
                        fseek(fi,-1,SEEK_CUR);
                        token_num++;
		}
		else if(isalpha(c)){
                        int char_p=0;
                        int string_length=0;
                        do{
                                string_length++;
                                tokens[token_num][char_p]=c;
                                char_p++;
                                c=fgetc(fi);
                        }while(isdigit(c)||isalpha(c));
                        tokens[token_num][char_p]='\0';
                        fseek(fi,-1,SEEK_CUR);
                        token_num++;
		}else if(check_word(c)==3){  //記号なら(1文字)
                        tokens[token_num][0]=c;
                        tokens[token_num][1]='\0';
                        token_num++;
		}else if(check_word(c)==4){  //記号なら(2文字)
                        tokens[token_num][0]=c;
                        tokens[token_num][1]=fgetc(fi);
                        tokens[token_num][2]='\0';
                        token_num++;
                }
	}
	
        for(int i=0;i<token_num;i++){
                printf("%3d  %s\t",i,tokens[i]);
                int is_reserved_word=0;
                for(int word_search_i=0;word_search_i<13;++word_search_i){
                        if(strcmp(reserved_words[word_search_i],tokens[i])==0){
                                printf("予約語\n");
                                is_reserved_word=1;
                                break;
                        }
                }
                if(is_reserved_word){
                        continue;
                }
                if(isdigit(tokens[i][0])){
                        printf("整数");
                }else if(check_word(tokens[i][0])>=3){
                        printf("記号");
                }else{
                        printf("名前");
                }
                puts("");
        }
}
