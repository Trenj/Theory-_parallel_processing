# Theory: Parallel Processing

Учебный репозиторий по параллельному программированию. В проекте собраны лабораторные работы по потокам и синхронизации, IPC, OpenMP, MPI и CUDA.

## Структура проекта

```text
.
├── labs/
│   ├── lab-01-thread-synchronization/
│   │   ├── docs/
│   │   └── src/
│   ├── lab-02-shared-memory-ipc/
│   │   ├── docs/
│   │   └── src/
│   ├── lab-03-named-pipe-ipc/
│   │   ├── docs/
│   │   └── src/
│   ├── lab-04-openmp/
│   │   ├── docs/
│   │   └── tasks/
│   ├── lab-05-mpi/
│   │   ├── docs/
│   │   └── tasks/
│   ├── lab-06-cuda-hello-world/
│   │   ├── docs/
│   │   └── notebooks/
│   ├── lab-07-cuda-vector-addition/
│   │   ├── docs/
│   │   └── notebooks/
│   └── lab-08-cuda-pi/
│       ├── docs/
│       └── notebooks/
└── README.md
```

## Принципы организации

- `labs/` содержит все лабораторные работы и является основной рабочей зоной проекта.
- `docs/` хранит условия заданий, методические указания, PDF и инструкции по настройке.
- `src/` используется для самостоятельных программ или парных приложений внутри конкретной лабораторной.
- `tasks/` используется там, где одна лабораторная включает несколько отдельных заданий.
- `notebooks/` выделен под Jupyter-ноутбуки для CUDA-работ.

Такая структура лучше масштабируется, чем единый `src`, потому что разделяет код, документацию и материалы по смыслу, а не только по номеру лабораторной.

## Содержание лабораторных

- `lab-01-thread-synchronization` — многопоточность на C++: писатели, читатель, общий буфер и синхронизация.
- `lab-02-shared-memory-ipc` — обмен между процессами через отображаемую в память область.
- `lab-03-named-pipe-ipc` — обмен между процессами через именованный канал.
- `lab-04-openmp` — задания по OpenMP: запуск потоков, идентификация, shared/private, reduction.
- `lab-05-mpi` — задания по MPI: базовый запуск, идентификация процессов, обмен сообщениями, кольцевая передача.
- `lab-06-cuda-hello-world` — базовая CUDA-программа в ноутбуке.
- `lab-07-cuda-vector-addition` — сложение векторов в CUDA.
- `lab-08-cuda-pi` — распараллеливание вычисления числа pi в CUDA.

## Быстрый старт

### OpenMP

Пример компиляции:

```bash
g++ labs/lab-04-openmp/tasks/task-02-hello-world/hello_world.cpp -fopenmp -o hello_world
./hello_world
```

Подробная настройка среды:

- `labs/lab-04-openmp/tasks/task-01-setup/quick_setup.md`

### MPI

Пример компиляции и запуска:

```bash
mpic++ labs/lab-05-mpi/tasks/task-17-send-recv/Send_and_Recv.cpp -o send_recv
mpirun -np 2 ./send_recv
```

Подробная настройка среды:

- `labs/lab-05-mpi/tasks/task-14-setup/quick_setup.md`

### CUDA

CUDA-лабораторные оформлены в виде Jupyter notebook и выполнялись в Google Colab, так как локальная CUDA-совместимая видеокарта отсутствует.

Ноутбуки:

- `labs/lab-06-cuda-hello-world/notebooks/Lab6.ipynb`
- `labs/lab-07-cuda-vector-addition/notebooks/Lab7.ipynb`
- `labs/lab-08-cuda-pi/notebooks/Lab8.ipynb`

Рекомендуемый сценарий запуска:

- открыть notebook в Google Colab
- включить GPU через `Runtime` → `Change runtime type` → `T4 GPU` или другой доступный ускоритель
- запускать ячейки последовательно

Локальный запуск возможен только при наличии настроенной CUDA Toolkit и совместимого GPU.

## Замечания по платформе

- `lab-02` и `lab-03` используют WinAPI и ориентированы на Windows.
- `lab-04` и `lab-05` удобно запускать на Linux с GCC/OpenMPI.
- `lab-06` - `lab-08` были выполнены в Google Colab и рассчитаны в первую очередь на запуск в Colab с GPU runtime.
- Локальный запуск `lab-06` - `lab-08` потребует CUDA-среду и совместимый GPU.

## Что изменено

- Убрана перегруженная роль общего каталога `src`.
- Все лабораторные перенесены в единую иерархию `labs/`.
- Код, документация и учебные материалы разделены по отдельным папкам.
- Корневой README приведён к формату, пригодному для сопровождения и навигации.
