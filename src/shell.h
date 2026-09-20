#pragma once

#include <string>
#include <vector>

// Етап 3: шлях пошуку виконуваних файлів і вбудована команда path.
class Shell {
public:
    Shell();
    void run_line(const std::string& line);

private:
    std::vector<std::string> path_;  // початково — лише /bin

    // Шукає виконуваний файл у path_. Порожній рядок => не знайдено.
    std::string resolve(const std::string& name) const;
};
