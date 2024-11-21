#include <fstream>
#include <iostream>
#include <filesystem>

#include <windows.h>

#include "headerStruct.h"

void readHeader(Header& header){

	char currentPath[MAX_PATH];
    if (GetModuleFileNameA(NULL, currentPath, MAX_PATH)) {

		std::ifstream inputFile(currentPath, std::ios::binary);
		if (!inputFile) {

			std::cerr << "Error reading executable" << std::endl;

		}

		unsigned long long fileSize = std::filesystem::file_size(currentPath);
		unsigned long long headerPos = fileSize - sizeof(Header);
		
		inputFile.seekg(headerPos);
		inputFile.read((char*)&header, sizeof(Header));
		inputFile.close();

	}else{

		std::cerr << "Error getting module path" << std::endl;

	}

}

