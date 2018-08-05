#include <stdio.h>
#include <stdlib.h>

#include "matriz.h"

int
main (int argc, char *argv[])
{
    if (argc != 3)
    {
        fprintf (stderr, "\n");
        fprintf (stderr, "Matrizes");
        fprintf (stderr, "\n");
        fprintf (stderr, "--------------------------------------------------------------------");
        fprintf (stderr, "\n");
        fprintf (stderr, "Realiza a multiplicação entre duas matrizes geradas aleatoriamente.");
        fprintf (stderr, "\n");
        fprintf (stderr, "O tamanho das matrizes é passado nos parâmetros da linha de comando, conforme instruções a seguir:");
        fprintf (stderr, "\n\n");
        fprintf (stderr, "Uso:");
        fprintf (stderr, "\n\n\t");
        fprintf (stderr, "matrizes LINHAS COLUNAS");
        fprintf (stderr, "\n\n\t");
        fprintf (stderr, "LINHAS  \t Inteiro");
        fprintf (stderr, "\n\t");
        fprintf (stderr, "COLUNAS \t Inteiro");
        fprintf (stderr, "\n");
        fprintf (stderr, "\n");

        return 1;
    }

    // Matriz *m;

    // int linhas  = atoi (argv[1]);
    // int colunas = atoi (argv[2]);

    // m = matriz_nova (linhas, colunas);

    // // Matriz *m1;
    // // m1 = matriz_nova (linhas, colunas);
    // matriz_preencher (m);
    // matriz_imprimir (m);

    // matriz_preencher (m);

    // matriz_imprimir (m);
    
    // Matriz **lista = matriz_quebrar (m, 3);

    // for (int i = 0; i < 3; i++)
    //     matriz_imprimir (lista[i]);
    
    
    // // printf ("%d", argc);
    // // printf ("\n");
    // // printf ("%d", atoi(argv[1]));
    // // printf ("\n");

    Matriz *matriz1;
    Matriz *matriz2;
    Matriz *matriz3;
    Matriz *matriz4;
    Matriz *matriz5;

    int linhas  = atoi (argv[1]);
    int colunas = atoi (argv[2]);


    matriz1 = matriz_nova (linhas, colunas);
    matriz2 = matriz_nova (linhas, colunas);

    /* a sintaxe matriz->m[][] pode ser usada aqui */

    matriz_preencher (matriz1);
    matriz_preencher (matriz2);

    matriz3 = matriz_multiplicar (matriz1, matriz2, false);
    matriz4 = matriz_multiplicar (matriz1, matriz2, true);
    matriz5 = matriz_multiplicar_com_threads (matriz1, matriz2);

    for (int i = 0; i < matriz3->linhas; i++)
    {
        for (int j = 0; j < matriz3->colunas; j++)
        {
            if ((matriz3->m[i][j] != matriz4->m[i][j])
                || (matriz3->m[i][j] != matriz5->m[i][j]))
            {
                printf ("\nAs matrizes resultantes de cada métodos contém valores divergentes\n");
            }
        }
    }

    matriz_imprimir (matriz1);
    matriz_imprimir (matriz2);
    // matriz_imprimir (matriz3);

    matriz_liberar (matriz1);
    matriz_liberar (matriz2);
    matriz_liberar (matriz3);
    matriz_liberar (matriz4);
    matriz_liberar (matriz5);

    return 0;
}
