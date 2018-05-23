#include "stdafx.h"
#include "TextAnalyzer.h"
#include "Trie.h"
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

	bool TxtAnalyzer::Analyze(const INPUT::IO & FileReader, const MODEL::Entity &inEnt, std::wstring & outname)
	{
		Sentence sentenceanalyze(inEnt.getDeterm());
		MTrie syn_trie;

		sentenceanalyze.normalize();
		if (outname.length() != 0)
			outname.clear();
		//Основной код анализа
		{
			//	DONE 1. Найти "инфинитивные" формы слов определения (реализовать в Sentence)
			sentenceanalyze.to_mainforms();
			//	DONE+- 2. По дереву синонимов найти ключевые слова
			//		2.1 Точно определить структуру файла
			//		2.2 Доделать код в IO
			FileReader.form_syn_trie(syn_trie);
			//	3. По списку ключевых форм выявить подходящую сущность
		}
		// В случае неудачного анализа необходимо провести логгирование в файл
		{
			std::wcout << L"\nANALYZE TEST. In name = " << inEnt.getName() << ". FAIL TO ANALYZE.\n";
			// TEST CODE END
			outname.append(inEnt.getName());
			return false;
		}
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