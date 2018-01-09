ARG=
CFLAGS=-Wall -g --std=c11

#実際にコンパイルするコマンド部分のファイル引数:以降を参照する?
pcp:token.c vector
	gcc $(CFLAGS) -o pcp vector token.c

vector:vector.*
	gcc $(CFLAGS) -c -o vector vector.c

stack:stack.*
	gcc $(CFLAGS) -c -o stack stack.c
revpol:stack revpol.c
	gcc $(CFLAGS) -o revpol stack revpol.c


clean:
	rm vector pcp

run:pcp
	./pcp $(ARG)
