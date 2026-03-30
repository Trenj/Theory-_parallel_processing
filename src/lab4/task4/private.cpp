#include <iostream>
#include <omp.h>

int main() {
    int k;
    std::cout << "Введите количество потоков: ";
    std::cin >> k;
    
    if (k <= 0) {
        std::cerr << "Ошибка: k должно быть положительным!" << std::endl;
        return 1;
    }
    
    omp_set_num_threads(k);
    
    int rank;  // Объявлена до параллельной области
    
    #pragma omp parallel private(rank)  // Каждая нить получает свою копию!
    {
        rank = omp_get_thread_num();  // Теперь безопасно!
        
        #pragma omp critical
        {
            printf("I am %d thread.\n", rank);
        }
    }
    
    return 0;
}