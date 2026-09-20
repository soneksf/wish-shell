// Етап 1: цикл читання рядків і запрошення "wish> ".
// Поки що вміємо лише вийти по "exit" або по EOF (Ctrl+D).

#include <cstdio>
#include <cstdlib>
#include <string>

#include <unistd.h>

#include "error.h"

int main() {
    char* line = nullptr;  // getline сам виділить пам'ять
    size_t cap = 0;

    while (true) {
        printf("wish> ");
        fflush(stdout);  // без цього запрошення зависне в буфері

        const ssize_t len = getline(&line, &cap, stdin);
        if (len == -1) {  // EOF (Ctrl+D)
            break;
        }

        std::string input(line, static_cast<size_t>(len));

        if (input == "exit\n" || input == "exit") {
            free(line);
            exit(0);
        }

        // тимчасова заглушка, поки немає запуску програм
        printf("ви ввели: %s", input.c_str());
    }

    free(line);
    exit(0);
}
