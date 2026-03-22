#include <windows.h>
#include <wchar.h>

int main() {
    // Получаем дескриптор консоли для вывода
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    // Создаём события о готовности данных и прочтении
    HANDLE hEventReady = CreateEventW(NULL, FALSE, FALSE, L"DataReadyEvent");
    HANDLE hEventRead  = CreateEventW(NULL, FALSE, FALSE, L"DataReadEvent");
    if (!hEventReady || !hEventRead) {
        WriteConsoleW(hConsole, L"Ошибка создания/открытия событий\n", 35, NULL, NULL);
        _getwch();
        return 1;
    }

    HANDLE hMapFile = NULL;
    // Ждём появления объекта отображения
    while (1) {
        hMapFile = OpenFileMappingW(FILE_MAP_ALL_ACCESS, FALSE, L"MySharedMemory");
        if (hMapFile) break;
        Sleep(500);
    }

    const int size = 256 * sizeof(wchar_t);
    wchar_t* data = (wchar_t*)MapViewOfFile(
        hMapFile,
        FILE_MAP_ALL_ACCESS,
        0, 0, size);

    if (!data) {
        wchar_t buf[100];
        wsprintfW(buf, L"Ошибка MapViewOfFile: %lu\n", GetLastError());
        WriteConsoleW(hConsole, buf, wcslen(buf), NULL, NULL);
        CloseHandle(hMapFile);
        CloseHandle(hEventReady);
        CloseHandle(hEventRead);
        _getwch();
        return 1;
    }

    WaitForSingleObject(hEventReady, INFINITE);     // Ждём сигнала от sender'а

    // Выводим сообщение
    wchar_t output[300];
    wsprintfW(output, L"Получено сообщение: %s\n", data);
    WriteConsoleW(hConsole, output, wcslen(output), NULL, NULL);

    SetEvent(hEventRead);   // Подтверждаем прочтение

    // Очистка
    UnmapViewOfFile(data);
    CloseHandle(hMapFile);
    CloseHandle(hEventReady);
    CloseHandle(hEventRead);

    WriteConsoleW(hConsole, L"Нажмите любую клавишу для выхода...\n", 40, NULL, NULL);
    _getwch();
    return 0;
}