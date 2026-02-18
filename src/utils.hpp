//
// Created by adil on 9/6/25.
//

#pragma once

#include <cstddef>
#include <optional>
#include <variant>
#include <string>

enum class DataType {
    Int,
    Float,
    String,
    Char
};

enum class TokenType {
    DataType,
    Identifier,
    Int_lit,
    String_lit,
    Symbol,
    SemCln,
    _class,
    _functionNode,
    _return,
    openParen,
    closeParen,
    __deleted,
    __moved
};

enum class SymbolType {
    Variable,
    Function,
    Parameter
};

enum class StorageType {
    Global,
    Static,
    Stack
};

struct Symbol {
    std::string type;
    SymbolType symbolType;
    StorageType storageType;
    int offset;
    size_t size;
    bool isMutable;
};

struct Token {
    TokenType type;
    std::optional<std::string> value;
    size_t line;
    size_t column;
};

struct ExpVarDecl {
    Token token;
};

/**
 * Represents a variable declaration node in the AST.
 */
struct NodeVarDecl {
    DataType dataType;
    std::string identifier;
    std::optional<ExpVarDecl> exp;
};

/**
 * Represents an expression for return statement that doesn't have identifier.
 */
struct ExpRetNIdent {
    Token token;
};

/**
 * Represents an expression for return statement that is defined by a identifier.
 */
struct ExpRetIdent {
    Token token;
};

struct NodeRetExp {
    std::variant<ExpRetNIdent, ExpRetIdent> token;
};

/**
 * Represents a return node in the AST.
 */
struct NodeRet {
    NodeRetExp exp;
};
