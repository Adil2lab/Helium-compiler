//
// Created by adil on 9/6/25.
//

#pragma once
#include <iostream>
#include <optional>
#include <ostream>
#include <vector>
#include "./utils.hpp"

class Parser {
public:
    inline explicit Parser(std::vector<Token> tokens) : tokens(std::move(tokens)) {
    }

    Token parse_paren(const int& line) {

    }

    std::optional<NodeExp> parse_exp() {
        if (peak().has_value() && peak().value().type == TokenType::Int_lit) {
            return NodeExp {.token = consume()};
        } // else if (peak().has_value() && peak().value().type == TokenType::openParen) {
        //     return NodeExp {.token = parse_paren()};
        // }
        return std::nullopt;
    }

    std::optional<NodeRet> parse_ret() {
        std::optional<NodeRet> res;
        while (peak().has_value()) {
            if (peak().value().type == TokenType::_return) {
                consume();
                if (auto nodeExp = parse_exp()) {
                     res = NodeRet {.exp = nodeExp.value()};
                } else {
                    std::cerr << "Invalid expression at line " << peak().value().line << std::endl;
                    exit(EXIT_FAILURE);
                }
                if (peak().has_value() && peak().value().type == TokenType::SemCln) {
                    consume();
                } else {
                    std::cerr << "Expected ';' at line " << peak().value().line << " at column " << peak().value().column << std::endl;
                    exit(EXIT_FAILURE);
                }
                
            }
        }
        return res;
    }

private:
    [[nodiscard]] inline std::optional<Token> peak(int offset = 0) const {
        if (m_index + offset >= tokens.size()) {
            return {};
        } else {
            return tokens.at(m_index + offset);
        }
    }

    inline Token consume() {
        return tokens.at(m_index++);
    }


    const std::vector<Token> tokens;
    size_t m_index = 0;
};
