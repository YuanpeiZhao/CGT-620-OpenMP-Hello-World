#include <omp.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

using namespace std;

#define MatrixOrder 1024

int M1[MatrixOrder][MatrixOrder];
int M2[MatrixOrder][MatrixOrder];
int M3[MatrixOrder][MatrixOrder];

int MatrixMulti(int row, int col)
{
    int ans = 0;
    for (int i = 0; i < MatrixOrder; i++)
        ans += M1[row][i] * M2[i][col];

    return ans;
}

int main()
{
    int nthreads, tid;

    #pragma omp parallel private(nthreads, tid)
    {
        tid = omp_get_thread_num();

        if (tid == 0)
        {
            nthreads = omp_get_num_threads();
            printf("Number of threads = %d\n", nthreads);
        }
    }
    

    
    for (int i = 0; i < MatrixOrder; i++) {
        for (int j = 0; j < MatrixOrder; j++) {
            srand(i + j);
            M1[i][j] = (rand() % 10);
            M2[i][j] = (rand() % 10);
        }
    }

    clock_t t1;
    clock_t t2;
    double t = 0.0f;

    for (int time = 0; time < 10; time++) {

        t1 = clock();

        for (int i = 0; i < MatrixOrder; i++) {
            for (int j = 0; j < MatrixOrder; j++) {
                M3[i][j] = MatrixMulti(i, j);
            }
        }

        clock_t t2 = clock();
        t += t2 - t1;
        cout << "serial time: " << t2 - t1 << endl;
    }

    cout << "overall serial time: " << t / 10 << endl;
    t = 0.0f;

    for (int time = 0; time < 10; time++) {

        t1 = clock();

        #pragma omp parallel for
        for (int i = 0; i < MatrixOrder; i++) {
            for (int j = 0; j < MatrixOrder; j++) {
                M3[i][j] = MatrixMulti(i, j);
            }
        }

        t2 = clock();
        t += t2 - t1;
        cout << "parallel time: " << t2 - t1 << endl;
    }

    cout << "overall serial time: " << t / 10 << endl;

    system("pause");

    return 0;
}