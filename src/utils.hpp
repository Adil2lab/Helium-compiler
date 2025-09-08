//
// Created by adil on 9/6/25.
//

#pragma once

enum class TokenType {
    Identifier,
    Int_lit,
    String,
    Symbol,
    SemCln,
    _class,
    _ftn,
    _return,
    openParen,
    closeParen
};

struct Token {
    TokenType type;
    std::optional<std::string> value;
    int line;
    int column;
    size_t orderParen;
};

struct NodeExp {
    Token token;
};

struct NodeRet {
    NodeExp exp;
};