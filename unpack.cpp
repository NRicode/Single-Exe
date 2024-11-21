#include <iostream>
#include <fstream>
#include <filesystem>
#include "headerStruct.h"

#include <windows.h>

#include "helper.h"
#include "zip.h"

void unpack(Header& header){

	char currentPath[MAX_PATH];
	if (GetModuleFileNameA(NULL, currentPath, MAX_PATH)){

		unsigned long long fileSize = std::filesystem::file_size(currentPath);
		unsigned long long headerPos = fileSize - sizeof(Header);
		unsigned long long zipPos = headerPos -  header.zipSize;

		HANDLE hFile = CreateFileA(
			currentPath,
			GENERIC_READ,
			0,
			NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			NULL
		);

		if (hFile == INVALID_HANDLE_VALUE) {
			std::cerr << "CreateFile failed, error " << GetLastError() << std::endl;
			return;
		}

		HANDLE hMapFile = CreateFileMapping(
			hFile,
			NULL,
			PAGE_READONLY,
			0,
			0,
			NULL
		);

		if (hMapFile == NULL) {
			std::cerr << "CreateFileMapping failed, error " << GetLastError() << std::endl;
			CloseHandle(hFile);
			return;
		}

		LPVOID lpBase = MapViewOfFile(
			hMapFile,
			FILE_MAP_READ,
			0,
			0,
			0
		);

		if (lpBase == NULL) {
			std::cerr << "MapViewOfFile failed, error " << GetLastError() << std::endl;
			CloseHandle(hMapFile);
			CloseHandle(hFile);
			return;
		}

		int arg = 2;

		char tempFolder[MAX_PATH] = { 0 };
		GetTempPathA(MAX_PATH, tempFolder);

		std::string tempFolderPath = std::string(tempFolder);
		replaceSubstring(tempFolderPath, "\\", "/");

		std::string extractPath = tempFolderPath + std::string(header.uid);
		std::string launchCommand = std::string(header.launchCommand);
		std::string executableName = getFirstWord(launchCommand);
		std::string command = removeFirstWord(launchCommand);

		zip_stream_extract((const char*)(((unsigned long long)lpBase) + ((unsigned long long)zipPos)), header.zipSize, extractPath.c_str(), NULL, &arg);

		UnmapViewOfFile(lpBase);
		CloseHandle(hMapFile);
		CloseHandle(hFile);

		STARTUPINFO si;
		PROCESS_INFORMATION pi;
		ZeroMemory( &si, sizeof(si) );
		si.cb = sizeof(si);
		ZeroMemory( &pi, sizeof(pi) );

		if(CreateProcessA(
			(char*)std::string(extractPath + "\\" + executableName).c_str(),
			(char*)std::string(extractPath + "\\" + launchCommand).c_str(),
			NULL,
			NULL,
			FALSE,
			CREATE_NEW_CONSOLE | CREATE_NEW_PROCESS_GROUP,
			NULL,
			extractPath.c_str(),
			&si,
			&pi))

		{
		
			WaitForSingleObject(pi.hProcess, INFINITE );
			std::filesystem::remove_all(extractPath);

			CloseHandle( pi.hProcess );
			CloseHandle( pi.hThread );

		}

	}else{

		std::cerr << "Error getting module path" << std::endl;

	}

}
