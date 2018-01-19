#include "stdafx.h"
#include <fstream>
#include <fcntl.h>
#include <io.h>
#include "Core.h"


int wmain(int argc, wchar_t* argv[])
{	
	// Управляющий параметр командной строки
	// s - проверка студенческой модели (student)
	// t - генерация файла параметров для эталона (teacher)	
	wchar_t controlparam = L's';	
	std::wstring flname = L"Model_1.txt", theme = L"Test";

	//
	// _setmode - Отображение wide char в консоли. Источник:
	// https://ru.stackoverflow.com/questions/459154/%D0%A0%D1%83%D1%81%D1%81%D0%BA%D0%B8%D0%B9-%D1%8F%D0%B7%D1%8B%D0%BA-%D0%B2-%D0%BA%D0%BE%D0%BD%D1%81%D0%BE%D0%BB%D0%B8/459299#459299
	//
	_setmode(_fileno(stdout), _O_U8TEXT);
	_setmode(_fileno(stdin), _O_U8TEXT);
	_setmode(_fileno(stderr), _O_U8TEXT);

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
				wprintf_s(L"\nError. Usage: ModelFerification.exe file_name theme_name\n");
				system("pause");
				return -1;
			}
		}
	}
	else
	{
		wprintf_s(L"\n(параметры:\ts - проверка студенческой модели);"
				"\n(\t\tt - генерация эталонных параметров).\n"
				"Введите параметр: ");
		controlparam = getwchar();
		if (controlparam != L's' && controlparam != L't') {
			wprintf_s(L"\nНеизвестный параметр. Параметр по умолчанию - 's'\n");
			controlparam = L's';
		}
	}

	if (std::wstring::npos == flname.find_last_of(L".txt"))
		flname.append(L".txt");

	std::wfstream FileOut;
	std::wstreambuf *savebuf;

	FileOut.open(L"txt files/results/logfile_" + flname, std::ios_base::out);
	savebuf = std::wcout.rdbuf();
	std::wcout.rdbuf(FileOut.rdbuf());
	std::wcout.imbue(std::locale(std::locale::empty(), new std::codecvt_utf8<wchar_t>));

	std::wcout << L'\n' << L"Control parameter: '" << controlparam << L'\n';
	
	try {
		VCORE::VerificationCore MCore;

		MCore.setnamesuffix(flname);
		MCore.setThemeName(theme);

		if (controlparam == L's')
			MCore.start();
		else
			if (controlparam == L't')
				MCore.genStandParam();
	}
	catch (const wchar_t *ErrMSG) {
		std::wcout << L'\n' << ErrMSG << L'\n';
		std::wcout.rdbuf(savebuf);
		FileOut.close();
		// argc <= 1 - mean that program run in console mode.
		if (argc <= 1) {
			std::wcout << L"\nРабота программы завершена с ошибкой.\nЛог выведен в файл /txt files/results/logfile_" + flname << L'\n';
			system("pause");
		}
		return -1;
	}
	std::wcout << L"\nExit status 0.\n";
	std::wcout.rdbuf(savebuf);
	FileOut.close();
	if (argc <= 1) {
		std::wcout << L"\nРабота программы завершена без ошибок.\nРезультаты выведены в файл /txt files/results/VerifResults_" + flname << L'\n';
		system("pause");
	}

    return 0;
}