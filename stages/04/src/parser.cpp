#include "parser.h"

namespace {
bool is_space(char c) {
    return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}
}  // namespace

std::vector<std::string> tokenize(const std::string& line) {
    std::vector<std::string> tokens;
    std::string cur;

    for (const char c : line) {
        if (is_space(c)) {
            if (!cur.empty()) {
                tokens.push_back(cur);
                cur.clear();
            }
        } else {
            cur.push_back(c);
        }
    }
    if (!cur.empty()) {
        tokens.push_back(cur);
    }
    return tokens;
}
