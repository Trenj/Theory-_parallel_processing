#include <iostream>
#include <omp.h>

int main() {
    int k;

    std::cout << "Введите количество потоков: ";
    std::cin >> k;

    // Проверка корректности ввода
    if (k <= 0) {
        std::cerr << "Ошибка: количество потоков должно быть положительным!" << std::endl;
        return 1;
    }

    omp_set_num_threads(k);

#pragma omp parallel
{
    int thread_id = omp_get_thread_num();
    int total_threads = omp_get_num_threads();

    if (thread_id % 2 == 0) {
        #pragma omp critical
        {
            std::cout << "I am " << thread_id
                      << " thread from " << total_threads
                      << " threads!" << std::endl;
        }
    }
}

    return 0;
}