#include <stdio.h>
#include <omp.h>

int main() {
    int N;
    printf("Введите N: ");
    scanf("%d", &N);

    if (N <= 0) {
        printf("Ошибка: N должно быть положительным!\n");
        return 1;
    }

    int sum = 0;
    int partial_sum = 0;

    omp_set_num_threads(2);

    #pragma omp parallel private(partial_sum) reduction(+:sum)
    {
        int thread_id = omp_get_thread_num();
        partial_sum = 0;

        if (thread_id == 0) {
            // поток 0 считает от 1 до N/2
            for (int i = 1; i <= N / 2; i++)
                partial_sum += i;
        } else {
            // поток 1 считает от N/2+1 до N
            for (int i = N / 2 + 1; i <= N; i++)
                partial_sum += i;
        }

        printf("[%d]: Sum = %d\n", thread_id, partial_sum);
        sum += partial_sum; // reduction суммирует копии sum со всех потоков
    }

    printf("Sum = %d\n", sum);

    return 0;
}