# Настройка VSCode для работы с MPI на Ubuntu Linux

## Содержание

- [Настройка VSCode для работы с MPI на Ubuntu Linux](#настройка-vscode-для-работы-с-mpi-на-ubuntu-linux)
  - [Содержание](#содержание)
  - [Требования](#требования)
  - [Установка OpenMPI](#установка-openmpi)
  - [Настройка VSCode](#настройка-vscode)
    - [Настройка Code Runner](#настройка-code-runner)
    - [Настройка IntelliSense](#настройка-intellisense)
  - [Проверка установки](#проверка-установки)
    - [Как это работает](#как-это-работает)
    - [Компиляция вручную (без Code Runner)](#компиляция-вручную-без-code-runner)
  - [Краткий справочник по MPI](#краткий-справочник-по-mpi)
    - [Обязательная структура программы](#обязательная-структура-программы)
    - [Основные функции](#основные-функции)
    - [Функции передачи сообщений точка-точка](#функции-передачи-сообщений-точка-точка)
    - [Типы данных MPI](#типы-данных-mpi)
    - [Флаги компилятора](#флаги-компилятора)

---

## Требования

- Ubuntu Linux (любая современная версия)
- VSCode
- GCC (входит в `build-essential`)
- OpenMPI

---

## Установка OpenMPI

Установи пакеты OpenMPI:

```bash
sudo apt update
sudo apt install libopenmpi-dev openmpi-bin
```

Проверь установку:

```bash
mpicc --version
mpirun --version
```

Должно вывести что-то вроде:
```
mpicc (Open MPI) 4.1.2
mpirun (Open MPI) 4.1.2
```

Проверь что MPI доступен:

```bash
echo "#include <mpi.h>" | mpicc -x c - -c -o /dev/null && echo "MPI работает!"
```

Если видишь `MPI работает!` — всё готово.

---

## Настройка VSCode

### Настройка Code Runner

В отличие от OpenMP, MPI использует собственные обёртки над компилятором (`mpicc` для C, `mpic++` для C++) и отдельную команду запуска `mpirun`. Code Runner нужно настроить для работы с ними.

> **Важно:** `mpirun` нельзя запустить напрямую через Code Runner удобным образом, потому что нужно указывать количество процессов (`-np`). Поэтому рекомендуется использовать **встроенный терминал VSCode** для запуска MPI-программ.

Открой настройки Code Runner:
- `Ctrl+Shift+P` → введи `Open User Settings (JSON)`

Найди секцию `code-runner.executorMap` и добавь строки для `c` и `cpp`:

```json
"code-runner.executorMap": {
    "c":   "cd $dir && mpicc $fileName -o $fileNameWithoutExt && mpirun -np 4 ./$fileNameWithoutExt",
    "cpp": "cd $dir && mpic++ $fileName -o $fileNameWithoutExt && mpirun -np 4 ./$fileNameWithoutExt"
}
```

> Замени `4` на нужное количество процессов. Для гибкости лучше компилировать через Code Runner, а запускать вручную в терминале — см. раздел «Компиляция вручную».

### Настройка IntelliSense

Без этого шага VSCode будет подчёркивать `#include <mpi.h>` и все функции MPI красным, хотя код будет компилироваться нормально.

Найди путь к заголовочным файлам MPI:

```bash
find /usr -name "mpi.h" 2>/dev/null
```

Обычно путь выглядит как `/usr/lib/x86_64-linux-gnu/openmpi/include`.

Создай или обнови файл `.vscode/c_cpp_properties.json` в папке своего проекта:

```json
{
    "configurations": [
        {
            "name": "Linux",
            "includePath": [
                "${workspaceFolder}/**",
                "/usr/lib/x86_64-linux-gnu/openmpi/include"
            ],
            "defines": [],
            "compilerPath": "/usr/bin/gcc",
            "cStandard": "c17",
            "cppStandard": "c++17",
            "intelliSenseMode": "linux-gcc-x64"
        }
    ],
    "version": 4
}
```

> Если путь к `mpi.h` у тебя другой — подставь свой из результата команды `find` выше.

---

## Проверка установки

Создай файл `test_mpi.c` и скомпилируй его в терминале:

```c
#include <stdio.h>
#include <mpi.h>

int main(int argc, char* argv[]) {
    int rank, size;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    printf("Hello from process %d of %d!\n", rank, size);

    MPI_Finalize();
    return 0;
}
```

Компилируй и запускай:

```bash
mpicc test_mpi.c -o test_mpi
mpirun -np 4 ./test_mpi
```

Ожидаемый вывод (порядок строк может отличаться — это нормально):

```
Hello from process 0 of 4!
Hello from process 2 of 4!
Hello from process 1 of 4!
Hello from process 3 of 4!
```

Если видишь 4 строки с разными номерами процессов — настройка завершена успешно.

---

### Как это работает

```
mpirun -np 4 ./программа
      │
      │  запускает 4 независимых процесса
      │
      ├── процесс 0 (rank=0)   ← обычно выполняет роль «главного»
      ├── процесс 1 (rank=1)
      ├── процесс 2 (rank=2)
      └── процесс 3 (rank=3)

Каждый процесс выполняет ОДИН И ТОТ ЖЕ код,
но знает свой rank и ведёт себя по-разному.

Процессы общаются через передачу сообщений:
   процесс 0 ──MPI_Send──► процесс 1
   процесс 1 ──MPI_Recv──► получает сообщение
```

**`MPI_Init()`** — инициализирует MPI-среду. Всегда первым вызовом.

**`MPI_Comm_rank()`** — возвращает номер текущего процесса (от 0 до N-1).

**`MPI_Comm_size()`** — возвращает общее количество процессов.

**`MPI_Finalize()`** — завершает MPI-среду. Всегда последним вызовом.

**`MPI_COMM_WORLD`** — коммуникатор по умолчанию, объединяющий все процессы приложения.

### Компиляция вручную (без Code Runner)

```bash
# Компиляция C
mpicc program.c -o program

# Компиляция C++
mpic++ program.cpp -o program

# Запуск с указанием числа процессов
mpirun -np 4 ./program
```

---

## Краткий справочник по MPI

### Обязательная структура программы

```c
#include <mpi.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
    int rank, size;

    MPI_Init(&argc, &argv);                    // ← всегда первым
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);      // узнаём свой номер
    MPI_Comm_size(MPI_COMM_WORLD, &size);      // узнаём общее кол-во

    // ... код программы ...

    MPI_Finalize();                            // ← всегда последним
    return 0;
}
```

### Основные функции

```c
// Инициализация и завершение
MPI_Init(&argc, &argv);          // инициализировать MPI
MPI_Finalize();                  // завершить MPI

// Информация о процессах
MPI_Comm_rank(MPI_COMM_WORLD, &rank);   // номер текущего процесса (0..N-1)
MPI_Comm_size(MPI_COMM_WORLD, &size);   // общее количество процессов
```

### Функции передачи сообщений точка-точка

```c
// Отправка сообщения (блокирующая)
MPI_Send(
    &data,            // указатель на данные
    count,            // количество элементов
    MPI_INT,          // тип данных
    dest,             // номер процесса-получателя
    tag,              // метка сообщения (любое число, например 0)
    MPI_COMM_WORLD    // коммуникатор
);

// Приём сообщения (блокирующая)
MPI_Status status;
MPI_Recv(
    &data,            // буфер для получения
    count,            // максимальное количество элементов
    MPI_INT,          // тип данных
    source,           // номер процесса-отправителя (или MPI_ANY_SOURCE)
    tag,              // метка сообщения (или MPI_ANY_TAG)
    MPI_COMM_WORLD,   // коммуникатор
    &status           // информация о полученном сообщении
);
```

> **Блокирующие функции:** `MPI_Send` блокируется до тех пор, пока данные не переданы; `MPI_Recv` блокируется до получения сообщения. Оба процесса должны вызвать свою функцию, иначе возникнет **дедлок** (взаимная блокировка).

Пример — процесс 0 отправляет число процессу 1:

```c
if (rank == 0) {
    int num = 42;
    MPI_Send(&num, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
} else if (rank == 1) {
    int num;
    MPI_Status status;
    MPI_Recv(&num, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
    printf("Получено: %d\n", num);
}
```

### Типы данных MPI

| Тип MPI | Тип C |
|---|---|
| `MPI_INT` | `int` |
| `MPI_LONG` | `long` |
| `MPI_FLOAT` | `float` |
| `MPI_DOUBLE` | `double` |
| `MPI_CHAR` | `char` |
| `MPI_BYTE` | `unsigned char` |

### Флаги компилятора

| Команда | Назначение |
|---|---|
| `mpicc` | компилятор C с поддержкой MPI |
| `mpic++` / `mpicxx` | компилятор C++ с поддержкой MPI |
| `mpirun -np N` | запустить N процессов |
| `-O2` | оптимизация (рекомендуется для замеров производительности) |

---

> **Примечание:** порядок вывода строк из разных процессов не детерминирован — каждый запуск может давать разный порядок. Это нормальное поведение параллельных программ.

> **Отличие от OpenMP:** OpenMP создаёт потоки внутри одного процесса и они делят общую память. MPI запускает отдельные процессы, у каждого своя память — обмен данными возможен только через явную передачу сообщений.