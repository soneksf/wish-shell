#pragma once

#include <string>
#include <vector>

// Етап 2: розбиття рядка на лексеми за пробілами й табуляціями.
std::vector<std::string> tokenize(const std::string& line);
