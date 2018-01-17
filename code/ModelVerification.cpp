#include "stdafx.h"
#include <locale.h>
#include <fstream>
#include <vector>
#include "Core.h"
#include "IOModule.h"

int main(int argc, char** argv)
{	
	setlocale(LC_ALL, "RUS");

	// Управляющий параметр командной строки
	// s - проверка студенческой модели (student)
	// t - генерация файла параметров для эталона (teacher)	
	char controlparam;

	if (argc > 1)
		controlparam = argv[1][0];
	else
	{
		printf_s("\n(параметры:\ts - проверка студенческой модели);"
				"\n(\t\tt - генерация эталонных параметров).\n"
				"Введите параметр: ");
		controlparam = getchar();
		if (controlparam != 's' && controlparam != 't') {
			printf_s("\nНеизвестный параметр. Параметр по умолчанию - 's'\n");
			controlparam = 's';
		}
	}

	std::fstream FileOut;
	std::streambuf *save;

	FileOut.open("txt files/results/TestLog.txt", std::ios_base::out);
	save = std::cout.rdbuf();
	std::cout.rdbuf(FileOut.rdbuf());

	std::cout << '\n' << "Control parameter: '" << controlparam << "'\n";
	
	try {
		VCORE::VerificationCore MCore;

		if (controlparam == 's') {
			MCore.setFilename("txt files/TestFile.txt");
			MCore.setParamFlName("txt files/TestStandardParamStr.txt");
			MCore.start();
		}
		else
			if (controlparam == 't') {
				MCore.setFilename("txt files/TestStandFile.txt");
				MCore.genStandParam("txt files/TestStandardParamStr.txt");
			}
	}
	catch (const char *ErrMSG) {
		std::cout << '\n' << ErrMSG << '\n';
		std::cout.rdbuf(save);
		FileOut.close();
		if (argc <= 1) {
			printf_s("\nРабота программы завершена с ошибкой.\n"
				"Лог выведен в файл /txt files/results/TestLog.txt\n");
			system("pause");
		}
		return -1;
	}

	std::cout.rdbuf(save);
	FileOut.close();
	if (argc <= 1) {
		printf_s("\nРабота программы завершена без ошибок.\n"
			"Результаты выведены в файл /txt files/results/TestVerificationResults.txt\n");
		system("pause");
	}

    return 0;
}