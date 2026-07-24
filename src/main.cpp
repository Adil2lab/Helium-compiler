#include <algorithm>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#ifdef WIN32
#include <windows.h> // for getting the file path
#endif

#include "./tokenizer.hpp"
#include "generator.hpp"
#include "parser.hpp"

int main(int argc, char* argv[]) {
	bool there_is_a_error = false; // I know it has a long name, and I am bad at naming stuffs

	// -- CLI work starts here --
	if (argc < 2) {
		std::cerr << "Incorrect usage. Please provide at least the file path." << std::endl;
		std::cerr << "Correct usage is:" << std::endl;
#ifdef WIN32
		std::cerr << "  hel.exe <file.qlm> [<args>]" << std::endl;
		std::cerr << "Or run this:" << std::endl;
		std::cerr << "  hel.exe --help" << std::endl;
#endif // WIN32

#ifdef __LINUX__
		std::cerr << "  ./hel <file.qlm> [<args>]" << std::endl;
		std::cerr << "  or " << std::endl;
		std::cerr << "  hel <file.qlm> [<args>]" << std::endl;
		std::cerr << "Or run this:" << std::endl;
		std::cerr << "  ./hel --help" << std::endl;
#endif // __LINUX__
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

	/*
		-- CLI work ends here --
		-- Actual compiler work starts here --
	*/

	Tokenizer tokenizer(contents);

	std::vector<Token> tokens = tokenizer.tokenize();

	// File deletion code for debugging
	/*
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
	*/

	Parser parser(std::move(tokens));
	std::optional<NodeRet> treeRet = parser.parse_ret();
	if (!treeRet.has_value()) {
		std::cerr << "Failed to parse AST." << std::endl;
		return EXIT_FAILURE;
	}

	std::stringstream _out;
	Generator generator(_out);

	{
		std::fstream file("out.asm", std::ios::out);
		file << generator.gen_RetStmt(treeRet.value()); // I have to make a algorithm to check if there is any return or not.

	}

#ifdef WIN32 
	char path_buff[MAX_PATH];

	GetModuleFileNameA(NULL, path_buff, MAX_PATH);

	std::filesystem::path compiler_dir = std::filesystem::path(path_buff).parent_path();
	std::filesystem::path nasm_path = compiler_dir / "tools" / "nasm.exe";
	std::filesystem::path lld_path = compiler_dir / "tools" / "lld-link.exe";
	std::filesystem::path ld_path = compiler_dir / "tools" / "ld.lld.exe";

	std::string nasm_cmd;
	std::string linker_cmd;

	std::string platform_type;
	std::vector<std::string> libraries;

	enum class Platform {
		Windows64,
		Linux64,
		MacOS,
		HostOS
	};

	Platform platform;

	for (size_t i = 1; i < argc; ++i) {
		if (std::string(argv[i]).starts_with('-')) {
			if (std::string(argv[i]) == "--platform-win64" || std::string(argv[i]) == "-pwin64") {
				size_t j = i + 1;
				if (j == argc) {
					std::cerr << "If you want to build for windows, you have to specify every libraries you are using. You do it with  \'-l\'" << std::endl;
					std::cerr << "And also you have to set the default library  \'kernel32.lib\'  in every program." << std::endl;
					std::cerr << "Let\' say, you want to compile a program which uses  \'printf()\'  . So you would write something like this" << std::endl;
					std::cerr << "	... -pwin64 -l kernel32.lib ucrt.lib ..." << std::endl;
					there_is_a_error = true;
					break;
				}
				else if (std::string(argv[j]) == "-l" && (j + 1) == argc) {
					std::cerr << "You didn\'t mention the libraries after  -l  " << std::endl;
					std::cerr << "You should do it like this" << std::endl;
					std::cerr << "	... -pwin64 -l kernel32.lib ucrt.lib ..." << std::endl;
					there_is_a_error = true;
					break;
				}
				while (j < argc && !(std::string(argv[j]).starts_with('-'))) {
					libraries.push_back(argv[j]);
					++j;
				}
				i = j - 1;
				platform = Platform::Windows64;
			} 
			else if (std::string(argv[i]) == "--platform-linux64" || std::string(argv[i]) == "-plinux64") {
				platform = Platform::Linux64;
			}
			else if (std::string(argv[i]) == "--platform-mac64" || std::string(argv[i]) == "-pmac64") {
				platform = Platform::MacOS;
			}
		}
	}

	if (there_is_a_error) return EXIT_FAILURE;

	if (platform == Platform::Linux64) {
		nasm_cmd = "\"" + nasm_path.string() + "\" -f elf64 out.asm";
		linker_cmd = "\"" + ld_path.string() + "\" -o out out.o";
	}
	else if (platform == Platform::Windows64) {
		nasm_cmd = "\"" + nasm_path.string() + "\" -f win64 out.asm";
		linker_cmd = "\"" + lld_path.string() + "\" out.obj /OUT:out.exe /ENTRY:main /SUBSYSTEM:CONSOLE";
	}
	else if (platform == Platform::MacOS) {
		std::cerr << "Sorry, but macOS is not supported yet. Please use Linux or Windows instead." << std::endl;
		return EXIT_FAILURE;
	}
	else {
		nasm_cmd = "\"" + nasm_path.string() + "\" -f win64 out.asm";
		linker_cmd = "\"" + lld_path.string() + "\" out.obj /OUT:out.exe /ENTRY:main /SUBSYSTEM:CONSOLE";
	}

	system(nasm_cmd.c_str());
	system(linker_cmd.c_str());

	if (argc > 2 && (std::string(argv[2]) == "--run-with-delete" || std::string(argv[2]).find("-rd") != std::string::npos)) { // I need to get a debug arg not a delete arg
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
#endif // WIN32

#ifdef __linux__
	system("nasm -felf64 out.asm");
	system("ld -o out out.o");

	if (argc > 2 && (std::string(argv[2]) == "--run-with-delete" || std::string(argv[2]).find("-rd") != std::string::npos)) { // // I need to get a debug arg not a delete arg
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
#endif // __linux__
}
