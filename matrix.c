#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define MAX 100

int A[MAX][MAX], B[MAX][MAX], C[MAX][MAX];
int rowsA, colsA, rowsB, colsB;

typedef struct {
    int row;
} ThreadData;

void* multiplyRow(void* arg) {
    ThreadData* data = (ThreadData*)arg;
    int r = data->row;

    for (int j = 0; j < colsB; j++) {
        C[r][j] = 0;
        for (int k = 0; k < colsA; k++) {
            C[r][j] += A[r][k] * B[k][j];
        }
    }

    pthread_exit(NULL);
}

int main() {
    printf("Enter rows and columns for Matrix A: ");
    scanf("%d %d", &rowsA, &colsA);

    printf("Enter rows and columns for Matrix B: ");
    scanf("%d %d", &rowsB, &colsB);

    if (colsA != rowsB) {
        printf("Matrix multiplication not possible!\n");
        return 1;
    }

    printf("Enter elements of Matrix A:\n");
    for (int i = 0; i < rowsA; i++)
        for (int j = 0; j < colsA; j++)
            scanf("%d", &A[i][j]);

    printf("Enter elements of Matrix B:\n");
    for (int i = 0; i < rowsB; i++)
        for (int j = 0; j < colsB; j++)
            scanf("%d", &B[i][j]);

    pthread_t threads[rowsA];
    ThreadData threadData[rowsA];

    for (int i = 0; i < rowsA; i++) {
        threadData[i].row = i;
        pthread_create(&threads[i], NULL, multiplyRow, &threadData[i]);
    }

    for (int i = 0; i < rowsA; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("Resulting Matrix C:\n");
    for (int i = 0; i < rowsA; i++) {
        for (int j = 0; j < colsB; j++) {
            printf("%d ", C[i][j]);
        }
        printf("\n");
    }

    return 0;
}
