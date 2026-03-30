#include <iostream>
#include <omp.h>
#include <algorithm> // для min

using namespace std;

int main() {
    int k, N;
    cout << "Введите k (кол-во потоков): ";
    cin >> k;
    cout << "Введите N (кол-во чисел): ";
    cin >> N;

    long long total_sum = 0;

    // Устанавливаем количество потоков
    omp_set_num_threads(k);

    #pragma omp parallel reduction(+:total_sum)
    {
        int tid = omp_get_thread_num();
        int num_threads = omp_get_num_threads();
        long long partial_sum = 0;

        // Математическое распределение диапазонов для k потоков
        // Базовое количество чисел на поток
        int base_count = N / num_threads;
        // Остаток, который распределяется по первым потокам
        int remainder = N % num_threads;

        // Сколько чисел достанется этому потоку
        int my_count = base_count + (tid < remainder ? 1 : 0);

        // Вычисляем начальный индекс для этого потока
        // Потоки с id < remainder имеют размер base_count + 1
        // Потоки с id >= remainder имеют размер base_count
        int start_index;
        if (tid < remainder) {
            start_index = tid * (base_count + 1) + 1;
        } else {
            start_index = remainder * (base_count + 1) + (tid - remainder) * base_count + 1;
        }

        int end_index = start_index + my_count - 1;

        // Вычисление частичной суммы
        for (int i = start_index; i <= end_index; ++i) {
            partial_sum += i;
        }

        // Обновление общей суммы (механизм reduction)
        total_sum += partial_sum;

        // Вывод частичной суммы
        #pragma omp critical
        {
            cout << "[" << tid << "]: Sum = " << partial_sum << endl;
        }
    }

    // Вывод общей суммы
    cout << "Sum = " << total_sum << endl;

    return 0;
}