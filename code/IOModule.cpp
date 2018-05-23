#include "stdafx.h"
#include "IOModule.h"
#include "Errors.h"
#include "UsefulFunctions.h"
#include <fstream>
#include <vector>

namespace INPUT {

	unsigned int IO::Counter = 0;

	//
	//
	// To do: 
	//		1. Убрать лишние методы. Оставить только необходимые
	//		2. Проверять файлы на наличие BOM в UTF-8. (отсутствие проверки приводит к тому, что при считывании появляются "пустые" символы)
	//

	IO::IO()
	{
		ID = ++Counter;
		ReadFileName = L"";
		FilePos = 0;
		RelCount = 0;
#ifdef _DEBUG
		std::wcout << L"\nIO(). Object with ID = " << ID << L" was created.\n";
#endif // _DEBUG
	}

	IO::IO(const std::wstring & fname)
	{
		ID = ++Counter;
		ReadFileName = fname;
		FilePos = 0;
		RelCount = 0;
#ifdef _DEBUG
		std::wcout << L"\nIO(). Object with ID = " << ID << L" was created.\n";
#endif // _DEBUG		
	}

	IO::~IO()
	{
		ReadFileName.clear();
#ifdef _DEBUG
		std::wcout << L"\n~IO(). Object with ID = " << ID << L" was destroyed.\n";
#endif // _DEBUG
	}

	std::wstring IO::getFileName() const
	{
		return std::wstring(ReadFileName);
	}

	void IO::setFileName(const std::wstring & fname)
	{
		if (FilePos != 0) {
			ReadFileName.clear();
			FilePos = 0;
			trueFilePos = NULL;
		}
		ReadFileName.append(fname);
		std::wifstream FileIn(ReadFileName);
		FileIn.imbue(std::locale(std::locale::empty(), new std::codecvt_utf8<wchar_t>));
		std::wstring wstrforFl;
		std::string strforJs;
		wchar_t symb;

		while (!FileIn.eof()) {
			symb = FileIn.get();
			wstrforFl += symb;
		}
		FileIn.close();
		strforJs = USEFUNC::ws2s(wstrforFl);	
		std::stringstream strstream;
		strstream << strforJs;
		strstream >> j;
		strstream.clear();
		//i >> TEST;
		//FileIn >> j;
	}

	int IO::getFilePos() const
	{
		return FilePos;
	}

	unsigned int IO::getID() const
	{
		return ID;
	}

	std::wstring IO::getJSONEntFromID(unsigned int & EntID)
	{
		std::wstring wentName;
		std::string sentName;
		sentName = j["nodes"][EntID]["entityname"].dump();
		wentName = USEFUNC::s2ws(sentName);

		while (wentName.find(L'"') != std::wstring::npos)
			wentName.erase(wentName.find(L'"'), 1);
		USEFUNC::editalltolower(wentName);
		return std::wstring(wentName);
	}

	bool IO::checkEOF() const
	{
		if (FilePos == -1)
			return true;
		return false;
	}

	void IO::resetFilePos()
	{
		FilePos = 0;
		trueFilePos = NULL;
	}

	void IO::opentest() const
	{
		if (ReadFileName.length() == 0)
			ERROR::throwError(L"Error in IO. ReadFileName is empty.", ID);
		if (checkEOF())
			ERROR::throwError(L"Error in IO. Unexpected end of file.", ID);
		std::wifstream InFile;
		InFile.open(ReadFileName);
		if (!InFile.is_open())
			ERROR::throwError(L"Error in IO. Can't open the file.", ID);
		InFile.close();
	}

	void IO::rawread(std::wstring & rstr, wchar_t stop)
	{
		opentest();
		std::wifstream InFile;
		wchar_t symb;
		InFile.open(ReadFileName);
		InFile.imbue(std::locale(std::locale::empty(), new std::codecvt_utf8<wchar_t>));
		if (FilePos != 0)
			InFile.seekg(FilePos, std::wios::beg);

		rstr = L"";
		InFile.get(symb);

		while (symb != stop && !InFile.eof()) {
			symb = towlower(symb);
			rstr += symb;
			InFile.get(symb);
		}
		FilePos = (int)InFile.tellg();
		if (InFile.peek() == EOF)
			FilePos = -1;
		InFile.close();
	}

