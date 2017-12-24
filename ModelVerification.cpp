#include "stdafx.h"
#include <locale.h>
#include <fstream>
#include <vector>
#include "Core.h"
#include "IOModule.h"
#include "UsefulFunctions.h"

int main(int argc, char** argv)
{	
	setlocale(LC_ALL, "RUS");

	// Управляющий параметр командной строки
	// s - проверка студенческой модели (student)
	// t - генерация файла параметров для эталона (teacher)	
	char controlparam = 's';
	std::string flname = "TestFile.txt", theme = "Test";

	if (argc > 1) {
		controlparam = argv[1][0];
		if (argc > 2) {
			flname.clear();
			flname.append(argv[2]);
			if (argc > 3) {
				theme.clear();
				theme.append(argv[3]);
			}
			else
			{
				printf_s("\nError. Usage: ModelFerification.exe file_name theme_name\n");
				system("pause");
				return -1;
			}
		}
	}
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

	if (std::string::npos == flname.find_last_of(".txt"))
		flname.append(".txt");

	std::fstream FileOut;
	std::streambuf *save;

	FileOut.open("txt files/results/logfile_" + flname, std::ios_base::out);
	save = std::cout.rdbuf();
	std::cout.rdbuf(FileOut.rdbuf());

	std::cout << '\n' << "Control parameter: '" << controlparam << "'\n";
	
	try {
		VCORE::VerificationCore MCore;

		MCore.setnamesuffix(flname);
		MCore.setThemeName(theme);

		if (controlparam == 's')
			MCore.start();
		else
			if (controlparam == 't')
				MCore.genStandParam();
	}
	catch (const char *ErrMSG) {
		std::cout << '\n' << ErrMSG << '\n';
		std::cout.rdbuf(save);
		FileOut.close();
		if (argc <= 1) {
			std::cout << "\nРабота программы завершена с ошибкой.\nЛог выведен в файл /txt files/results/logfile_" + flname << '\n';
			system("pause");
		}
		return -1;
	}
	std::cout << "\nExit status 0.\n";
	std::cout.rdbuf(save);
	FileOut.close();
	if (argc <= 1) {
		std::cout << "\nРабота программы завершена без ошибок.\nРезультаты выведены в файл /txt files/results/VerifResults_" + flname << '\n';
		system("pause");
	}

    return 0;
}