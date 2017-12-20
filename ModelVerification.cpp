#include "stdafx.h"
#include <locale.h>
#include <fstream>
#include <vector>
#include "Core.h"
#include "IOModule.h"

int main()
{	
	setlocale(LC_ALL, "RUS");

	std::fstream FileOut;
	std::streambuf *save;

	FileOut.open("txt files/results/TestLog.txt", std::ios_base::out);
	save = std::cout.rdbuf();
	std::cout.rdbuf(FileOut.rdbuf());
	
	try {
		std::cout << "\nTEST\n";
		VCORE::VerificationCore MCore;
		
		//std::vector<std::string> testvec;

		//INPUT::IO testIO("txt files/TestStandardParamStr.txt");
		//testIO.readParamStr(testvec);
		//testIO.readParamStr(testvec);
		//testIO.readParamStr(testvec);
		
		MCore.setFilename("txt files/TestFile.txt");
		MCore.setParamFlName("txt files/TestStandardParamStr.txt");
		MCore.start();
		//MCore.setFilename("txt files/TestStandFile.txt");
		//MCore.genStandParam("txt files/TestStandardParamStr.txt");
	}
	catch (const char *ErrMSG) {
		std::cout << '\n' << ErrMSG << '\n';
		std::cout.rdbuf(save);
		FileOut.close();
		return -1;
	}

	std::cout.rdbuf(save);
	FileOut.close();
    return 0;
}