#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

void fillMatrixVector(int *matrix, int *vector, int rows, int cols, int size);
void printMatrixVector(int *matrix, int *vector, int result[], int rows, int cols);
void matrixVectorMultiplication(int *matrix, int *vector, int result[], int rows, int cols, int rank, int size);

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <matrix size> <vector size>\n", argv[0]);
        return 1;
    }

    int matrixSize = atoi(argv[1]);
    int vectorSize = atoi(argv[2]);

    MPI_Init(&argc, &argv);
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    if (matrixSize % world_size != 0) {
        if (world_rank == 0) {
            printf("Size must be divisible by the number of processes.\n");
        }
        MPI_Finalize();
        return 1;
    }

    int localRows = matrixSize / world_size;

    int *matrix = (int *)malloc(localRows * vectorSize * sizeof(int));
    int *vector = (int *)malloc(vectorSize * sizeof(int));
    int *result = (int *)malloc(localRows * sizeof(int));

    if (matrix == NULL || vector == NULL || result == NULL) {
        printf("Memory allocation failed\n");
        MPI_Finalize();
        return 1;
    }

    fillMatrixVector(matrix, vector, localRows, vectorSize, 10);
    matrixVectorMultiplication(matrix, vector, result, localRows, vectorSize, world_rank, world_size);

    int *gatheredResult = NULL;
    if (world_rank == 0) {
        gatheredResult = (int *)malloc(matrixSize * sizeof(int));
    }
    MPI_Gather(result, localRows, MPI_INT, gatheredResult, localRows, MPI_INT, 0, MPI_COMM_WORLD);
    if (world_rank == 0) {
        printMatrixVector(matrix, vector, gatheredResult, matrixSize, vectorSize);
        free(gatheredResult);
    }

    free(matrix);
    free(vector);
    free(result);
    MPI_Finalize();
    return 0;
}

void fillMatrixVector(int *matrix, int *vector, int rows, int cols, int size) {
    for (int i = 0; i < rows * cols; ++i) {
        matrix[i] = rand() % size;
    }
    for (int i = 0; i < cols; ++i) {
        vector[i] = rand() % size;
    }
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
void matrixVectorMultiplication(int *matrix, int *vector, int result[], int rows, int cols, int rank, int size) {
    int *localResult = (int *)malloc(rows * sizeof(int));

    for (int i = 0; i < rows; ++i) {
        localResult[i] = 0;
        for (int j = 0; j < cols; ++j) {
            localResult[i] += matrix[i * cols + j] * vector[j];
        }
    }
    MPI_Allgather(localResult, rows, MPI_INT, result, rows, MPI_INT, MPI_COMM_WORLD);
    free(localResult);
}
