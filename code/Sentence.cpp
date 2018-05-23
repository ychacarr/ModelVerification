#include "stdafx.h"
#include "Sentence.h"
#include "IOModule.h"
#include "Errors.h"

namespace TXTANALYZE
{
	unsigned int Sentence::Counter = 0;
	//
	// ������� ���������� �������� ����������� ���������� �� �����
	//
	unsigned int MIN_SYMB = 3;
	std::wstring MIN_EXCEP = L"��";
	std::wstring WORD_FORM_FILE = L"word_forms.txt";
	//
	//
	//

	Sentence::Sentence()
	{
		ID = ++Counter;
		#ifdef _DEBUG
			std::wcout << L"\nSentence(). Object with ID = " << ID << L" was created.\n";
		#endif // _DEBUG
	}

	Sentence::Sentence(const std::wstring & instr, const wchar_t rdparam)
	{
		ID = ++Counter;
		//if (instr.length() == 0)
		//	ERROR::throwError(L"Error in Sentence::Sentence(). Input string can't be empty.", ID);
		if (instr.length() != 0) {
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
			if (lowerstr.length() != 0) {
				words.push_back(lowerstr);
				lowerstr.clear();
			}
			#ifdef _DEBUG
					std::wcout << L"\nSentence(). Object with ID = " << ID << L" was created.";
					for (unsigned int i = 0; i < words.size(); i++)
						std::wcout << L"\nSentence TEST.\n WORD AT " << i << L" = " << words.at(i) << L'\n';
			#endif // _DEBUG
		}
		//else
		#ifdef _DEBUG
			std::wcout << L"\nSentence(). Object with ID = " << ID << L" was created.\n";
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

	void Sentence::normalize()
	{
		if (words.size() == 0)
			ERROR::throwError(L"Error in Sentence::normalize(). Emtpy sentence.", ID);
		std::vector<std::wstring> tmp;
		tmp.clear();
		for (unsigned int i = 0; i < words.size(); i++) {
			if (words.at(i).length() > MIN_SYMB)
				tmp.push_back(words[i]);
			else {
				if (std::wstring::npos != MIN_EXCEP.find(words[i]))
					tmp.push_back(words[i]);
			}
		}
		if (tmp.size() == 0)
			ERROR::throwError(L"Error in Sentence::normalize(). Normalized sentence size = 0.", ID);
		words.clear();
		words = tmp;
	}

	double Sentence::coincidenceTest(const Sentence & lhs) const
	{
		if (words.size() * lhs.words.size() == 0) {
			if (words.size() == lhs.words.size())
				return 100.0;
			else
				return 0.0;
		}
		double res = 0.0;
		for (unsigned int i = 0; i < words.size(); i++)
			for (unsigned int j = 0; j < lhs.words.size(); j++)
				if (words.at(i) == lhs.words.at(j))
					res += 1.0;
		res = double(words.size()) - res;
		return res;
	}

	void Sentence::to_mainforms()
	{
		INPUT::IO FileFind;
		std::vector<std::wstring> new_words;
		std::wstring tmp_str;

		FileFind.setFileName(WORD_FORM_FILE);

		for (unsigned int i = 0; i < words.size(); i++) {
			tmp_str = FileFind.fnd_main_form(words.at(i));
			if (tmp_str.length() != 0)
				new_words.push_back(tmp_str);
			else {
				// ���� ���.
				// ���������� ������� ����� � ����������� ��� ���� ������� � ���, ��� ������ ����� ��� � ������� ����
				tmp_str.clear();
				std::wcout << L"\n\t!!! !!! !!! !!! SENTENCE::to_mainforms(). !!! !!! !!! !!!\n";
			}
		}
		words.clear();
		words = new_words;
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