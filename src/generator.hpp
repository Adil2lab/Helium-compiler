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
    inline explicit Generator() {}

    // -- Windows 64 -- start --

    void gen_drectiveHeader(const std::vector<std::string>& libraries, std::stringstream& out) {
        
    }

    [[nodiscard]] void gen_win_RetStmt(NodeRet& node, std::stringstream& out) const {
        out << "    .global main\nmain:\n";
        out << "    mov rax, 60\n";
        Gen_ExpVisitor exp_visitor{out};
        std::visit(exp_visitor, node.exp.token);
        out << "    syscall\n";
        return;
    }

    // -- Windows 64 -- end --
    // -- Linux 64 -- start --

    [[nodiscard]] std::string gen_lin_RetStmt(NodeRet& node, std::stringstream& out) {
        out << "    .global main\nmain:\n";
        out << "    mov rax, 60\n";
        Gen_ExpVisitor exp_visitor{out};
        std::visit(exp_visitor, node.exp.token);
        out << "    syscall\n";
        return;
    }
    [[nodiscard]] std::string gen_lin_VarDeclStmt(NodeVarDecl& node) {
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
        out << "    sub rsp," << quantity << "\n";
        out << "    mov " << byteSize << "[rsp]," << node.exp.value().token.value.value() << "\n";

        return out.str();
    }

    // -- Linux 64 -- end --

    int gen_Progam(std::stringstream& assemblyCode) {

    }
private:
};
