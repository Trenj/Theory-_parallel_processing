#include <stdio.h>
#include <omp.h>

int main() {
    int k, N;
    printf("Введите количество потоков: ");
    scanf("%d", &k);
    printf("Введите N: ");
    scanf("%d", &N);

    if (k <= 0 || N <= 0) {
        printf("Ошибка: k и N должны быть положительными!\n");
        return 1;
    }

    int sum = 0;
    int partial_sum = 0;

    omp_set_num_threads(k);

    #pragma omp parallel private(partial_sum) reduction(+:sum)
    {
        int thread_id = omp_get_thread_num();
        int total = omp_get_num_threads();
        partial_sum = 0;

        // каждый поток получает свой диапазон [start, end]
        int chunk = N / total;         // размер куска на каждый поток
        int start = thread_id * chunk + 1;
        int end = (thread_id == total - 1) ? N : start + chunk - 1;
        // последний поток забирает остаток если N не делится на k

        for (int i = start; i <= end; i++)
            partial_sum += i;

        printf("[%d]: Sum = %d\n", thread_id, partial_sum);
        sum += partial_sum;
    }

    printf("Sum = %d\n", sum);

    return 0;
}
```

Проверь на примерах из задания:
```
k=3, N=2:
  chunk = 2/3 = 0  ← поток 0: [1,0] = пустой диапазон, sum=0...