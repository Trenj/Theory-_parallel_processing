#include <stdio.h>
#include <omp.h>

int main() {
    omp_set_num_threads(4);    // Создаём 4 потока
    
    #pragma omp parallel    // Делаем область кода запускаемую в каждом потоке
    {
        printf("Hello World!\n");
    }
    
    return 0;
}