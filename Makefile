matrizes : main.c matriz.c matriz.h teste
	cc -std=c99 -o matrizes main.c matriz.c -lpthread 

teste : teste.c
	cc -o teste teste.c
