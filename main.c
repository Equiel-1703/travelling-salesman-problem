#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include <time.h>

#define MTX_SIZE 50

int custo_caminho(int *caminho, int caminho_len, int matrix_distances[MTX_SIZE][MTX_SIZE])
{
    int i = 0;
    int size = 0;
    while (i + 1 < caminho_len)
    {
        size += matrix_distances[caminho[i]][caminho[i + 1]];
        ++i;
    }
    size += matrix_distances[caminho[i]][caminho[0]];

    return size;
}

void swap(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

void pinzon_rodrigues_lisboa(int *vertices, int start_verts, int end_verts, int *menor_caminho, int matrix_distances[MTX_SIZE][MTX_SIZE], int const matrix_size)
{
    if (start_verts == end_verts)
    {
        int temp_size = custo_caminho(vertices, matrix_size, matrix_distances);

        if (*menor_caminho > temp_size)
            *menor_caminho = temp_size;

        return;
    }
    else
    {
        // Recursive case: Generate permutations
        for (int i = start_verts; i <= end_verts; i++)
        {
            swap(&vertices[start_verts], &vertices[i]);
            pinzon_rodrigues_lisboa(vertices, start_verts + 1, end_verts, menor_caminho, matrix_distances, matrix_size);
            swap(&vertices[start_verts], &vertices[i]); // Backtrack
        }
    }
}

int main(int argc, char const *argv[])
{
    if (argc < 2)
    {
        printf("Voce deve inserir um arquivo de entrada!");
        exit(EXIT_FAILURE);
    }

    FILE *fp = fopen(argv[1], "r");

    if (fp == NULL)
    {
        printf("O arquivo %s nao existe ou esta restrito.", argv[1]);
        exit(EXIT_FAILURE);
    }

    int matrix_distances[MTX_SIZE][MTX_SIZE];
    char buffer[500], *ptr_buffer = buffer;
    int i = 0, j = 0;

    // Pega uma linha
    while (fgets(buffer, 500, fp))
    {
        while (*ptr_buffer) // Pega os numeros dessa linha
        {
            if (isdigit(*ptr_buffer))
            {
                // Encontrou um numero
                int val = strtol(ptr_buffer, &ptr_buffer, 10);
                matrix_distances[i][j++] = val;
            }
            else
                ptr_buffer++;
        }
        ++i;
        j = 0;
        ptr_buffer = buffer;
    }

    const int actualMatrixSize = i;

    // Printa a matriz lida
    for (i = 0; i < actualMatrixSize; ++i)
    {
        for (j = 0; j < actualMatrixSize; ++j)
        {
            printf("%3d ", matrix_distances[i][j]);
        }
        printf("\n");
    }

    printf("\n");

    int numbers_permutate[actualMatrixSize];

    for (i = 0; i < actualMatrixSize; i++)
    {
        numbers_permutate[i] = i;
    }

    int menor_caminho = INT_MAX;

    clock_t start = clock();
    pinzon_rodrigues_lisboa(numbers_permutate, 0, actualMatrixSize - 1, &menor_caminho, matrix_distances, actualMatrixSize);
    clock_t end = clock();
    float seconds = (float)(end - start) / CLOCKS_PER_SEC;

    printf("Num. de permutacoes: %d!\n", actualMatrixSize);
    printf("Menor caminho (usando Pinzon-Rodrigues-Lisboa): %d\n", menor_caminho);
    printf("Tempo: %f segundos", seconds);

    return 0;
}
