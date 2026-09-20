#include "shell.h"

#include <cstdio>
#include <cstdlib>
#include <vector>

#include <sys/wait.h>
#include <unistd.h>

#include "error.h"
#include "parser.h"

void Shell::run_line(const std::string& line) {
    const std::vector<std::string> toks = tokenize(line);
    if (toks.empty()) {  // порожній рядок — не помилка
        return;
    }

    if (toks[0] == "exit") {
        if (toks.size() != 1) {  // exit не приймає аргументів
            print_error();
            return;
        }
        exit(0);
    }

    // Поки що шукаємо програму лише в /bin — шлях пошуку буде на етапі 3.
    const std::string exe = "/bin/" + toks[0];

    fflush(nullptr);  // щоб буфери батька не продублювалися в нащадку

    const pid_t pid = fork();
    if (pid < 0) {
        print_error();
        return;
    }

    if (pid == 0) {  // дочірній процес
        std::vector<char*> args;
        for (const auto& t : toks) {
            args.push_back(const_cast<char*>(t.c_str()));
        }
        args.push_back(nullptr);

        execv(exe.c_str(), args.data());

        print_error();  // сюди потрапляємо, лише якщо execv не спрацював
        _exit(1);
    }

    waitpid(pid, nullptr, 0);  // батько чекає на нащадка
}
