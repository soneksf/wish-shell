#pragma once

#include <string>
#include <vector>

// Одна команда з рядка вводу (сегмент між символами '&').
struct Command {
    std::vector<std::string> argv;  // ім'я програми + аргументи
    std::string outfile;            // порожньо => без перенаправлення
    bool valid = true;              // false => синтаксична помилка
};

// Розбиває рядок на лексеми. Пробіли і табуляції — роздільники;
// '>' та '&' завжди виділяються в окремі лексеми навіть без пробілів.
std::vector<std::string> tokenize(const std::string& line);

// Розбиває рядок на команди по '&' та валідує перенаправлення.
// Порожні сегменти (напр. "cmd &" або рядок з самих пробілів) відкидаються
// без помилки.
std::vector<Command> parse_line(const std::string& line);
