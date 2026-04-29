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

        if (N >= total) {
            // блочное распределение: каждый поток получает непрерывный диапазон
            int chunk = N / total;
            int start = thread_id * chunk + 1;
            int end = (thread_id == total - 1) ? N : start + chunk - 1;

            for (int i = start; i <= end; i++)
                partial_sum += i;
        } else {
            // чередование: каждый поток берёт одно число (или ничего если N < thread_id)
            for (int i = thread_id + 1; i <= N; i += total)
                partial_sum += i;
        }

        printf("[%d]: Sum = %d\n", thread_id, partial_sum);
        sum += partial_sum;
    }

    printf("Sum = %d\n", sum);

    return 0;
}