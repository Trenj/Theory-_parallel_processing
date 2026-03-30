# Настройка VSCode для работы с OpenMP на Ubuntu Linux

## Содержание

- [Настройка VSCode для работы с OpenMP на Ubuntu Linux](#настройка-vscode-для-работы-с-openmp-на-ubuntu-linux)
  - [Содержание](#содержание)
  - [Требования](#требования)
  - [Установка GCC](#установка-gcc)
  - [Установка VSCode и расширений](#установка-vscode-и-расширений)
    - [Необходимые расширения](#необходимые-расширения)
  - [Настройка Code Runner](#настройка-code-runner)
    - [Что делает эта команда](#что-делает-эта-команда)
  - [Настройка IntelliSense](#настройка-intellisense)
  - [Проверка установки](#проверка-установки)
  - [Проверка установки OpenMP](#проверка-установки)
    - [Как это работает](#как-это-работает)
    - [Компиляция вручную (без Code Runner)](#компиляция-вручную-без-code-runner)
  - [Краткий справочник по OpenMP](#краткий-справочник-по-openmp)
    - [Основные директивы](#основные-директивы)
    - [Полезные функции](#полезные-функции)
    - [Переменные: shared и private](#переменные-shared-и-private)
    - [Флаги компилятора](#флаги-компилятора)

---

## Требования

- Ubuntu Linux (любая современная версия)
- VSCode
- GCC с поддержкой OpenMP (входит в `build-essential`)

---

## Установка GCC

OpenMP на Ubuntu работает из коробки вместе с GCC. Установи стандартные инструменты сборки:

```bash
sudo apt update
sudo apt install build-essential
```

Проверь установку:

```bash
gcc --version
```

Должно вывести что-то вроде:
```
gcc (GCC) 15.2.0
```

Проверь что OpenMP доступен:

```bash
echo "#include <omp.h>" | gcc -x c - -fopenmp -c -o /dev/null && echo "OpenMP работает!"
```

Если видишь `OpenMP работает!` — всё готово.

---

## Установка VSCode и расширений

### Необходимые расширения

Установи следующие расширения через вкладку Extensions (`Ctrl+Shift+X`):

| Расширение | Издатель | Назначение |
|---|---|---|
| **C/C++** | Microsoft | IntelliSense, подсветка синтаксиса, отладка |
| **Code Runner** | Jun Han | Быстрый запуск кода кнопкой ▶️ |

---

## Настройка Code Runner

Code Runner нужно настроить так, чтобы он передавал флаг `-fopenmp` компилятору. Без этого флага OpenMP директивы будут игнорироваться и программа будет работать в один поток.

Открой настройки Code Runner:
- `Ctrl+Shift+P` → введи `Open User Settings (JSON)`

Найди секцию `code-runner.executorMap` и убедись что строки для `c` и `cpp` выглядят так:

```json
"code-runner.executorMap": {
    "c": "cd $dir && gcc $fileName -fopenmp -o $fileNameWithoutExt && ./$fileNameWithoutExt",
    "cpp": "cd $dir && g++ $fileName -fopenmp -o $fileNameWithoutExt && ./$fileNameWithoutExt"
}
```

> **Важно:** на Linux запуск через `./$fileNameWithoutExt`, а не `$dir$fileNameWithoutExt` как на Windows.

### Что делает эта команда

```
cd $dir                        # переходим в папку с файлом
gcc $fileName -fopenmp         # компилируем с поддержкой OpenMP
-o $fileNameWithoutExt         # называем исполняемый файл как исходный (без расширения)
&& ./$fileNameWithoutExt       # запускаем если компиляция прошла успешно
```

---

## Настройка IntelliSense

Без этого шага VSCode будет подчёркивать `#include <omp.h>` и все функции OpenMP красным, хотя код будет компилироваться нормально.

Создай файл `.vscode/c_cpp_properties.json` в папке своего проекта:

```json
{
    "configurations": [
        {
            "name": "Linux",
            "includePath": [
                "${workspaceFolder}/**"
            ],
            "defines": ["_OPENMP"],
            "compilerPath": "/usr/bin/gcc",
            "cStandard": "c17",
            "cppStandard": "c++17",
            "intelliSenseMode": "linux-gcc-x64"
        }
    ],
    "version": 4
}
```

Путь к компилятору можно уточнить командой:

```bash
which gcc
```

---

## Проверка установки

Создай файл `test_omp.c` и запусти через Code Runner (кнопка ▶️) или в терминале:

```c
#include <stdio.h>
#include <omp.h>

int main() {
    omp_set_num_threads(4);

    #pragma omp parallel
    {
        printf("Hello from thread %d!\n", omp_get_thread_num());
    }

    return 0;
}
```

Ожидаемый вывод (порядок строк может отличаться — это нормально):

```
Hello from thread 0!
Hello from thread 2!
Hello from thread 1!
Hello from thread 3!
```

Если видишь 4 строки с разными номерами потоков — настройка завершена успешно.

---


### Как это работает

```
главный поток
      │
      │  omp_set_num_threads(4)  ← настраиваем количество потоков
      │
      │  #pragma omp parallel    ← здесь потоки создаются
      ├──────────┬──────────┬──────────┐
   поток 0   поток 1   поток 2   поток 3
Hello World! Hello World! Hello World! Hello World!
      │          │          │          │
      └──────────┴──────────┴──────────┘
      │
   программа продолжается       ← потоки завершились, управление вернулось
```

**`omp_set_num_threads(4)`** — говорит OpenMP сколько потоков создать. Сами потоки ещё не создаются.

**`#pragma omp parallel`** — директива компилятору. Создаёт потоки и запускает блок кода в каждом из них параллельно. После закрывающей `}` все потоки завершаются.

Эти две строки можно объединить в одну:
```c
#pragma omp parallel num_threads(4)
```

### Компиляция вручную (без Code Runner)

```bash
gcc hello_world.c -fopenmp -o hello_world && ./hello_world
```

---

## Краткий справочник по OpenMP

### Основные директивы

```c
// Параллельный регион — код выполняется всеми потоками
#pragma omp parallel
{
    // выполняется каждым потоком
}

// Параллельный цикл — итерации распределяются между потоками
#pragma omp parallel for
for (int i = 0; i < N; i++) {
    // итерации выполняются параллельно
}

// Критическая секция — только один поток за раз
#pragma omp critical
{
    shared_var += local_val;
}

// Атомарная операция — более лёгкий аналог critical для простых выражений
#pragma omp atomic
sum += local_sum;

// Редукция — автоматическое суммирование результатов потоков
#pragma omp parallel for reduction(+:sum)
for (int i = 0; i < N; i++) {
    sum += arr[i];
}
```

### Полезные функции

```c
omp_set_num_threads(k);      // установить количество потоков
omp_get_thread_num();        // номер текущего потока (0, 1, 2, ...)
omp_get_num_threads();       // количество потоков в текущей параллельной области
omp_get_wtime();             // текущее время в секундах (для замера производительности)
```

### Переменные: shared и private

```c
int x = 0;

// x — общая для всех потоков (shared, по умолчанию)
// i — своя копия для каждого потока (private)
#pragma omp parallel for private(x) shared(arr)
for (int i = 0; i < N; i++) {
    x = arr[i] * 2; // каждый поток работает со своей x
}
```

### Флаги компилятора

| Флаг | Назначение |
|---|---|
| `-fopenmp` | включить поддержку OpenMP (обязателен) |
| `-O2` | оптимизация кода (рекомендуется для замеров производительности) |

---

> **Примечание:** порядок вывода строк из разных потоков не детерминирован — каждый запуск может давать разный порядок. Это нормальное поведение параллельных программ.