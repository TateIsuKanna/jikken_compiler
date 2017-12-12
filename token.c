#include<stdio.h>

char outtoken[200][50]; //token格納用配列
FILE *fi;
int check_word(char);

int main()
{
	int i,j,token_num;
	char c;
	fi=fopen("./pro1.p","r");
	token_num=0;  //トークンの数の初期化
	j=0;
	while(1){
		c=fgetc(fi);  //1文字ずつ入力
		printf("%c\n",c);
		if(c==EOF)break;

		if(check_word(c)==0){//数字なら 
			:
			:
		}
		else if(check_word(c)==1){//文字なら 
			:
			:
		}
		else if(check_word(c)==2){  //改行，スペース，タブなら
			if(j!=0){
				outtoken[token_num][j]='\0';
				j=0;
				token_num++;
			}
			else{
				//do nothing!!
			}
		}else if(check_word(c)==3){  //記号なら(1文字)
			:
			:
		} else if(check_word(c)==4){  //記号なら(2文字)
			:
			:
		}
	}
	
	for(i=0;i<token_num;i++)
		printf("%3d  %s\n",i,outtoken[i]);
	return 0;
}

int check_word(char c)
{
	if(c>='0' && c<='9') // 数字なら 
		return 0;
	else if(....)        //アルファベット
		return 1;
	else if(....)        //改行，スペース，タブ
		return 2;
	else if(....)        //1文字のみの記号
		return 3;
	else if(c==':')      //コロンは'='と必ず一緒になる
		return 4;
	else if(c=='<' || c=='>'){  //この場合は次の文字を見る必要がある
		c=fgetc(fi);
		fseek(fi,-1,1);   //ファイルポインタを1つ戻す 
		if(c=='=' || c=='>'){
			return 4;
		}else{
			return 3;
		}
	}
}


