#pragma once

#include <string>
#include <vector>

#include <sys/types.h>

#include "parser.h"

class Shell {
public:
    Shell();

    // Розбирає та виконує один рядок вводу (паралельні команди — одночасно).
    void run_line(const std::string& line);

private:
    std::vector<std::string> path_;  // шлях пошуку виконуваних файлів

    static bool is_builtin(const std::string& name);
    void run_builtin(const Command& cmd);

    // Шукає виконуваний файл у path_. Порожній рядок => не знайдено.
    std::string resolve(const std::string& name) const;

    // Породжує дочірній процес. -1 => помилка (повідомлення вже виведено).
    pid_t spawn(const Command& cmd) const;
};
