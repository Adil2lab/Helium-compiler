#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "./tokenizer.hpp"
#include "generator.hpp"
#include "parser.hpp"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "Incorrect usage. Please provide exactly one argument." << std::endl;
        std::cerr << "Correct usage is:" << std::endl;
        std::cerr << "  ./hel <file.hlm>" << std::endl;
        std::cerr << "  or " << std::endl;
        std::cerr << "  hel <file.hlm>" << std::endl;
        return EXIT_FAILURE;
    }

    std::string contents;
    {
        std::stringstream contents_stream;
        std::fstream input(argv[1], std::ios::in);
        contents_stream << input.rdbuf();
        contents = contents_stream.str();
    }

    Tokenizer tokenizer(std::move(contents));

    std::vector<Token> tokens = tokenizer.tokenize();

    // File deletion for debugging
    {
        std::filesystem::path a_path = "out.asm";
        std::filesystem::path o_path = "out.o";
        std::filesystem::path e_path = "out";

        try {
            if (std::filesystem::exists(a_path) || std::filesystem::exists(o_path) || std::filesystem::exists(e_path)) {
                if (std::filesystem::remove(a_path)) {
                    std::cout << "File '" << a_path << "' deleted successfully." << std::endl;
                }
                if (std::filesystem::remove(o_path)) {
                    std::cout << "File '" << o_path << "' deleted successfully." << std::endl;
                }
                if (std::filesystem::remove(e_path)) {
                    std::cout << "File '" << e_path << "' deleted successfully." << std::endl;
                }
            }
        } catch (const std::filesystem::filesystem_error &ex) {
            std::cerr << "Filesystem error: " << ex.what() << std::endl;
            return EXIT_FAILURE;
        }
    }

    Parser parser(std::move(tokens));
    std::optional<NodeRet> tree = parser.parse();
    if (!tree.has_value()) {
        std::cerr << "Failed to parse AST." << std::endl;
        return EXIT_FAILURE;
    }
    Generator generator(std::move(tree.value()));

    {
        std::fstream file("out.asm", std::ios::out);
        file << generator.generate();

        // file.close();
        //system("rm -f out.asm");
    }

    system("nasm -felf64 out.asm");
    system("ld -o out out.o");


    return EXIT_SUCCESS;
}
