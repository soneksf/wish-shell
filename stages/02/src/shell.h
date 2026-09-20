#pragma once

#include <string>

// Етап 2: виконання однієї команди в дочірньому процесі.
class Shell {
public:
    void run_line(const std::string& line);
};
