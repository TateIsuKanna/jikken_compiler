#define _GNU_SOURCE
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#include"stack.h"

int token_priority(char c){
        switch(c){
		case '*' :
		case '/' :  
                return 3;
		case '+' :
		case '-' :  
                return 2;
		case '=' :
		case '(' :
                return 1;
		case ')' :
                default:
                return 0;
        }
}
void three_address_code(char* inputstring){
        struct stack code_stack;
        new_stack(&code_stack);
        size_t count=0;
	for(int i=0;i<strlen(inputstring);i++){
		if(inputstring[i]>='A' && inputstring[i]<='Z'){
                        stack_push(&code_stack,inputstring+i,1);
                }else if(inputstring[i]=='='){
                        char* t1=stack_pop(&code_stack);
                        char* t2=stack_pop(&code_stack);

                        printf("%c(%s,%s)\n",inputstring[i],t1,t2);
                        printf("LD %s\n",t1);
                        printf("ST %s\n",t2);

                        count++;
                }else if(inputstring[i]!='='){
                        char* t1=stack_pop(&code_stack);
                        char* t2=stack_pop(&code_stack);

			char* work_str;
			asprintf(&work_str,"W%d",count);
                        stack_push(&code_stack,work_str,strlen(work_str));

                        printf("%c(%s,%s,%s)\n",inputstring[i],t2,t1,work_str);
                        printf("LD %s\n",t2);
                        switch(inputstring[i]){
                                case '+':
                                        printf("AD");
                                        break;
                                case '-':
                                        printf("SB");
                                        break;
                                case '*':
                                        printf("ML");
                                        break;
                                case '/':
                                        printf("DV");
                                        break;
                        }
                        printf(" %s\n",t1);

                        printf("ST %s\n",work_str);
                        count++;
                }
        }
}

bool is_eqstr(char* a,char* b){
	return strcmp(a,b)==0;
}

int main(){
        char* inputstring;
        scanf("%ms",&inputstring);


	for(int i=0;i<strlen(inputstring);i++){
		printf("%c ",inputstring[i]);
	}
	puts("");

        struct stack parsestack;
        new_stack(&parsestack);

        char* outstring=(char*)malloc(strlen(inputstring)+1);//NULL文字分+1
        outstring[strlen(inputstring)]='\0';
        int outstring_i=0;

	//演算子などはとりあえず1文字としている(後で自由に変えよう)

	for(int i=0;i<strlen(inputstring);i++){
		if(inputstring[i]>='A' && inputstring[i]<='Z'){
			strncpy(outstring+outstring_i,inputstring+i,1);
			outstring_i++;
		}else if(inputstring[i]=='('){
                        stack_push(&parsestack,inputstring+i,1);
		}else if(inputstring[i]==')'){
                        while(1){
                                char* str=stack_pop(&parsestack);
                                if(is_eqstr(str,"(")){
                                        break;
                                }
				strcpy(outstring+outstring_i,str);
				outstring_i+=strlen(str);
                        };
		}else{
			if(parsestack.size==0){  //スタックが空の場合無条件にプッシュ
				stack_push(&parsestack,inputstring+i,1);
			}else if(token_priority(inputstring[i])>token_priority(stack_peek(&parsestack)[0])){   //inputstringの優先>topの優先
                                stack_push(&parsestack,inputstring+i,1);
			}else{                       //inputstringの優先<=topの優先
                                while(1){
                                        char* str=stack_pop(&parsestack);
					strncpy(outstring+outstring_i,str,1);
					outstring_i++;
                                        if(token_priority(inputstring[i])<=token_priority(str[0])){
                                                break;
                                        }
                                }
                                stack_push(&parsestack,inputstring+i,1);
			}
		}
	}

	while(1){
                if(parsestack.size==0){
                        break;
                }
		strcpy(outstring+outstring_i,stack_pop(&parsestack));
		outstring_i++;
        }

        puts("");
	printf("outstring : %s\n",outstring);

        three_address_code(outstring);
}

