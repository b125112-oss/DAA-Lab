#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define PI 3.14159265358979323846

typedef struct {
    double real;
    double imag;
} Complex;

void fft(Complex *a, int n, int invert) {
    for (int i = 1, j = 0; i < n; i++) {
        int bit = n >> 1;
        while (j & bit) {
            j ^= bit;
            bit >>= 1;
        }
        j ^= bit;
        if (i < j) {
            Complex tmp = a[i];
            a[i] = a[j];
            a[j] = tmp;
        }
    }

    for (int len = 2; len <= n; len <<= 1) {
        double angle = 2.0 * PI / len * (invert ? -1 : 1);
        Complex wlen = {cos(angle), sin(angle)};

        for (int i = 0; i < n; i += len) {
            Complex w = {1.0, 0.0};
            for (int j = 0; j < len / 2; j++) {
                Complex u = a[i + j];
                Complex v = {
                    a[i + j + len / 2].real * w.real - a[i + j + len / 2].imag * w.imag,
                    a[i + j + len / 2].real * w.imag + a[i + j + len / 2].imag * w.real
                };

                a[i + j].real = u.real + v.real;
                a[i + j].imag = u.imag + v.imag;
                a[i + j + len / 2].real = u.real - v.real;
                a[i + j + len / 2].imag = u.imag - v.imag;

                Complex wNext = {
                    w.real * wlen.real - w.imag * wlen.imag,
                    w.real * wlen.imag + w.imag * wlen.real
                };
                w = wNext;
            }
        }
    }

    if (invert) {
        for (int i = 0; i < n; i++) {
            a[i].real /= n;
            a[i].imag /= n;
        }
    }
}

void convolutionFFT(int A[], int B[], int m, int n, int result[]) {
    int size = 1;
    while (size < m + n - 1) size <<= 1;

    Complex *fa = (Complex *)malloc(size * sizeof(Complex));
    Complex *fb = (Complex *)malloc(size * sizeof(Complex));

    for (int i = 0; i < size; i++) {
        fa[i].real = 0; fa[i].imag = 0;
        fb[i].real = 0; fb[i].imag = 0;
    }

    for (int i = 0; i < m; i++) fa[i].real = A[i];
    for (int i = 0; i < n; i++) fb[i].real = B[i];

    fft(fa, size, 0);
    fft(fb, size, 0);

    for (int i = 0; i < size; i++) {
        Complex product = {
            fa[i].real * fb[i].real - fa[i].imag * fb[i].imag,
            fa[i].real * fb[i].imag + fa[i].imag * fb[i].real
        };
        fa[i] = product;
    }

    fft(fa, size, 1);

    for (int i = 0; i < m + n - 1; i++) {
        result[i] = (int)round(fa[i].real);
    }

    free(fa);
    free(fb);
}

void printArray(int arr[], int n) {
    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

int main() {
    srand((unsigned)time(NULL));
    int mode = 0;
    printf("Choose input mode:\n1) Default arrays\n2) Manual input\n3) Random arrays\nSelect (1-3): ");
    if (scanf("%d", &mode) != 1) mode = 1;

    int m, n;
    int *A = NULL, *B = NULL, *result = NULL;

    if (mode == 1) {
        int tmpA[] = {1,2,3};
        int tmpB[] = {4,5,6};
        m = sizeof(tmpA)/sizeof(tmpA[0]);
        n = sizeof(tmpB)/sizeof(tmpB[0]);
        A = malloc(m * sizeof(int)); B = malloc(n * sizeof(int));
        for (int i = 0; i < m; i++) A[i] = tmpA[i];
        for (int i = 0; i < n; i++) B[i] = tmpB[i];
    } else if (mode == 2) {
        printf("Enter length of A: "); if (scanf("%d", &m) != 1) return 0;
        printf("Enter length of B: "); if (scanf("%d", &n) != 1) return 0;
        A = malloc(m * sizeof(int)); B = malloc(n * sizeof(int));
        printf("Enter %d values for A:\n", m);
        for (int i = 0; i < m; i++) scanf("%d", &A[i]);
        printf("Enter %d values for B:\n", n);
        for (int i = 0; i < n; i++) scanf("%d", &B[i]);
    } else {
        printf("Enter length of A to generate: "); if (scanf("%d", &m) != 1) return 0;
        printf("Enter length of B to generate: "); if (scanf("%d", &n) != 1) return 0;
        int range = 10; printf("Enter max random value (default 10): "); if (scanf("%d", &range) != 1) range = 10;
        A = malloc(m * sizeof(int)); B = malloc(n * sizeof(int));
        for (int i = 0; i < m; i++) A[i] = rand() % range;
        for (int i = 0; i < n; i++) B[i] = rand() % range;
    }

    result = malloc((m + n - 1) * sizeof(int));
    convolutionFFT(A, B, m, n, result);

    printf("A: "); printArray(A, m);
    printf("B: "); printArray(B, n);
    printf("Convolution result: "); printArray(result, m + n - 1);

    free(A); free(B); free(result);
    return 0;
}