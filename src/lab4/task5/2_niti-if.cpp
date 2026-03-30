#include <iostream>
#include <omp.h>

using namespace std;

int main() {
    int N;
    cout << "Введите N: ";
    cin >> N;

    long long total_sum = 0;

    // Запускаем параллельную область с 2 потоками
    // reduction(+:total_sum) автоматически суммирует локальные суммы потоков
    #pragma omp parallel num_threads(2) reduction(+:total_sum)
    {
        int tid = omp_get_thread_num();
        long long partial_sum = 0;
        int start, end;

        // Распределение работы с помощью оператора if
        if (tid == 0) {
            // Поток 0 считает от 1 до N/2
            start = 1;
            end = N / 2;
        } else {
            // Поток 1 считает от N/2 + 1 до N
            start = N / 2 + 1;
            end = N;
        }

        // Вычисление частичной суммы
        for (int i = start; i <= end; ++i) {
            partial_sum += i;
        }

        // Добавляем частичную сумму в общую (через механизм reduction)
        // В явном виде внутри reduction это делается автоматически, 
        // но переменная partial_sum локальна для потока.
        // Чтобы reduction сработал, мы должны добавить partial_sum в total_sum
        total_sum += partial_sum; 

        // Вывод частичной суммы (защищаем вывод критической секцией, чтобы строки не перемешались)
        #pragma omp critical
        {
            cout << "[" << tid << "]: Sum = " << partial_sum << endl;
        }
    }

    // Вывод общей суммы (выполняется одним потоком после завершения parallel)
    cout << "Sum = " << total_sum << endl;

    return 0;
}