#pragma once

#include <string>
#include <vector>

// Етап 5: команда може мати перенаправлення виводу у файл.
struct Command {
    std::vector<std::string> argv;  // ім'я програми + аргументи
    std::string outfile;            // порожньо => без перенаправлення
    bool valid = true;              // false => синтаксична помилка
};

// '>' виділяється в окрему лексему навіть без пробілів навколо.
std::vector<std::string> tokenize(const std::string& line);

// Розбирає рядок в одну команду й перевіряє синтаксис перенаправлення.
Command parse_line(const std::string& line);
