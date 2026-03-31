#include <cstdio>
#include <omp.h>

int main() {
    int k;

    printf("Введите количество потоков: ");
    scanf("%d", &k);

    // Проверка корректности ввода
    if (k <= 0) {
        printf("Ошибка: количество потоков должно быть положительным!\n");
        return 1;
    }

    omp_set_num_threads(k);

    #pragma omp parallel
    {
        int thread_id = omp_get_thread_num();
        int total_threads = omp_get_num_threads();

        printf("I am %d thread from %d threads!\n", thread_id, total_threads);

        }

    return 0;
}