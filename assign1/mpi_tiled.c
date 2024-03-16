#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

void fillMatrixVector(int *matrix, int *vector, int rows, int cols, int size);
void printMatrixVector(int *matrix, int *vector, int result[], int rows, int cols);
void matrixVectorMultiplication(int *matrix, int *vector, int *result, int rows, int cols, int tileSize);

int main(int argc, char *argv[]) {
    if (argc != 4) {
        printf("Usage: %s <matrix size> <vector size> <tile size>\n", argv[0]);
        return 1;
    }

    int matrixSize = atoi(argv[1]);
    int vectorSize = atoi(argv[2]);
    int tileSize = atoi(argv[3]);
    MPI_Init(&argc, &argv);
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    if (matrixSize % world_size != 0) {
        if (world_rank == 0) {
            printf("Matrix size must be divisible by the number of processes.\n");
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
    srand(time(NULL));
    fillMatrixVector(matrix, vector, localRows, vectorSize, 10);
    matrixVectorMultiplication(matrix, vector, result, localRows, vectorSize, tileSize);
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
void matrixVectorMultiplication(int *matrix, int *vector, int *result, int rows, int cols, int tileSize) {
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    for (int i = 0; i < rows; i += tileSize) {
        for (int j = 0; j < cols; j += tileSize) {
            for (int k = 0; k < cols; ++k) {
                for (int ii = i; ii < i + tileSize && ii < rows; ++ii) {
                    result[ii] += matrix[(ii - i) * cols + k] * vector[k];
                }
            }
        }
    }
}
