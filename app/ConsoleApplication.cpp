#include "FileCompressor.h"
#include <fstream>
#include <iostream>
#include <string>
#include <memory>

/// 1 - command
/// 2 - input file(name/direction)
/// 3 - output file(name/direction)
int main(int argc, char **argv) {
    try  {
        if (argc != 4) {
			throw std::runtime_error("WRONG ARGUMENTS");
		}

        std::string command = argv[1];
		if(command == "encrypt") {
			encrypt(argv[2], argv[3]);
		} else if(command == "decrypt") {
			decrypt(argv[2], argv[3]);
        } else {
            throw std::runtime_error("UNDEF OPERATION");
        }
    }
	catch (std::runtime_error const &e) {
        std::cerr << e.what() << '\n';
        return 1;
    }
	return 0;
}
