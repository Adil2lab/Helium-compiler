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
    _ix,
    _ftn,
    _return,
    Unknown
};

struct Token {
    TokenType type;
    std::optional<std::string> value;
    int line;
    int column;
};

struct NodeExp {
    Token token;
};

struct NodeRet {
    NodeExp exp;
};