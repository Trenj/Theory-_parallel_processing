#include <stdio.h>
#include <omp.h>
#include <unistd.h>

int main() {
    int k;
    printf("Введите количество потоков: ");
    scanf("%d", &k);

    if (k <= 0) {
        printf("Ошибка: количество потоков должно быть положительным!\n");
        return 1;
    }

    int rank;
    omp_set_num_threads(k);

    #pragma omp parallel private(rank)
    {
        rank = omp_get_thread_num(); // каждый поток пишет в свою копию
        usleep(100);
        printf("I am %d thread.\n", rank);
    }

    return 0;
}