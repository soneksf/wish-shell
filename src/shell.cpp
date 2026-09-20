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
    if (toks.empty()) {
        return;
    }

    if (toks[0] == "exit") {
        if (toks.size() != 1) {
            print_error();
            return;
        }
        exit(0);
    }


    const std::string exe = "/bin/" + toks[0];

    fflush(nullptr);

    const pid_t pid = fork();
    if (pid < 0) {
        print_error();
        return;
    }

    if (pid == 0) {
        std::vector<char*> args;
        for (const auto& t : toks) {
            args.push_back(const_cast<char*>(t.c_str()));
        }
        args.push_back(nullptr);

        execv(exe.c_str(), args.data());

        print_error();
        _exit(1);
    }

    waitpid(pid, nullptr, 0);
}
