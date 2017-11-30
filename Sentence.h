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
		std::vector<std::string> words;
	public:
		Sentence();
		// rdparam - тип чтения слов. а - all - все слова.  
		Sentence(const std::string & instr, const char rdparam = 'a');
		unsigned int getWordcount() const;
		std::string getWord(unsigned int wnumber);
		void pushword(const std::string & newword);
		~Sentence();
	};

	

}
#endif // !H_SENTENCE