	void IO::read(const wchar_t datatype, std::wstring & rdstring)
	{
		opentest();
		std::wifstream InFile;
		InFile.open(ReadFileName);
		InFile.imbue(std::locale(std::locale::empty(), new std::codecvt_utf8<wchar_t>));
		wchar_t symb;
		if (FilePos != 0)
			InFile.seekg(FilePos, std::ios::beg);

		if (InFile.peek() == L'_')
			InFile.get(symb);
		if (InFile.peek() == datatype)
		{
			InFile.seekg(FilePos + 4, std::ios::beg);
			rdstring.clear();
			InFile.get(symb);
			while (symb != L'\n') {
				towlower(symb);
				rdstring += symb;
				InFile.get(symb);
			}
#ifdef _DEBUG
			std::wcout << L"\nTEST. IO::read(). rdstring = " << rdstring << L"\n";
#endif // _DEBUG
		}
		else
			ERROR::throwError(L"Error in IO::read(). Incorrect file structure.", ID);
		if (InFile.peek() != EOF)
			FilePos = (int)InFile.tellg();
		else
			FilePos = -1;
		InFile.close();
	}

	void IO::readEntName(std::wstring & rdname)
	{
		read(L'N', rdname);
	}

	void IO::readEntDeterm(std::wstring & rddeterm)
	{
		read(L'D', rddeterm);
	}

	void IO::readEntity(MODEL::Entity & inEnt)
	{
		std::string strread;
		std::wstring wstr = L"";

		strread = j["nodes"][FilePos]["entityname"].dump();
		
		wstr = USEFUNC::s2ws(strread);
		while (wstr.find(L'"') != std::wstring::npos)
			wstr.erase(wstr.find(L'"'), 1);

		USEFUNC::editalltolower(wstr);
#ifdef _DEBUG
		std::wcout << L"\nTEST. IO::readEntity(). EtnName = " << wstr << L"\n";
#endif // _DEBUG
		inEnt.setName(wstr);

		strread = j["nodes"][FilePos]["description"].dump();
		wstr = USEFUNC::s2ws(strread);
		while (wstr.find(L'"') != std::wstring::npos)
			wstr.erase(wstr.find(L'"'), 1);
		USEFUNC::editalltolower(wstr);

#ifdef _DEBUG
		std::wcout << L"\nTEST. IO::readEntity(). EtnDet = " << wstr << L"\n";
#endif // _DEBUG

		inEnt.setDeterm(wstr);
		FilePos++;
	}

	void IO::readRelType(wchar_t & rdtype)
	{
		//rdtype = j["connects"][RelCount]["type"].dump();
		/*std::string wstr;
		read(L'T', wstr);
		if (wstr.length() != 0)
			rdtype = wstr.at(0);*/
		rdtype = L'i';
	}

	void IO::readRelParentEnt(std::wstring & rdparent)
	{
		std::wstring s, s2 = L"";
		std::string strName;
		
		strName = j["connects"][RelCount]["source"].dump();
		s = USEFUNC::s2ws(strName);

		s2 += s[7];
		unsigned int variable = USEFUNC::strTouint(s2);
		rdparent = getJSONEntFromID(variable);
	}

	void IO::readRelChildEnt(std::wstring & rdchild)
	{
		std::wstring s, s2 = L"";
		std::string strName;

		strName = j["connects"][RelCount]["target"].dump();
		s = USEFUNC::s2ws(strName);

		s2 += s[7];
		unsigned int variable = USEFUNC::strTouint(s2);
		rdchild = getJSONEntFromID(variable);
		RelCount++;
	}

	void IO::readRelation(MODEL::Relation & inRel)
	{
		std::wstring readstr;
		//char readchar;
		//readRelType(readchar);
		inRel.setType(L'i');
		readRelParentEnt(readstr);
		inRel.setParentEnt(readstr);
		readRelChildEnt(readstr);
		inRel.setChildEnt(readstr);
	}

	void IO::readEntCount(int & rdcount)
	{
		rdcount = j["nodes"].size();
	}

