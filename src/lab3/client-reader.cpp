#include <windows.h>
#include <iostream>

int main() {

    // Кодировка сообщений в консоли - UTF-8
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    const char* pipeName = "\\\\.\\pipe\\MyPipe";

    HANDLE hPipe;

    // Ожидание создание канала сервером
    while (true) {

        hPipe = CreateFile(
            pipeName,
            GENERIC_READ,
            0,
            NULL,
            OPEN_EXISTING,
            0,
            NULL);

        if (hPipe != INVALID_HANDLE_VALUE)
            break;

        Sleep(500);
    }

    char buffer[512];
    DWORD bytesRead;

    while (true) {

        BOOL success = ReadFile(
            hPipe,
            buffer,
            sizeof(buffer),
            &bytesRead,
            NULL);

        if (!success || bytesRead == 0)
            break;

        std::cout << "Получено сообщение: " << buffer << std::endl;
    }

    CloseHandle(hPipe);

    std::cout << "Чтение завершено. Нажмите любую клавишу...\n";
    system("pause");

    return 0;
}