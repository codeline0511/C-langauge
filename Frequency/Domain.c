#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define WINDOW_SIZE 500   // 샘플링한 데이터를 추출할 값
#define PI 3.1415926535   // PI의 값이 정교해질 수록 오차가 줄어듦
#define SAMPLE_RATE 8000  // 받은 데이터를 1초마다 몇개로 나눌 것에 대한 값

typedef struct {
    double real;
    double imag;
} Complex;

int main() {
    FILE* fp = fopen("sample1.snd", "rb");
    if (!fp) {
        perror("파일 열기 실패");
        return 1;
    }

    signed char signal[WINDOW_SIZE];
    size_t sampleRead = fread(signal, sizeof(signed char), WINDOW_SIZE, fp);
    fclose(fp);

    Complex output[WINDOW_SIZE];

    for (int k = 0; k < WINDOW_SIZE; k++) {
        output[k].real = 0;
        output[k].imag = 0;
        for (int n = 0; n < WINDOW_SIZE; n++) {
            double W = 2 * PI * k * n / WINDOW_SIZE;
            output[k].real += signal[n] * cos(W);
            output[k].imag -= signal[n] * sin(W);
        }
    }

    for (int i = 0; i < WINDOW_SIZE/2; i++) {
        int frequency = i * SAMPLE_RATE / WINDOW_SIZE;
        double magnitude = sqrt(output[i].real * output[i].real + output[i].imag * output[i].imag);
        printf("%d Hz: Magnitude = %.0f\n", frequency, magnitude);
    }
    return 0;
}