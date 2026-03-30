#include <iostream>
#include <omp.h>

int main() {
    int k;
    std::cout << "Введите количество потоков: ";
    std::cin >> k;
    
    omp_set_num_threads(k);
    
    int rank;  // Общая переменная по умолчанию!
    
    #pragma omp parallel
    {
        // ОШИБКА: все нити пишут в одну общую переменную
        rank = omp_get_thread_num();
        
        #pragma omp critical
        {
            printf("I am %d thread.\n", rank);
        }
    }
    
    return 0;
}