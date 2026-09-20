#include "shell.h"

#include <cstdio>
#include <cstdlib>
#include <vector>

#include <sys/wait.h>
#include <unistd.h>

#include "error.h"
#include "parser.h"

Shell::Shell() : path_{"/bin"} {}

bool Shell::is_builtin(const std::string& name) {
    return name == "exit" || name == "cd" || name == "path";
}

void Shell::run_builtin(const std::vector<std::string>& toks) {
    const std::string& name = toks[0];

    if (name == "exit") {
        if (toks.size() != 1) {
            print_error();
            return;
        }
        exit(0);
    }

    if (name == "cd") {
        if (toks.size() != 2) {
            print_error();
            return;
        }
        if (chdir(toks[1].c_str()) != 0) {
            print_error();
        }
        return;
    }


    path_.assign(toks.begin() + 1, toks.end());
}

std::string Shell::resolve(const std::string& name) const {
    for (const auto& dir : path_) {
        std::string full = dir;
        if (!full.empty() && full.back() != '/') {
            full += '/';
        }
        full += name;
        if (access(full.c_str(), X_OK) == 0) {
            return full;
        }
    }
    return std::string();
}

void Shell::run_line(const std::string& line) {
    const std::vector<std::string> toks = tokenize(line);
    if (toks.empty()) {
        return;
    }

    if (is_builtin(toks[0])) {
        run_builtin(toks);
        return;
    }

    const std::string exe = resolve(toks[0]);
    if (exe.empty()) {
        print_error();
        return;
    }

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
