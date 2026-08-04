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
    [[nodiscard]] std::string gen_RetStmt(NodeRet& node) {
        std::stringstream out;
        out << "    mov rax, 60\n";
        Gen_ExpVisitor exp_visitor{out};
        std::visit(exp_visitor, node.exp.token);
        out << "    syscall";
        _exout << out.str();
        return out.str();
    }
    [[nodiscard]] std::string gen_VarDeclStmt(NodeVarDecl& node) {
        std::stringstream out;
        int quantity;
        std::string byteSize;
        if (node.dataType == DataType::Int) {
            quantity = 4;
            byteSize = "dword";
        } else if (node.dataType == DataType::Char) {
            quantity = 1;
            byteSize = "byte";
        }
        out << "    sub rsp," << a << "\n";
        out << "    mov " << d << "[rsp]," << node.exp.value().token.value.value() << "\n";

        _exout << out.str();
        return out.str();
    }
    #endif
    #ifdef __APPLE__
    
    #endif

private:
    static bool _isInitiated;
    static bool _islast;

    bool isAlright() {
        return _isInitiated && _islast;
    }
};