	void IO::readRelCount(int & rdcount)
	{
		rdcount = j["connects"].size();
		/*opentest();
		std::wifstream InFile;
		InFile.open(ReadFileName);
		std::string readstr;
		wchar_t symb;

		if (FilePos != 0)
			InFile.seekg(FilePos, std::ios::beg);

		InFile.get(symb);
		while (symb != '\n') {
			readstr += symb;
			InFile.get(symb);
		}
		rdcount = USEFUNC::strToint(readstr);

		if (InFile.peek() != EOF)
			FilePos = (int)InFile.tellg();
		else
			FilePos = -1;

		InFile.close();*/
	}

	void IO::readParamStr(std::vector<std::wstring> & paramstr)
	{
		std::wstring readstring;
		std::wstring wreadcount = L"";
		rawread(wreadcount);

		int readcount = USEFUNC::strToint(wreadcount);

		for (int i = 0; i < readcount; i++) {
			rawread(readstring);
			paramstr.push_back(readstring);
#ifdef _DEBUG
			std::wcout << L"\nTEST. IO::readParamStr(). rdstring = " << readstring << L"\n";
#endif // _DEBUG
		}
	}

	void IO::writeParamString(const std::wstring & FileName, const std::vector<std::wstring> & paramstr) const
	{
		if (FileName.length() == 0)
			ERROR::throwError(L"Error in IO::writeParamString(). Incorrect output file name.", ID);
		if (paramstr.size() == 0)
			ERROR::throwError(L"Error in IO::writeParamString(). paramstring is empty.", ID);

		std::wofstream OutFile;
		OutFile.open(FileName, std::ios_base::out | std::ios_base::trunc);
		OutFile.imbue(std::locale(std::locale::empty(), new std::codecvt_utf8<wchar_t>));
		if (!OutFile.is_open())
			ERROR::throwError(L"Error in IO::writeParamString(). Can't open the file.", ID);

		OutFile << paramstr.size() << L'\n';
		for (unsigned int i = 0; i < paramstr.size(); i++)
			OutFile << paramstr.at(i) << L'\n';

		OutFile.close();
	}

	void IO::writeLine(const std::wstring & FileName, const std::wstring & strline)
	{
		if (FileName.length() == 0)
			ERROR::throwError(L"Error in IO::writeParamString(). Incorrect output file name.", ID);
		if (strline.size() == 0)
			ERROR::throwError(L"Error in IO::writeParamString(). paramstring is empty.", ID);

		std::wofstream OutFile;
		OutFile.open(FileName, std::ios_base::out | std::ios_base::app);
		OutFile.imbue(std::locale(std::locale::empty(), new std::codecvt_utf8<wchar_t>));
		if (!OutFile.is_open())
			ERROR::throwError(L"Error in IO::writeParamString(). Can't open the file.", ID);

		OutFile << strline << L'\n';

		OutFile.close();
	}

	bool IO::freset(const std::wstring &FileName) const
	{
		std::wofstream File;
		File.open(FileName, std::ios_base::out | std::ios_base::trunc);
		if (!File.is_open())
			return false;
		File.close();
		return true;
	}

	std::wstring IO::fnd_main_form(const std::wstring & inKey)
	{
		wchar_t symb;
		bool endFlag = false;
		std::wstring rd_string;

		std::wifstream inFile(ReadFileName, std::ios_base::in);
		inFile.imbue(std::locale(std::locale::empty(), new std::codecvt_utf8<wchar_t>));
		while (!inFile.eof() && !endFlag) {
			symb = inFile.get();
			if (symb != inKey.at(0)) {
				while (symb != L'\n' && !inFile.eof())
					symb = inFile.get();
			}
			else {
				rd_string.clear();
				rd_string += towlower(symb);
				while (symb != L'\n' && !inFile.eof()) {
					symb = inFile.get();
					rd_string += towlower(symb);
				}
				if (std::wstring::npos != rd_string.find(inKey))
					endFlag = true;
			}
		}
		if (endFlag && rd_string.length() != 0)
			rd_string.erase(rd_string.find_first_of('#', 0), rd_string.length());
		else
			rd_string.clear();
		return std::wstring(rd_string);
	}

	void IO::form_syn_trie(MTrie & inTrie) const
	{
		//	1. Взять i-ое основное слово.
		//	2. Взять массив его синонимов.
		//	3. Заносить синонимы с указанием текущего основного слова. 
		//		DONE 3.1 доделать функцию добавления основного слова в Trie
	}

}