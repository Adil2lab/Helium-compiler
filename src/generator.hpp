//
// Created by adil on 9/7/25.
//

#pragma once
#include <algorithm>
#include <sstream>
#include <string>

#include "utils.hpp"

class Generator {
public:
    inline explicit Generator(NodeRet root) : _root(std::move(root)) {}
    // Generator(const Generator&) = delete;

    [[nodiscard]] std::string generate() const {
        std::stringstream out;
        out << "global _start\n_start:\n";
        out << "    mov rax, 60\n";
        out << "    mov rdi, " << _root.exp.token.value.value() << "\n";
        out << "    syscall";
        return out.str();
    }

private:
    const NodeRet _root;
};
