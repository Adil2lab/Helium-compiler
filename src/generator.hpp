//
// Created by adil on 9/7/25.
//

#pragma once
#include <sstream>
#include <string>
#include <variant>

#include "expVisitor.hpp"
#include "utils.hpp"

class Generator {
public:
    inline explicit Generator(std::stringstream& exout) : _exout(exout) {}

    std::stringstream& _exout;
    std::stringstream _data;

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
    void init(std::stringstream& out) {
        /*if (!_isInitiated) {
            out << "section .text\n";
            out << "    global _start\n";
            out << "_start:\n";
            out << "    ;; Program entry point\n";
            out << "    push rbp\n";
            out << "    mov rbp, rsp\n";
            out << "    sub rsp, 16\n\n";
            _isInitiated = true;
        }
        return;*/
    }
    [[nodiscard]] std::string gen_RetStmt(NodeRet& node) {
        std::stringstream out;
        // init(out);
        out << "    mov rax, 60\n";
        Gen_ExpVisitor exp_visitor{out};
        std::visit(exp_visitor, node.exp.token);
        out << "    syscall";
        _exout << out.str();
        return out.str();
    }
    [[nodiscard]] std::string gen_VarDeclStmt(NodeVarDecl& node) {
        std::stringstream out;
        int a;
        std::string d;
        init(out);
        if (node.dataType == DataType::Int) {
            a = 4;
            d = "dword";
        } else if (node.dataType == DataType::Char) {
            a = 1;
            d = "byte";
        }
        out << "    sub rsp," << a << "\n";
        out << "    mov " << d << "[rsp]," << node.exp.value().token.value.value() << "\n";

        _exout << out.str();
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
    static bool _isInitiated;
    static bool _islast;

    bool isAlright() {
        return _isInitiated && _islast;
    }
};
