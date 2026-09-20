// Етап 2: цикл читання + виконання команди через клас Shell.

#include <cstdio>
#include <cstdlib>
#include <string>

#include <unistd.h>

#include "shell.h"

int main() {
    Shell shell;

    char* line = nullptr;
    size_t cap = 0;

    while (true) {
        printf("wish> ");
        fflush(stdout);

        const ssize_t len = getline(&line, &cap, stdin);
        if (len == -1) {
            break;
        }

        shell.run_line(std::string(line, static_cast<size_t>(len)));
    }

    free(line);
    exit(0);
}
