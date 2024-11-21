#pragma once
const char signature[] = "EXECUTABLE_PACKER_SIGNATURE_TEXT";
struct Header{

	char signatureText[sizeof(signature)];
	char uid[33];
	char launchCommand[260];
	unsigned long long zipSize;

}; 

