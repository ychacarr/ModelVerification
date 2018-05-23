#pragma once
#ifndef H_SENTENCE
#define H_SENTENCE
#include <vector>

namespace TXTANALYZE
{
	class Sentence
	{
	private:
		static unsigned int Counter;
		unsigned int ID;
		std::vector<std::wstring> words;
	public:
		Sentence();
		// rdparam - тип чтения слов. а - all - все слова.  
		Sentence(const std::wstring & instr, const wchar_t rdparam = 'a');
		unsigned int getWordcount() const;
		std::wstring getWord(unsigned int wnumber);
		void pushword(const std::wstring & newword);
		void normalize();
		double coincidenceTest(const Sentence &lhs) const;
		~Sentence();
	};

	

}
#endif // !H_SENTENCE