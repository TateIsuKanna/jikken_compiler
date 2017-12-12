CFLAGS=-Wall --std=c11

pcp:token.c vector
	gcc $(CFLAGS) -o pcp vector token.c

vector:vector.*
	gcc $(CFLAGS) -c -o vector vector.c
