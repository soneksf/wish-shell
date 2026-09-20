
#include <cstdio>
#include <cstdlib>
#include <string>

#include <unistd.h>

#include "error.h"

int main() {
    char* line = nullptr;
    size_t cap = 0;

    while (true) {
        printf("wish> ");
        fflush(stdout);

        const ssize_t len = getline(&line, &cap, stdin);
        if (len == -1) {  // EOF (Ctrl+D)
            break;
        }

        std::string input(line, static_cast<size_t>(len));

        if (input == "exit\n" || input == "exit") {
            free(line);
            exit(0);
        }

        printf("ви ввели: %s", input.c_str());
    }

    free(line);
    exit(0);
}
