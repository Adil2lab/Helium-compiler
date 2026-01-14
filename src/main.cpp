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
    if (argc < 2) {
        std::cerr << "Incorrect usage. Please provide exactly one argument." << std::endl;
        std::cerr << "Correct usage is:" << std::endl;
        std::cerr << "  ./hel <file.qlm>" << std::endl;
        std::cerr << "  or " << std::endl;
        std::cerr << "  hel <file.qlm>" << std::endl;
        std::cerr << "Or run this:" << std::endl;
        std::cerr << "  ./hel --help" << std::endl;
        return EXIT_FAILURE;
    }
    else if (!(std::string(argv[1]).ends_with(".qlm") || std::string(argv[1]).ends_with(".QLM") || std::string(argv[2]).ends_with(".qlm") || std::string(argv[2]).ends_with(".QLM"))) {
        std::cerr << "Error: given files are not supported. Please use .qlm files instead." << std::endl;
        return EXIT_FAILURE;
    }
    else if (!(std::filesystem::exists(argv[1]))) {
        std::cerr << "Error: file '" << argv[1] << "' does not exist." << std::endl;
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
    // {
    //     std::filesystem::path a_path = "out.asm";
    //     std::filesystem::path o_path = "out.o";
    //     std::filesystem::path e_path = "out";

    //     try {
    //         if (std::filesystem::exists(a_path) || std::filesystem::exists(o_path) || std::filesystem::exists(e_path)) {
    //             if (std::filesystem::remove(a_path)) {
    //                 std::cout << "File '" << a_path << "' deleted successfully." << std::endl;
    //             }
    //             if (std::filesystem::remove(o_path)) {
    //                 std::cout << "File '" << o_path << "' deleted successfully." << std::endl;
    //             }
    //             if (std::filesystem::remove(e_path)) {
    //                 std::cout << "File '" << e_path << "' deleted successfully." << std::endl;
    //             }
    //         }
    //     } catch (const std::filesystem::filesystem_error &ex) {
    //         std::cerr << "Filesystem error: " << ex.what() << std::endl;
    //         return EXIT_FAILURE;
    //     }
    // }

    Parser parser(std::move(tokens));
    std::optional<NodeRet> tree = parser.parse_ret();
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

    if (argc > 2 && (std::string(argv[2]) == "--run-with-delete" || std::string(argv[2]).find("-rd") != std::string::npos)) {
        system("./out");
        system("rm -f out.asm out.o out");
        return EXIT_SUCCESS;
    }

    // Moving executable to the same directory as the input file
    {
        std::string filen = std::string(argv[1]);

        filen.erase(filen.find_last_of('/') + 1, filen.length() - (filen.find_last_of('/') + 1));

        filen += "out";
        system(("mv out " + filen).c_str());
    }

    return EXIT_SUCCESS;
}