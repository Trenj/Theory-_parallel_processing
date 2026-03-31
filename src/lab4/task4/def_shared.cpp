#include <stdio.h>
#include <omp.h>
#include <unistd.h>    // для usleep

int main() {
    int k;
    printf("Введите количество потоков: ");
    scanf("%d", &k);

    if (k <= 0) {
        printf("Ошибка: количество потоков должно быть положительным!\n");
        return 1;
    }

    int rank;   // shared по умолчанию — общая для всех потоков
    omp_set_num_threads(k);

    #pragma omp parallel shared(rank)
    {
        rank = omp_get_thread_num();
        usleep(100);    // имитация долгих вычислений (гонка станет заметнее)
        printf("I am %d thread.\n", rank);
    }

    return 0;
}