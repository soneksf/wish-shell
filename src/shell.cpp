#include "shell.h"

#include <cstdio>
#include <cstdlib>
#include <vector>

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>

#include "error.h"

Shell::Shell() : path_{"/bin"} {}

bool Shell::is_builtin(const std::string& name) {
    return name == "exit" || name == "cd" || name == "path";
}

void Shell::run_builtin(const Command& cmd) {
    const std::string& name = cmd.argv[0];

    if (name == "exit") {
        if (cmd.argv.size() != 1) {
            print_error();
            return;
        }
        exit(0);
    }

    if (name == "cd") {
        if (cmd.argv.size() != 2) {
            print_error();
            return;
        }
        if (chdir(cmd.argv[1].c_str()) != 0) {
            print_error();
        }
        return;
    }


    path_.assign(cmd.argv.begin() + 1, cmd.argv.end());
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

pid_t Shell::spawn(const Command& cmd) const {
    const std::string exe = resolve(cmd.argv[0]);
    if (exe.empty()) {
        print_error();
        return -1;
    }

    fflush(nullptr);

    const pid_t pid = fork();
    if (pid < 0) {
        print_error();
        return -1;
    }

    if (pid == 0) {
        if (!cmd.outfile.empty()) {
            const int fd = open(cmd.outfile.c_str(), O_CREAT | O_WRONLY | O_TRUNC, 0644);
            if (fd < 0) {
                print_error();
                _exit(1);
            }

            if (dup2(fd, STDOUT_FILENO) < 0 || dup2(fd, STDERR_FILENO) < 0) {
                print_error();
                _exit(1);
            }
            close(fd);
        }

        std::vector<char*> args;
        args.reserve(cmd.argv.size() + 1);
        for (const auto& a : cmd.argv) {
            args.push_back(const_cast<char*>(a.c_str()));
        }
        args.push_back(nullptr);

        execv(exe.c_str(), args.data());


        print_error();
        _exit(1);
    }

    return pid;
}

void Shell::run_line(const std::string& line) {
    std::vector<pid_t> children;

    for (const auto& cmd : parse_line(line)) {
        if (!cmd.valid) {
            print_error();
            continue;
        }
        if (cmd.argv.empty()) {
            continue;
        }
        if (is_builtin(cmd.argv[0])) {
            run_builtin(cmd);
            continue;
        }
        const pid_t pid = spawn(cmd);
        if (pid > 0) {
            children.push_back(pid);
        }
    }


    for (const pid_t pid : children) {
        waitpid(pid, nullptr, 0);
    }
}
