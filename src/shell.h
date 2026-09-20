#pragma once

#include <string>
#include <vector>

// Етап 4: усі три вбудовані команди — exit, cd, path.
class Shell {
public:
    Shell();
    void run_line(const std::string& line);

private:
    std::vector<std::string> path_;

    static bool is_builtin(const std::string& name);
    void run_builtin(const std::vector<std::string>& toks);

    std::string resolve(const std::string& name) const;
};
