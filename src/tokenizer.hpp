#pragma once
#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include <optional>
#include "./utils.hpp"

class Tokenizer {
public:
    inline explicit Tokenizer(std::string &src) : m_src(std::move(src)) {
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
                    tokens.push_back({TokenType::_return, std::nullopt, m_line, m_token});
                    buff.clear();
                    continue;
                } else if (buff == "fn") {
                    tokens.push_back({TokenType::_functionNode, buff, m_line, m_token});
                    buff.clear();
                    continue;
                } else if (buff == "class") {
                    tokens.push_back({TokenType::_class, buff, m_line, m_token});
                    buff.clear();
                    continue;
                } else if (buff == "int" || buff == "float" || buff == "String" || buff == "char") {
                    tokens.push_back({TokenType::DataType, buff, m_line, m_token});
                    buff.clear();
                    continue;
                } else {
                    tokens.push_back({TokenType::Identifier, buff, m_line, m_token});
                    buff.clear();
                    continue;
                }
            } else if (std::isdigit(peak().value())) {
                buff.push_back(consume());
                while (peak().has_value() && std::isdigit(peak().value())) {
                    buff.push_back(consume());
                }
                tokens.push_back({TokenType::Int_lit, buff, m_line, m_token});
                buff.clear();
                continue;
            } else if (peak().value() == '=') {
                consume();
                tokens.push_back({TokenType::Symbol, std::string("="), m_line, m_token});
                continue;
            } else if (peak().value() == '"') {
                consume();
                while (peak().has_value() && peak().value() != '"') {
                    buff.push_back(consume());
                }
                consume();
                tokens.push_back({TokenType::String_lit, buff, m_line, m_token});
                buff.clear();
                continue;
            } else if (peak().value() == ';') {
                consume();
                tokens.push_back({TokenType::SemCln, std::nullopt, m_line, m_token});
                continue;
            } else if (peak().value() == '(') {
                consume();
                tokens.push_back({TokenType::openParen, std::nullopt, m_line, m_token});
                continue;
            } else if (peak().value() == ')') {
                consume();
                tokens.push_back({TokenType::closeParen, std::nullopt, m_line, m_token});
                continue;
            } else if (std::isspace(peak().value())) {
                consume();
                continue;
            } else {
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
        char c = m_src.at(m_index++);
        if (c == '\n') {
            m_line++;
            m_token = 1;
        } else {
            m_token++;
        }
        return c;
    }

    const std::string m_src;
    size_t m_index = 0;
    size_t m_line = 1;
    size_t m_token = 1;
};
