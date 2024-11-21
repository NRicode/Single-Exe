#include <iostream>
#include <filesystem>
#include <string>
#include <vector>
#include <fstream>

#include <windows.h>
#include <shlobj.h>
#include <commdlg.h>

#include "zip.h"
#include "helper.h"
#include "headerStruct.h"

std::string selectFolder(){

    BROWSEINFO bi = { 0 };
    bi.lpszTitle = "Select a Folder";  
    bi.ulFlags = BIF_USENEWUI | BIF_RETURNONLYFSDIRS;  

    PIDLIST_ABSOLUTE pidl = SHBrowseForFolder(&bi);
    if (pidl != nullptr)
    {

        char folderPath[MAX_PATH];
        if (SHGetPathFromIDList(pidl, folderPath))
        {

            std::string selectedFolder = folderPath;
            CoTaskMemFree(pidl);
            return selectedFolder;

        }

        CoTaskMemFree(pidl);

    }

    return "";

}

namespace fs = std::filesystem;
void walk(struct zip_t* zip, const fs::path directory, unsigned long long* packedCount) {

    try {

		for (const auto entry : fs::recursive_directory_iterator(directory)) {

			if (fs::is_regular_file(entry.status())) {
			
				std::string pathStr = fs::relative(entry.path(),directory).string();
				replaceSubstring(pathStr, "\\", "/");

				zip_entry_open(zip, pathStr.c_str()); 
				zip_entry_fwrite(zip, entry.path().string().c_str());
				(*packedCount)++;
				zip_entry_close(zip);

			}

		}

    } catch (const std::exception e) {

        std::cerr << "Error: " << e.what() << std::endl;

    }

}

void generateStub(const char* launchCommand){

	char currentPath[MAX_PATH];
    if (GetModuleFileNameA(NULL, currentPath, MAX_PATH)) {

		//current mingw compiler bug causes copy_options::overwrite_existing doesn't work on windows
		if (std::filesystem::exists("./portable.exe")) {
			std::filesystem::remove("./portable.exe");
		}

		if(std::filesystem::copy_file(currentPath, "./portable.exe", std::filesystem::copy_options::overwrite_existing)){

			std::ofstream outFile("./portable.exe", std::ios::binary | std::ios::app);

			if (!outFile) {
				std::cerr << "Error opening output file" << std::endl;
				return;
			}

			std::ifstream inputFile("./temp", std::ios::binary);
			if (!inputFile) {

				std::cerr << "Error opening source file" << std::endl;
				return;

			}

			const size_t bufferSize = 10240;
			std::vector<char> buffer(bufferSize);
			while (inputFile.read(buffer.data(), buffer.size()) || inputFile.gcount() > 0) {

				outFile.write(buffer.data(), inputFile.gcount());

			}

			unsigned long long zipSize = std::filesystem::file_size("./temp");

			Header header;
			strcpy(header.signatureText, "EXECUTABLE_PACKER_SIGNATURE_TEXT");

			std::string randStr = generateRandomString(32);
			strcpy(header.uid, randStr.c_str());
			strcpy(header.launchCommand, launchCommand);
			header.zipSize = zipSize;

			outFile.write((const char*)(&header), sizeof(Header));
			outFile.close();
			inputFile.close();

			std::filesystem::remove("./temp");


		} else {

			std::cerr << "Error copying file" << std::endl;

		}

	} else {

		std::cerr << "Error getting module path" << std::endl;

	}

}

void pack(const char* path, const char* launchCommand, int level, unsigned long long* packedCount){

	struct zip_t* zip = zip_open("temp", level, 'w');
	walk(zip, path, packedCount); 
	zip_close(zip);
	generateStub(launchCommand);

}
