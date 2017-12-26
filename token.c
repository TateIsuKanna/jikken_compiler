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
	size_t line=1;
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
				vector_push(&tokens,line_str+line_p,word_end_g-line_p,line,line_p+1);
				line_p+=word_end_g-line_p;
			}else if(isalpha(line_str[line_p])){
				int word_end_g=line_p;
				while(isdigit(line_str[word_end_g])||isalpha(line_str[word_end_g])){
                                        word_end_g++;
                                }
				vector_push(&tokens,line_str+line_p,word_end_g-line_p,line,line_p+1);
				line_p+=word_end_g-line_p;
			}else if(get_word_type(line_str+line_p)==word_type_1char){
				vector_push(&tokens,line_str+line_p,1,line,line_p+1);
				line_p++;
			}else if(get_word_type(line_str+line_p)==word_type_2char){
				vector_push(&tokens,line_str+line_p,2,line,line_p+1);
				line_p+=2;
			}else if(line_str[line_p]=='\n'||line_str[line_p]=='\r'){
				break;
			}else{
				line_p++;
			}
		}
		line++;
	}
	free(line_str);

	//TODO:この辺関数で切り分け，あと全体的にHACK
	for(int i=0;i<tokens.size;i++){
		printf("%3d  %s %d:%d\t",i,tokens.data[i]->name,tokens.data[i]->line,tokens.data[i]->column);
		bool is_reserved_word=false;
		for(int word_search_i=0;word_search_i<sizeof(reserved_words)/sizeof(char*);++word_search_i){
			if(strcmp(reserved_words[word_search_i],tokens.data[i]->name)==0){
				printf("予約語\n");
				is_reserved_word=true;
				break;
			}
		}
		if(is_reserved_word){
			continue;
		}
		switch(get_word_type(tokens.data[i]->name)){
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
                        return token_type_reserved;
                }
        }
        for(int word_search_i=0;word_search_i<sizeof(sign_words)/sizeof(char*);++word_search_i){
                if(strcmp(sign_words[word_search_i],token)==0){
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
        fprintf(stderr,"%d:%d: error %s but '%s' is given\n",tokens.data[current_token_i]->line,tokens.data[current_token_i]->column,str,tokens.data[current_token_i]->name);
        exit(-1);
}

void print_token(char* str){
	printf("%s\t%s %d:%d\n",str,tokens.data[current_token_i]->name,tokens.data[current_token_i]->line,tokens.data[current_token_i]->column);
}



void const_decl(){
        while(1){
                if(get_token_type(tokens.data[current_token_i]->name)!=token_type_ident){
                        exit_by_error("expected const value ident");
                }
		print_token("const_decl");
                current_token_i++;
                if(!iseqstr(tokens.data[current_token_i]->name,"=")){
                        exit_by_error("expected '=' in const value declaration");
                }
		print_token("const_decl");
                current_token_i++;
                if(get_token_type(tokens.data[current_token_i]->name)!=token_type_integer){
                        exit_by_error("expected value at the const value declaration");
                }
		print_token("const_decl");
                current_token_i++;
		if(!iseqstr(tokens.data[current_token_i]->name,",")){
			break;
		}
		print_token("const_decl");
		current_token_i++;
	}
        if(iseqstr(tokens.data[current_token_i]->name,";")){
		print_token("const_decl");
                current_token_i++;
		return;
        }else{
                exit_by_error("expected ';' at the end of const value declaration");
        }
}
void var_decl(){
        while(1){
                if(get_token_type(tokens.data[current_token_i]->name)!=token_type_ident){
                        exit_by_error("expected valiable ident");
                }
		print_token("var_decl");
                current_token_i++;
		if(!iseqstr(tokens.data[current_token_i]->name,",")){
			break;
		}
		print_token("var_decl");
		current_token_i++;
        }
        if(tokens.data[current_token_i]->name[0]==';'){
		print_token("var_decl");
		current_token_i++;
                return;
        }else{
                exit_by_error("expected ';' at the end of valiable declaration");
        }
}
void block();
void func_decl(){
        if(get_token_type(tokens.data[current_token_i]->name)!=token_type_ident){
                exit_by_error("expected function name");
        }
	print_token("func_decl");
        current_token_i++;
        if(tokens.data[current_token_i]->name[0]!='('){
                exit_by_error("expected '(' in function definition");
        }
	print_token("func_decl");
        current_token_i++;
        //HACK:
        if(get_token_type(tokens.data[current_token_i]->name)==token_type_ident){
                while(1){
                        if(get_token_type(tokens.data[current_token_i]->name)!=token_type_ident){
                                exit_by_error("func_decl token_type_ident");
                        }
			print_token("func_decl");
                        current_token_i++;
                        if(!iseqstr(tokens.data[current_token_i]->name,",")){
                                break;
                        }
			print_token("func_decl");
                        current_token_i++;
                }
        }
        if(tokens.data[current_token_i]->name[0]!=')'){
                exit_by_error("expected ')' in function definition");
        }
	print_token("func_decl");
	current_token_i++;
        block();
        if(tokens.data[current_token_i]->name[0]==';'){
		current_token_i++;
                return;
        }else{
                exit_by_error("expected ';' at the end of function definition");
        }
}


void expression();
void factor(){
        if(get_token_type(tokens.data[current_token_i]->name)==token_type_ident){
		print_token("factor");
                current_token_i++;
                if(iseqstr(tokens.data[current_token_i]->name,"(")){
			print_token("factor");
			current_token_i++;
			//TODO:ここいい感じにまとめれそうじゃない? DRY!
			if(iseqstr(tokens.data[current_token_i]->name,")")){
				print_token("factor");
				current_token_i++;
				return;
			}
                        while(1){
				expression();
				if(!iseqstr(tokens.data[current_token_i]->name,",")){
					break;
				}
				current_token_i++;
			}
			if(iseqstr(tokens.data[current_token_i]->name,")")){
				print_token("factor");
				current_token_i++;
				return;
			}else{
				exit_by_error("expected ')' at the end of function arg");
			}
                }else{
                        return;
                }
        }
        if(get_token_type(tokens.data[current_token_i]->name)==token_type_integer){
		print_token("factor");
		current_token_i++;
                return;
        }
        if(tokens.data[current_token_i]->name[0]=='('){
		print_token("factor");
		current_token_i++;
                expression();
                if(tokens.data[current_token_i]->name[0]==')'){
			print_token("factor");
			current_token_i++;
                        return;
                }else{
			exit_by_error("expected ')' at the end of expression");
                }
        }
}

void term(){
        factor();
        while(1){
                if(tokens.data[current_token_i]->name[0]!='*'&&
                tokens.data[current_token_i]->name[0]!='/'){
                        break;
                }
		print_token("term");
		current_token_i++;
                factor();
        }
}

void expression(){
        if(tokens.data[current_token_i]->name[0]=='+'||
           tokens.data[current_token_i]->name[0]=='-'){
		print_token("expression");
		current_token_i++;
        }
        term();
        while(1){
                if(tokens.data[current_token_i]->name[0]!='+'&&
                tokens.data[current_token_i]->name[0]!='-'){
                        break;
                }
		print_token("expression");
                current_token_i++;
                term();
        }
}

void condition(){
        print_token("condition");
        if(iseqstr(tokens.data[current_token_i]->name,"odd")){
		print_token("condition");
		current_token_i++;
                expression();
                return;
        }
        expression();
        if(iseqstr(tokens.data[current_token_i]->name,"=")||
                iseqstr(tokens.data[current_token_i]->name,"<>")||
                iseqstr(tokens.data[current_token_i]->name,"<")||
                iseqstr(tokens.data[current_token_i]->name,">")||
                iseqstr(tokens.data[current_token_i]->name,"<=")||
                iseqstr(tokens.data[current_token_i]->name,">=")){
		print_token("condition");
		current_token_i++;
                expression();
        }else{
                exit_by_error("expected = <> <> <= >= ");
        }
}

void statement(){
        if(get_token_type(tokens.data[current_token_i]->name)==token_type_ident){
		print_token("statement");
                current_token_i++;
                if(iseqstr(tokens.data[current_token_i]->name,":=")){
			print_token("statement");
			current_token_i++;
                        expression();
                }else{
                        exit_by_error("statement :=");
                }
                return;
        }
        if(iseqstr(tokens.data[current_token_i]->name,"begin")){
                print_token("statement");
                while(1){
			current_token_i++;
                        statement();
                        if(tokens.data[current_token_i]->name[0]!=';'){
                                break;
                        }
			print_token("statement");
                }
                if(!iseqstr(tokens.data[current_token_i]->name,"end")){
                        exit_by_error("statement end");
                }
		print_token("statement");
		current_token_i++;
                return;
        }
        if(iseqstr(tokens.data[current_token_i]->name,"if")){
                print_token("statement");
		current_token_i++;
                condition();
                if(!iseqstr(tokens.data[current_token_i]->name,"then")){
                        exit_by_error("statement then");
                }
		print_token("statement");
		current_token_i++;
                statement();
                return;
        }
        if(iseqstr(tokens.data[current_token_i]->name,"while")){
                print_token("statement");
		current_token_i++;
                condition();
                if(!iseqstr(tokens.data[current_token_i]->name,"do")){
                        exit_by_error("statement do");
                }
		print_token("statement");
		current_token_i++;
                statement();
                return;
        }
        if(iseqstr(tokens.data[current_token_i]->name,"return")){
                print_token("statement");
		current_token_i++;
                expression();
                return;
        }
        if(iseqstr(tokens.data[current_token_i]->name,"write")){
                print_token("statement");
		current_token_i++;
                expression();
                return;
        }
        if(iseqstr(tokens.data[current_token_i]->name,"writeln")){
                print_token("statement");
		current_token_i++;
                return;
        }
}

void block(){
        while(1){
                if(iseqstr(tokens.data[current_token_i]->name,"const")){
                        print_token("block");
			current_token_i++;
                        const_decl();
                }else if(iseqstr(tokens.data[current_token_i]->name,"var")){
                        print_token("block");
			current_token_i++;
                        var_decl();
                }else if(iseqstr(tokens.data[current_token_i]->name,"function")){
                        print_token("block");
                        current_token_i++;
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
	if(tokens.size==0){
		return 0;
	}

        block();
        if(tokens.data[tokens.size-1]->name[0]=='.'){
                printf("completed\n");
        }else{
                exit_by_error("expected '.' at the end of program");
        }
}
