#include "stdafx.h"
#include "Sentence.h"
#include "Errors.h"

namespace TXTANALYZE
{
	unsigned int Sentence::Counter = 0;

	Sentence::Sentence()
	{
		ID = ++Counter;
		#ifdef _DEBUG
			std::wcout << L"\nSentence(). Object with ID = " << ID << L" was created.\n";
		#endif // _DEBUG
	}

	Sentence::Sentence(const std::wstring & instr, const wchar_t rdparam)
	{
		if (instr.length() == 0)
			ERROR::throwError(L"Error in Sentence::Sentence(). Input string can't be empty.", ID);
		std::wstring lowerstr;
		for (unsigned int i = 0; i < instr.length(); i++) {
			if (instr.at(i) != L' ' && iswpunct(instr.at(i)) == 0)
				lowerstr += tolower(instr.at(i));
			else 
				if (lowerstr.length() > 0) {
					words.push_back(lowerstr);
					lowerstr.clear();
				}
		}
		#ifdef _DEBUG
			for (unsigned int i = 0; i < words.size(); i++)
				std::wcout << L"\nSentence TEST.\n WORD AT " << i << L" = " << words.at(i) << L'\n';
		#endif // _DEBUG
	}

	unsigned int Sentence::getWordcount() const
	{
		return words.size();
	}

	std::wstring Sentence::getWord(unsigned int wnum)
	{
		if (wnum > words.size())
			ERROR::throwError(L"Error in Sentence::getWord(). Incorrect input word number.", ID);
		return std::wstring(words.at(wnum));
	}
	
	void Sentence::pushword(const std::wstring & newword)
	{
		words.push_back(newword);
	}

	Sentence::~Sentence()
	{
		if (words.size() != 0)
			words.clear();
		#ifdef _DEBUG
			std::wcout << L"~Sentence(). Object with ID = " << ID << L" was destroyed.\n";
		#endif // _DEBUG
	}
}