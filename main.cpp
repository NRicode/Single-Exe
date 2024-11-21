#include <iostream>
#include <string>
#include <string.h>

#include "header.h"
#include "unpack.h"
#include "gui.h"



int main()
{
	
	//MZ_ZIP_MAX_IO_BUF_SIZE to change buffer size
	Header header;
	readHeader(header);

	if(!strncmp(header.signatureText, "EXECUTABLE_PACKER_SIGNATURE_TEXT", sizeof(signature))){

		unpack(header);
		return 0;

	}

	gui();
   
    return 0;
}
