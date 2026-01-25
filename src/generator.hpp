//
// Created by adil on 9/7/25.
//

#pragma once
#include <algorithm>
#include <sstream>
#include <string>
#include <variant>

#include "expVisitor.hpp"
#include "utils.hpp"

class Generator {
public:
    inline explicit Generator() {}

    #ifdef WIN32
    [[nodiscard]] std::string gen_RetStmt(NodeRet& node) const {
        std::stringstream out;
        out << "    .global main\nmain:\n";
        out << "    mov rax, 60\n";
        Gen_ExpVisitor exp_visitor{out};
        std::visit(exp_visitor, node.exp.token);
        out << "    syscall\n";
        return out.str();
    }
    #endif
    #ifdef __linux__
    [[nodiscard]] std::string gen_RetStmt(NodeRet& node) const {
        std::stringstream out;
        out << "global _start\n_start:\n";
        out << "    mov rax, 60\n";
        Gen_ExpVisitor exp_visitor{out};
        std::visit(exp_visitor, node.exp.token);
        out << "    syscall";
        return out.str();
    }
    #endif
    #ifdef __APPLE__
    [[nodiscard]] std::string gen_RetStmt(NodeRet& node) const {
        std::stringstream out;
        out << "    .global _main\n_main:\n";
        out << "    mov rax, 0x2000001\n"; // syscall: exit
        Gen_ExpVisitor exp_visitor{out};
        std::visit(exp_visitor, node.exp.token);
        out << "    syscall\n";
        return out.str();
    }
    #endif

private:
    const std::optional<NodeRet> _root;
};
