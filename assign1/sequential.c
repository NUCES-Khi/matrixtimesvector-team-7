#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void fillMatrixVector(int *matrix, int *vector, int rows, int cols, int size);
void printMatrixVector(int *matrix, int *vector, int result[], int rows, int cols);
void matrixVectorMultiplication(int *matrix, int *vector, int result[], int rows, int cols);

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <matrix size> <vector size>\n", argv[0]);
        return 1;
    }

    int matrixSize = atoi(argv[1]);
    int vectorSize = atoi(argv[2]);
    int *matrix = (int *)malloc(matrixSize * vectorSize * sizeof(int));
    int *vector = (int *)malloc(vectorSize * sizeof(int));
    int *result = (int *)malloc(matrixSize * sizeof(int));

    if (matrix == NULL || vector == NULL || result == NULL) {
        printf("Memory allocation failed\n");
        return 1;
    }

    srand(time(NULL));
    fillMatrixVector(matrix, vector, matrixSize, vectorSize, 10);

    matrixVectorMultiplication(matrix, vector, result, matrixSize, vectorSize);
    printMatrixVector(matrix, vector, result, matrixSize, vectorSize);

    free(matrix);
    free(vector);
    free(result);
    return 0;
}
void fillMatrixVector(int *matrix, int *vector, int rows, int cols, int size) {
    for (int i = 0; i < rows * cols; ++i) {
        matrix[i] = rand() % size;
    }
    for (int i = 0; i < cols; ++i) {
        vector[i] = rand() % size;}
}
void printMatrixVector(int *matrix, int *vector, int result[], int rows, int cols) {
    printf("Matrix:\n");
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            printf("%d\t", matrix[i * cols + j]);
        }
        printf("\n");
    }
    printf("\nVector:\n");
    for (int i = 0; i < cols; ++i) {
        printf("%d\n", vector[i]);
    }
    printf("\nResult:\n");
    for (int i = 0; i < rows; ++i) {
        printf("%d\n", result[i]);
    }
}
void matrixVectorMultiplication(int *matrix, int *vector, int result[], int rows, int cols) {
    for (int i = 0; i < rows; ++i) {
        result[i] = 0;
        for (int j = 0; j < cols; ++j) {
            result[i] += matrix[i * cols + j] * vector[j];
        }
    }
}
