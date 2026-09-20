#include <cstdio>
#include <cstdlib>
#include <string>

#include "error.h"
#include "shell.h"

int main(int argc, char* argv[]) {
    FILE* input = stdin;
    bool interactive = true;

    if (argc > 2) {  // більше одного файлу — фатальна помилка
        print_error();
        exit(1);
    }
    if (argc == 2) {
        input = fopen(argv[1], "r");
        if (input == nullptr) {  // поганий batch-файл — фатальна помилка
            print_error();
            exit(1);
        }
        interactive = false;
    }

    Shell shell;

    char* line = nullptr;
    size_t cap = 0;

    while (true) {
        if (interactive) {
            printf("wish> ");
            fflush(stdout);
        }

        const ssize_t len = getline(&line, &cap, input);
        if (len == -1) {  // EOF
            break;
        }

        shell.run_line(std::string(line, static_cast<size_t>(len)));
    }

    free(line);
    if (!interactive) {
        fclose(input);
    }

    exit(0);
}
