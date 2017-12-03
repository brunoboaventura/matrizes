#ifndef __MATRIZ_H__
#define __MATRIZ_H__

#include <stdbool.h>

typedef struct _Matriz Matriz;

struct _Matriz
{
        int linhas;
        int colunas;
        
        /*
        
        **m = ponteiro para a matriz na memória
        a sintaxe Matriz->m[i][j] é possível, respeitando os limites
        limites i < linhas e j < colunas

        */
        float **m;
};

typedef struct _DadosThread DadosThread;

struct _DadosThread
{
        Matriz *matriz1;
        Matriz *matriz2;
        Matriz *matriz3;

        int indice_inicial;
        int indice_final;
};
    


Matriz *matriz_nova (int linhas, int colunas);

Matriz *matriz_liberar (Matriz *matriz);

float **matriz_alocar_memoria (Matriz *matriz);

void matriz_preencher (Matriz *matriz);

void matriz_imprimir (Matriz *matriz);

Matriz *matriz_multiplicar (Matriz *matriz1,
                            Matriz *matriz2,
                            bool usa_registradores);

Matriz *matriz_multiplicar_com_threads (Matriz *matriz1,
                                        Matriz *matriz2);

void matriz_criar_threads (Matriz *matriz1,
                           Matriz *matriz2,
                           Matriz *matriz3);

void *thread_calcular (void *dados);

bool matriz_multiplicacao_possivel (Matriz *matriz1, Matriz *matriz2);

#endif /* __MATRIZ_H__ */
