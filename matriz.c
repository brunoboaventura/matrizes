#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>

#include "matriz.h"

Matriz *
matriz_nova (int linhas, int colunas)
{
    Matriz *matriz;

    matriz = (Matriz *) calloc (1, sizeof (Matriz));

    matriz->linhas = linhas;
    matriz->colunas = colunas;
    matriz->m = NULL;
    
    if (matriz_alocar_memoria(matriz) == NULL)
    {
        free (matriz);
        return NULL;
    }
    
//    matriz_preencher (matriz);

    return matriz;
}

Matriz *
matriz_liberar (Matriz *matriz)
{
    if (matriz == NULL)
        return NULL;
    
    for (register int i = 0; i < matriz->linhas; i++)
        free (matriz->m[i]);
    
    free (matriz->m);

    free (matriz);

    return (NULL);
}

float**
matriz_alocar_memoria (Matriz *matriz)
{
    if (matriz->m != NULL)
        return matriz->m;

    if (matriz->linhas < 1 || matriz->colunas < 1)
        return (NULL);

    matriz->m = (float **) calloc (matriz->linhas, sizeof(float *));

    if (matriz->m == NULL)
        return (NULL);

    for (int  i = 0; i < matriz->linhas; i++ )
    {
        matriz->m[i] = (float *) calloc (matriz->colunas, sizeof(float));

        if (matriz->m[i] == NULL)
        {
            free (matriz->m);
            return (NULL);
        }
    }

    return (matriz->m);
}


void
matriz_preencher (Matriz *matriz)
{
    srand (time (NULL) + rand());

    for (register int i = 0; i < matriz->linhas; i++)
    {
        for (register int j = 0; j < matriz->colunas; j++)
        {
            matriz->m[i][j] = (float) rand() / RAND_MAX * 2.0 - 1.0;
        }
    }
}


void
matriz_imprimir (Matriz *matriz)
{
    for (register int i = 0; i < matriz->linhas; i++)
    {
        for (register int j = 0; j < matriz->colunas; j++)
        {
            printf ("%.10f\t", matriz->m[i][j]);
        }

        printf ("\n");
    }

    printf ("\n");
}


Matriz *
matriz_multiplicar (Matriz *matriz1, Matriz *matriz2, bool usa_registradores)
{
    float resultado;
    
    if (matriz_multiplicacao_possivel (matriz1, matriz2) == false)
        return NULL;

    
    // a nova matriz terá a mesma quantidade de
    // linhas da primeira e de colunas da segunda

    Matriz *matriz3 = matriz_nova (matriz1->linhas, matriz2->colunas);

    clock_t ciclos_de_clock[2];

    ciclos_de_clock[0] = clock();

    if (usa_registradores)
    {
        for (register int i = 0; i < matriz1->linhas; i++)
        {   
            for (register int j = 0; j < matriz2->colunas; j++)
            {
                resultado = 0.0;
                for (register int k = 0; k < matriz1->colunas; k++)
                {
                    resultado = resultado + (matriz1->m[i][k] * matriz2->m[k][j]);
                }
                matriz3->m[i][j] = resultado;
            }
        }
    }
    else
    {
        for (int i = 0; i < matriz1->linhas; i++)
        {
            for (int j = 0; j < matriz2->colunas; j++)
            {
                resultado = 0.0;
                for (int k = 0; k < matriz1->colunas; k++)
                {
                    resultado = resultado + (matriz1->m[i][k] * matriz2->m[k][j]);
                }
                matriz3->m[i][j] = resultado;
            }
        }

    }

    ciclos_de_clock[1] = clock();

    double tempo = (ciclos_de_clock[1] - ciclos_de_clock[0]) * 1000.0 / CLOCKS_PER_SEC;

    printf("\n---------------------------------------------------");
    printf("\nMultiplicação Linear");

    if (usa_registradores)
        printf (" usando registradores nos laços");

    printf("\n\nTempo gasto na multiplicação:\t %.0f ms.\n", tempo);

    return matriz3;
}


