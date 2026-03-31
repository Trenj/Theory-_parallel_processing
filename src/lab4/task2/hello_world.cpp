#include <stdio.h>
#include <omp.h>

int main() {
    omp_set_num_threads(4);    // Создаём 4 потока
    
    #pragma omp parallel    // Делаем область кода запускаемую в каждом потоке (директива)
    {
        printf("Hello World!\n");
    }
    
    return 0;
}

// в чём разница #pragma omp parallel omp_set_num_threads(4)?

//  omp_set_num_threads(4);  
//  ^--- Динамическая установка через через библиотечную функцию
//       Работает на всю область кода (глобально)
//  #pragma omp parallel omp_set_num_threads(4)  
//  ^--- Локальная настройка
//       Работает только в этой параллельной области