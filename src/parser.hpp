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

    // Token parse_paren(const int& line) {

    // }

    std::optional<NodeRetExp> parse_retExp() {
        if (peak().has_value() && peak().value().type == TokenType::Int_lit) {
            return NodeRetExp {.token = ExpRetNIdent { consume() } };
        } else if (peak().has_value() && peak().value().type == TokenType::Identifier) {
            return NodeRetExp {.token = ExpRetIdent { consume() } };
        } // else if (peek().has_value() && peek().value().type == TokenType::openParen) {
        //     return NodeExp {.token = parse_paren()};
        // }
        return std::nullopt;
    }

    std::optional<NodeRet> parse_ret() {
        std::optional<NodeRet> res;
        while (peak().has_value()) {
            if (peak().value().type == TokenType::_return) {
                consume();
                if (auto nodeRetExp = parse_retExp()) {
                     res = NodeRet {.exp = nodeRetExp.value()};
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

    std::optional<ExpVarDecl> parse_expVarDecl() {
        if (peak().has_value() && (peak().value().type == TokenType::Int_lit || peak().value().type == TokenType::Identifier)) {
            return ExpVarDecl{ consume() };
        }
        return std::nullopt;
    }

    std::vector<NodeVarDecl> parse_varDecl() {
        std::vector<NodeVarDecl> res;

        while (peak().has_value()) {
            if (peak().value().type == TokenType::DataType) {
                Token identifierToken;
                std::string dataType = consume().value.value();
                DataType dataTypeValue;

                switch (dataType[0]) {
                    case 'i':
                        dataTypeValue = DataType::Int;
                        break;
                    case 'f':
                        dataTypeValue = DataType::Float;
                        break;
                    case 'S':
                        dataTypeValue = DataType::String;
                        break;
                    case 'c':
                        dataTypeValue = DataType::Char;
                        break;
                    default:
                        std::cerr << "Unknown data type at line " << peak().value().line << " at column " << peak().value().column << std::endl;
                        exit(EXIT_FAILURE);
                }

                if (peak().has_value() && peak().value().type == TokenType::Identifier) {
                    identifierToken = consume();
                } else {
                    std::cerr << "Expected identifier at line " << peak().value().line << " at column " << peak().value().column << std::endl;
                    exit(EXIT_FAILURE);
                }
                if (peak().has_value() && peak().value().type == TokenType::Symbol && peak().value().value == "=") {
                    consume();
                } else {
                    std::cerr << "Expected '=' at line " << peak().value().line << " at column " << peak().value().column << std::endl;
                    exit(EXIT_FAILURE);
                }
                if (auto expVarDecl = parse_expVarDecl()) {
                    res.push_back(NodeVarDecl {
                        .dataType = dataTypeValue,
                        .identifier = identifierToken.value.value(),
                        .exp = expVarDecl
                    });
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

    inline Token consume(bool eat = false) {
        Token& ref = tokens.at(m_index++);
        Token a = std::move(ref);
        if (eat) {
            ref = Token{TokenType::__deleted, std::nullopt, a.line, a.column};
        } else {
            ref = Token{TokenType::__moved, std::nullopt, a.line, a.column};
        }
        return a;
    }

    std::vector<Token> tokens;
    size_t m_index = 0;
};
