#pragma once
#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include <optional>
#include "./utils.hpp"

class Tokenizer {
public:
    inline explicit Tokenizer(std::string src) : m_src(std::move(src)) {
    }

    std::vector<Token> tokenize() {
        std::string buff;
        std::vector<Token> tokens;

        while (peak().has_value()) {
            if (std::isalpha(peak().value())) {
                buff.push_back(consume());
                while (peak().has_value() && std::isalnum(peak().value())) {
                    buff.push_back(consume());
                }
                if (buff == "return") {
                    tokens.push_back({TokenType::_return});
                    buff.clear();
                    continue;
                } else if (buff == "fn") {
                    tokens.push_back({TokenType::_ftn, buff});
                    buff.clear();
                    continue;
                } else if (buff == "class") {
                    tokens.push_back({TokenType::_class, buff});
                    buff.clear();
                    continue;
                } else {
                    std::cout << "you've entered an unknown token: " << buff << std::endl;
                    exit(EXIT_FAILURE);
                }
            } else if (std::isdigit(peak().value())) {
                buff.push_back(consume());
                while (peak().has_value() && std::isdigit(peak().value())) {
                    buff.push_back(consume());
                }
                tokens.push_back({TokenType::Int_lit, buff});
                buff.clear();
                continue;
            } else if (peak().value() == '"') {
                buff.push_back(consume());
                while (peak().has_value() && peak().value() != '"') {
                    buff.push_back(consume());
                }
                tokens.push_back({TokenType::String, buff});
                buff.clear();
                continue;
            } else if (peak().value() == ';') {
                consume();
                tokens.push_back({TokenType::SemCln});
                continue;
            } else if (peak().value() == '(') {
                buff.push_back(consume());
                tokens.push_back({TokenType::openParen, buff});
                buff.clear();
                continue;
            } else if (peak().value() == ')') {
                buff.push_back(consume());
                tokens.push_back({TokenType::closeParen, buff});
                buff.clear();
                continue;
            } else if (std::isspace(peak().value())) {
                consume();
                continue;
            } else {
                // return {Token{TokenType::Unknown, std::string(1, c)}};
                std::cerr << "You messed up ......" << std::endl;
                exit(EXIT_FAILURE);
            }
        }
        m_index = 0;
        return tokens;
    }

private:
    [[nodiscard]] inline std::optional<char> peak(int offset = 0) const {
        if (m_index + offset >= m_src.size()) {
            return {};
        } else {
            return m_src.at(m_index + offset);
        }
    }

    inline char consume() {
        if (peak().value() == '"') {
            m_index++;
        }
        return m_src.at(m_index++);
    }

    const std::string m_src;
    size_t m_index = 0;
};
