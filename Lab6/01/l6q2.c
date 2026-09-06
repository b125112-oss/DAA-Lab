#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define N 3

void printMatrix(double A[N][N]) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            printf("%.2f ", A[i][j]);
        }
        printf("\n");
    }
}

void matrixAdd(double A[N][N], double B[N][N], double C[N][N]) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            C[i][j] = A[i][j] + B[i][j];
        }
    }
}

void matrixMultiply(double A[N][N], double B[N][N], double C[N][N]) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            C[i][j] = 0;
            for (int k = 0; k < N; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

int isZeroMatrix(double A[N][N]) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (fabs(A[i][j]) > 1e-9) return 0;
        }
    }
    return 1;
}

int isSymmetric(double A[N][N]) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (fabs(A[i][j] - A[j][i]) > 1e-9) return 0;
        }
    }
    return 1;
}

double determinant(double A[N][N], int n) {
    if (n == 1) return A[0][0];
    if (n == 2) {
        return A[0][0] * A[1][1] - A[0][1] * A[1][0];
    }

    double det = 0;
    int sign = 1;
    for (int col = 0; col < n; col++) {
        double minor[N][N];
        int r = 0;
        for (int i = 1; i < n; i++) {
            int c = 0;
            for (int j = 0; j < n; j++) {
                if (j == col) continue;
                minor[r][c++] = A[i][j];
            }
            r++;
        }
        det += sign * A[0][col] * determinant(minor, n - 1);
        sign *= -1;
    }
    return det;
}

void transposeInPlace(double A[N][N]) {
    for (int i = 0; i < N; i++) {
        for (int j = i + 1; j < N; j++) {
            double temp = A[i][j];
            A[i][j] = A[j][i];
            A[j][i] = temp;
        }
    }
}

void multiplyVectorByMatrix(double A[N][N], double x[N], double out[N]) {
    for (int i = 0; i < N; i++) {
        out[i] = 0;
        for (int j = 0; j < N; j++) {
            out[i] += A[i][j] * x[j];
        }
    }
}

void powerIteration(double A[N][N], double *eigenValue, double eigenVector[N]) {
    double x[N] = {1.0, 1.0, 1.0};
    const int iterations = 100;

    for (int iter = 0; iter < iterations; iter++) {
        double y[N];
        multiplyVectorByMatrix(A, x, y);

        double norm = 0.0;
        for (int i = 0; i < N; i++) norm += y[i] * y[i];
        norm = sqrt(norm);

        for (int i = 0; i < N; i++) x[i] = y[i] / norm;
    }

    double y[N];
    multiplyVectorByMatrix(A, x, y);
    double numerator = 0.0;
    double denominator = 0.0;
    for (int i = 0; i < N; i++) {
        numerator += x[i] * y[i];
        denominator += x[i] * x[i];
    }

    *eigenValue = numerator / denominator;
    for (int i = 0; i < N; i++) eigenVector[i] = x[i];
}

int main() {
    srand((unsigned)time(NULL));
    int mode = 0;
    printf("Choose input mode:\n1) Use default matrices\n2) Random matrices\nSelect (1 or 2): ");
    if (scanf("%d", &mode) != 1) mode = 1;

    double A[N][N];
    double B[N][N];

    if (mode == 1) {
        double tmpA[N][N] = {
            {1, 2, 3},
            {4, 5, 6},
            {7, 8, 9}
        };
        double tmpB[N][N] = {
            {9, 8, 7},
            {6, 5, 4},
            {3, 2, 1}
        };
        for (int i = 0; i < N; i++) for (int j = 0; j < N; j++) { A[i][j] = tmpA[i][j]; B[i][j] = tmpB[i][j]; }
    } else {
        int range = 10;
        printf("Enter max random value for matrix entries (default 10): ");
        if (scanf("%d", &range) != 1) range = 10;
        for (int i = 0; i < N; i++) for (int j = 0; j < N; j++) {
            A[i][j] = (double)(rand() % range);
            B[i][j] = (double)(rand() % range);
        }
    }

    double C[N][N];
    double D[N][N];
    double eigVec[N];
    double eigVal;

    printf("Matrix A:\n");
    printMatrix(A);
    printf("\nMatrix B:\n");
    printMatrix(B);

    matrixAdd(A, B, C);
    printf("\nA + B:\n");
    printMatrix(C);

    matrixMultiply(A, B, D);
    printf("\nA * B:\n");
    printMatrix(D);

    printf("\nIs zero matrix? %s\n", isZeroMatrix(A) ? "Yes" : "No");
    printf("Is symmetric? %s\n", isSymmetric(A) ? "Yes" : "No");
    printf("Determinant: %.2f\n", determinant(A, N));

    transposeInPlace(A);
    printf("\nTranspose in place:\n");
    printMatrix(A);

    double S[N][N] = {
        {2, 1, 0},
        {1, 3, 1},
        {0, 1, 4}
    };
    powerIteration(S, &eigVal, eigVec);
    printf("\nLargest eigenvalue (approx): %.4f\n", eigVal);
    printf("Eigenvector: ");
    for (int i = 0; i < N; i++) printf("%.4f ", eigVec[i]);
    printf("\n");

    return 0;
}