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
			std::cout << "\nSentence(). Object with ID = " << ID << " was created.\n";
		#endif // _DEBUG
	}

	Sentence::Sentence(const std::string & instr, const char rdparam)
	{
		if (instr.length() == 0)
			ERROR::throwError("Error in Sentence::Sentence(). Input string can't be empty.", ID);
		std::string lowerstr;
		for (unsigned int i = 0; i < instr.length(); i++) {
			if (instr.at(i) != ' ' && iswpunct(instr.at(i)) == 0)
				lowerstr += tolower(instr.at(i));
			else 
				if (lowerstr.length() > 0) {
					words.push_back(lowerstr);
					lowerstr.clear();
				}
		}
		#ifdef _DEBUG
			for (unsigned int i = 0; i < words.size(); i++)
				std::cout << "\nSentence TEST.\n WORD AT " << i << " = " << words.at(i) << '\n';
		#endif // _DEBUG
	}

	unsigned int Sentence::getWordcount() const
	{
		return words.size();
	}

	std::string Sentence::getWord(unsigned int wnum)
	{
		if (wnum > words.size())
			ERROR::throwError("Error in Sentence::getWord(). Incorrect input word number.", ID);
		return std::string(words.at(wnum));
	}
	
	void Sentence::pushword(const std::string & newword)
	{
		words.push_back(newword);
	}

	Sentence::~Sentence()
	{
		if (words.size() != 0)
			words.clear();
		#ifdef _DEBUG
			std::cout << "~Sentence(). Object with ID = " << ID << " was destroyed.\n";
		#endif // _DEBUG
	}
}