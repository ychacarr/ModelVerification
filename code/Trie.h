#pragma once
#ifndef H_TRIE
#define H_TRIE
#include <map>

class MTrie
{
private:
	unsigned int ID;
	static unsigned int TrieCounter;
	bool isTerminal;
	std::wstring main_word;
	std::map<wchar_t, MTrie> nextV;
public:
	MTrie();
	MTrie(const MTrie & source);
	MTrie(const std::wstring & initword);
	void addWord(const std::wstring & word);
	void addWord(const std::wstring & word, const std::wstring & main_word);
	bool find(const std::wstring & key) const;
	std::wstring find_with_mainform(const std::wstring & key) const;
	MTrie& operator= (const MTrie & rhs);
	~MTrie();
	friend bool operator== (const MTrie & lhs, const MTrie & rhs);
};

#endif // !H_TRIE