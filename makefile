ARG=
CFLAGS=-Wall -g --std=c11

#実際にコンパイルするコマンド部分のファイル引数:以降を参照する?
pcp:token.c vector
	gcc $(CFLAGS) -o pcp vector token.c

vector:vector.*
	gcc $(CFLAGS) -c -o vector vector.c


clean:
	rm vector pcp

run:pcp
	./pcp $(ARG)