Matriz *
matriz_multiplicar_com_threads (Matriz * matriz1,
                                Matriz * matriz2)
{
    float resultado;

    Matriz *matriz3;

    clock_t ciclos_de_clock[2];

    if (matriz_multiplicacao_possivel (matriz1, matriz2) == false)
        return NULL;
    
    // a nova matriz terá a mesma quantidade de
    // linhas da primeira e de colunas da segunda

    matriz3 = matriz_nova (matriz1->linhas, matriz2->colunas);

    ciclos_de_clock[0] = clock();

    matriz_criar_threads (matriz1, matriz2, matriz3);

    ciclos_de_clock[1] = clock();

    float nucleos = sysconf(_SC_NPROCESSORS_ONLN);
   
    double tempo = (ciclos_de_clock[1] - ciclos_de_clock[0])
        * 1000.0 / CLOCKS_PER_SEC / nucleos;

    printf("\n---------------------------------------------------");
    printf("\nMultiplicação usando uma thread por núcleo");
    printf("\n\nTempo gasto na multiplicação:\t %.0f ms.\n", tempo);

    return matriz3;
}
                                

void
matriz_criar_threads (Matriz *matriz1,
                      Matriz *matriz2,
                      Matriz *matriz3)
{
    float nucleos = sysconf(_SC_NPROCESSORS_ONLN);

    int fracao = matriz1->linhas / nucleos;

    pthread_t threads[8];

    
    for (int i = 0; i < nucleos; i++)
    {
        DadosThread *dados = (DadosThread *) calloc (1, sizeof (DadosThread));

        dados->matriz1 = matriz1;
        dados->matriz2 = matriz2;
        dados->matriz3 = matriz3;
        dados->indice_inicial = i * fracao;

        if (i + 1 == nucleos)
            dados->indice_final = matriz1->linhas;
        else
            dados->indice_final = i * fracao + fracao;


        if (pthread_create(&threads[i], NULL, &thread_calcular, (void *)dados) != 0) 
            printf("\nErro ao criar a thread.\n");
    }

    for (int i = 0; i < nucleos; i++)
    {
        pthread_join (threads[i], NULL);
    }
}

void *
thread_calcular (void *dados)
{
    DadosThread *dados_thread = (DadosThread *) dados;

    int ini = dados_thread->indice_inicial;
    int fim = dados_thread->indice_final;

    Matriz *matriz1 = dados_thread->matriz1;
    Matriz *matriz2 = dados_thread->matriz2;
    Matriz *matriz3 = dados_thread->matriz3;
   
//    printf ("\n%d\t%d", dados_thread->indice_inicial, dados_thread->indice_final);

    float resultado;
    
    for (register int i = ini; i < fim; i++)
    {
        for (register int j = 0; j < matriz2->colunas; j++)
        {
            resultado = 0.0;
            for (register int k = 0; k < matriz1->colunas; k++)
            {
                resultado = resultado + (matriz1->m[i][k] * matriz2->m[k][j]);
            }
            matriz3->m[i][j] = resultado;
        }
    }
    
    free (dados);
}


bool
matriz_multiplicacao_possivel (Matriz *matriz1, Matriz *matriz2)
{
    if (matriz1->colunas == matriz2->linhas)
        return true;

    return false;
}


Matriz **matriz_quebrar (Matriz *matriz, int quantidade)                                          
{

    if (matriz->linhas < quantidade)
    {
        return NULL;
    }
    
    Matriz **lista_retorno = (Matriz **) calloc (quantidade, sizeof(Matriz *));                   
                                                                                                  
    int qtd_linhas = matriz->linhas / quantidade;                                                 
    int qtd_linhas_ultimo = qtd_linhas + (matriz->linhas % quantidade);                           

    printf ("%d, %d\n", qtd_linhas, qtd_linhas_ultimo);

    int i = 0;
    for (i = 0; i < quantidade -1; i++ )
    {
        printf ("%d\n", i);
        lista_retorno[i] = matriz_nova (qtd_linhas, matriz->colunas);
    }
    
    lista_retorno[i] = matriz_nova (qtd_linhas_ultimo, matriz->colunas);                         

    printf ("%d\n", i);


    int linha_todo = 0;

    for ( i = 0; i < quantidade; i++)
    {
        for (int linha = 0; linha < lista_retorno[i]->linhas; linha++)
        {
            for (int coluna = 0; coluna < lista_retorno[i]->colunas; coluna++)
            {
                lista_retorno[i]->m[linha][coluna] = matriz->m[linha_todo][coluna];
            }

            linha_todo++;
        }
    }
                                                                                                  
    return lista_retorno;                                                                         
}                                                                                                 
