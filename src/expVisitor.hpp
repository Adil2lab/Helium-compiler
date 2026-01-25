#pragma once
#include <iostream>
#include <sstream>
#include "utils.hpp"

struct Gen_ExpVisitor {
    std::stringstream& out;

    void operator()(const ExpRetNIdent& exp) {
        out << "    mov rdi, " << exp.token.value.value() << "\n";
    }
    void operator()(const ExpRetIdent& exp) {
        out << "    mov rdi, " << exp.token.value.value() << "\n";
    }
};