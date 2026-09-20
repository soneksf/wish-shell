#pragma once

#include <string>
#include <vector>

#include "parser.h"

// Етап 5: перенаправлення stdout і stderr у файл.
class Shell {
public:
    Shell();
    void run_line(const std::string& line);

private:
    std::vector<std::string> path_;

    static bool is_builtin(const std::string& name);
    void run_builtin(const Command& cmd);

    std::string resolve(const std::string& name) const;
    void run_external(const Command& cmd) const;
};
