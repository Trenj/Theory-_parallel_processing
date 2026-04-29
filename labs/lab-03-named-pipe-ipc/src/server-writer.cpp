#include <windows.h>
#include <iostream>
#include <cstring>

int main() {

    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    const char* pipeName = "\\\\.\\pipe\\MyPipe";

    // событие подтверждения
    HANDLE hEventRead = CreateEvent(NULL, FALSE, FALSE, "MessageReadEvent");

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

        // ждём, пока клиент прочитает
        WaitForSingleObject(hEventRead, INFINITE);
    }

    CloseHandle(hPipe);
    CloseHandle(hEventRead);

    std::cout << "Передача завершена. Нажмите любую клавишу...\n";
    system("pause");

    return 0;
}