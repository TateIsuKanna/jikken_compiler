#include<stdio.h>
#include <ctype.h>

char tokens[200][50];
FILE *fi;

int check_word(char c){
	if(isdigit(c))
		return 0;
	else if(isalpha(c))
		return 1;
	else if(c=='\n' || c==' ' || c=='\t')
		return 2;
	else if((c>='(' && c<='/')||c==';'||c=='=')
		return 3;
	else if(c==':')      //コロンは'='と必ず一緒になるので
		return 4;
	else if(c=='<' || c=='>'){  //この場合は次の文字を見る必要がある
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

int main(){
	fi=fopen("p1","r");
	int token_num=0;
	while(1){
		char c=fgetc(fi);
		if(c==EOF){
                        break;
                }
		printf("%c\n",c);
		if(check_word(c)==0){//数字なら 
                        int char_p=0;
                        int string_length=0;
                        do{
                                string_length++;
                                tokens[token_num][char_p]=c;
                                char_p++;
                                c=fgetc(fi);
                        }while(check_word(c)==0);
                        tokens[token_num][char_p]='\0';
                        fseek(fi,-1,SEEK_CUR);
                        token_num++;
		}
		else if(check_word(c)==1){//文字なら 
                        int char_p=0;
                        int string_length=0;
                        do{
                                string_length++;
                                tokens[token_num][char_p]=c;
                                char_p++;
                                c=fgetc(fi);
                        }while(check_word(c)==1);
                        tokens[token_num][char_p]='\0';
                        fseek(fi,-1,SEEK_CUR);
                        token_num++;
		}
		//else if(check_word(c)==2){  //改行，スペース，タブなら
                //        int char_p=0;
		//	if(char_p!=0){
		//		tokens[token_num][char_p]='\0';
		//		char_p=0;
		//		token_num++;
		//	}
		//	else{
		//		//do nothing!!
		//	}
		//}
                else if(check_word(c)==3){  //記号なら(1文字)
                        tokens[token_num][0]=c;
                        tokens[token_num][1]='\0';
                        token_num++;
		} else if(check_word(c)==4){  //記号なら(2文字)
                        tokens[token_num][0]=c;
                        tokens[token_num][1]=fgetc(fi);
                        tokens[token_num][2]='\0';
                        token_num++;
		}
	}
	
	for(int i=0;i<token_num;i++){
		printf("%3d  %s\n",i,tokens[i]);
        }
}

