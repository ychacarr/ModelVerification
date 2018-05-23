#include "stdafx.h"
#include "TextAnalyzer.h"
#include "UsefulFunctions.h"
#include "Sentence.h"
#include "Errors.h"
#include <fstream>

namespace TXTANALYZE
{
	unsigned int TxtAnalyzer::Counter = 0;
	//
	// СДЕЛАТЬ СЧИТЫВАНИЕ ИЗ ФАЙЛА, ТАК ЖЕ КАК И MIN_SYMB (и др.) 
	double MAX_NOT_EQUAL = 0.0;
	//
	//

	TxtAnalyzer::TxtAnalyzer()
	{
		ID = ++Counter;
		#ifdef _DEBUG
			std::wcout << L"\nTxtAnalyzer(). Object with ID = " << ID << L" was created.\n";
		#endif // _DEBUG
	}

	TxtAnalyzer::~TxtAnalyzer()
	{
		#ifdef _DEBUG
			std::wcout << L"~TxtAnalyzer(). Object with ID = " << ID << L" was destroyed.\n";
		#endif // _DEBUG
	}

	bool TxtAnalyzer::Analyze(const MODEL::Entity &inEnt, const std::wstring & theme, std::wstring & outname)
	{
		std::wstring keywords, testkey;
		std::size_t notfound = std::wstring::npos;
		Sentence sentenceanalyze(inEnt.getDeterm());
		sentenceanalyze.normalize();
		if (outname.length() != 0)
			outname.clear();
		//TEST CODE START
		{
			// !! Использование файла в обход IOModule
			std::wifstream InFile;
			bool endFlag = false;
			for (unsigned int i = 0; i < sentenceanalyze.getWordcount(); i++) {
				endFlag = false;
				//std::wcout << L'\n' << sentenceanalyze.getWord(i) << L" now analyze.";
				InFile.open(L"txt files/Dictionary_" + theme + L".txt", std::ios_base::in);
				InFile.imbue(std::locale(std::locale::empty(), new std::codecvt_utf8<wchar_t>));
				if (!InFile.is_open())
					ERROR::throwError(L"Error in TxtAnalyzer::Analyze(). Can't open Dictionary_ file. ID - Entity.", inEnt.getID());
				while (!InFile.eof() && !endFlag) {
					std::wstring strsynon;
					std::getline(InFile, testkey, L'\n');
					std::getline(InFile, strsynon, L'\n');
					
				if (notfound != strsynon.find(sentenceanalyze.getWord(i))) {
						// Для исключения повторного добавления ключевых слов
						// Возможно излишяя проверка, но пока пусть будет
						if (notfound == keywords.find(testkey)) {
							keywords += testkey + L" ";
							endFlag = true;
						}
					}
				}
				InFile.close();
				//std::wcout << L'\n' << sentenceanalyze.getWord(i) << L" analyze end.";
			}
			InFile.close();
			if (keywords.length() > 1)
				keywords.erase(keywords.end() - 1);
			testkey.clear();
			InFile.open(L"txt files/StandardDefinit_" + theme + L".txt", std::ios_base::in);
			InFile.imbue(std::locale(std::locale::empty(), new std::codecvt_utf8<wchar_t>));
			if (!InFile.is_open())
				ERROR::throwError(L"Error in TxtAnalyzer::Analyze(). Can't open StandardDefinit_ file. ID - Entity.", inEnt.getID());


			if (keywords.length() != 0) {
				while (!InFile.eof()) {
					std::wstring standdeterm;
					std::getline(InFile, testkey);
					std::getline(InFile, standdeterm);

					//Sentence Stand(standdeterm), KeySent(keywords);
					//double coincidence = Stand.coincidenceTest(KeySent);
					//if (coincidence <= MAX_NOT_EQUAL) {
					//		std::wcout << L"\nANALYZE TEST. \n Проверяемая сущность - " + inEnt.getName() + L". Сущность в эталоне - " + testkey + L'\n';
					//		std::wcout << L"Не совпало слов = " << coincidence << L'\n';
					//		InFile.close();
					//		outname.append(testkey);
					//		return true;
					//}

					if (notfound != standdeterm.find(keywords)) {
						std::wcout << L"\nANALYZE TEST. \n Проверяемая сущность - " + inEnt.getName() + L". Сущность в эталоне - " + testkey + L'\n';
						InFile.close();
						outname.append(testkey);
						return true;
					}
				}
			}
			InFile.close();
		}
		std::wcout << L"\nANALYZE TEST. In name = " << inEnt.getName() << ". FAIL TO ANALYZE.\n";
		// TEST CODE END
		outname.append(inEnt.getName());
		return false;
	}

	std::wstring TxtAnalyzer::GenParam(const std::wstring & inname)
	{
		if (inname.length() == 0)
			return std::wstring(L"");

#ifdef _DEBUG
		std::wcout << L"\nTEST. TxtAnalyzer::GenParam. inname = " << inname << L"\n";
#endif // _DEBUG

		std::wstring res = L"";
		res += inname.at(0);
		res += inname.at((inname.length() / 2));
		res += inname.at(inname.length() - 1);
		res += USEFUNC::intTostr(inname.length());
#ifdef _DEBUG
		std::wcout << L"\nTEST. TxtAnalyzer::GenParam. param = " << res << L"\n";
#endif // _DEBUG
		return std::wstring(res);
	}

	std::wstring TxtAnalyzer::GenParam(const MODEL::Entity & inEnt)
	{
		return GenParam(inEnt.getName());
	}

	std::wstring TxtAnalyzer::GenParam(const MODEL::Relation & inRel)
	{
		std::wstring res = L"";
		res = GenParam(inRel.getParentEnt());
		res += L'[' + inRel.getType() + L']';
		res += GenParam(inRel.getChildEnt());
		return res;
	}

}