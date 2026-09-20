#include "parser.h"

#include <algorithm>

namespace {

bool is_space(char c) {
    return c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\v' || c == '\f';
}

// Будує команду з лексем одного сегмента та перевіряє синтаксис.
Command build_command(const std::vector<std::string>& toks) {
    Command cmd;

    const std::ptrdiff_t redirs = std::count(toks.begin(), toks.end(), std::string(">"));

    if (redirs == 0) {
        cmd.argv = toks;
        return cmd;
    }
    if (redirs > 1) {  // "a > b > c"
        cmd.valid = false;
        return cmd;
    }

    const auto pos = std::find(toks.begin(), toks.end(), std::string(">"));
    const std::vector<std::string> left(toks.begin(), pos);
    const std::vector<std::string> right(pos + 1, toks.end());

    // "> file" (немає команди) або "cmd >" / "cmd > a b" (не один файл)
    if (left.empty() || right.size() != 1) {
        cmd.valid = false;
        return cmd;
    }

    cmd.argv = left;
    cmd.outfile = right[0];
    return cmd;
}

}  // namespace

std::vector<std::string> tokenize(const std::string& line) {
    std::vector<std::string> tokens;
    std::string cur;

    auto flush = [&]() {
        if (!cur.empty()) {
            tokens.push_back(cur);
            cur.clear();
        }
    };

    for (const char c : line) {
        if (is_space(c)) {
            flush();
        } else if (c == '>' || c == '&') {
            flush();
            tokens.emplace_back(1, c);
        } else {
            cur.push_back(c);
        }
    }
    flush();
    return tokens;
}

std::vector<Command> parse_line(const std::string& line) {
    std::vector<Command> commands;
    std::vector<std::string> segment;

    auto finish = [&]() {
        if (!segment.empty()) {
            commands.push_back(build_command(segment));
            segment.clear();
        }
    };

    for (const auto& tok : tokenize(line)) {
        if (tok == "&") {
            finish();
        } else {
            segment.push_back(tok);
        }
    }
    finish();

    return commands;
}
