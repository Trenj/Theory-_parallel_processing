#include <windows.h>
#include <iostream>
#include <cstring>

int main() {

    // Устанавливаем UTF-8 кодировку консоли
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    const char* pipeName = "\\\\.\\pipe\\MyPipe";

    HANDLE hPipe = CreateNamedPipe(
        pipeName,
        PIPE_ACCESS_OUTBOUND,
        PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,
        1,
        512,
        512,
        0,
        NULL);

    if (hPipe == INVALID_HANDLE_VALUE) {
        std::cout << "Ошибка CreateNamedPipe\n";
        system("pause");
        return 1;
    }

    std::cout << "Ожидание подключения клиента...\n";

    ConnectNamedPipe(hPipe, NULL);

    for (int i = 0; i < 10; ++i) {

        const char* message = "Hello!";
        DWORD bytesWritten;

        WriteFile(
            hPipe,
            message,
            strlen(message) + 1,
            &bytesWritten,
            NULL);

        std::cout << "Отправлено сообщение: " << message << std::endl;

        Sleep(2000); // 2 секунды
    }

    CloseHandle(hPipe);

    std::cout << "Передача завершена. Нажмите любую клавишу...\n";
    system("pause");

    return 0;
}