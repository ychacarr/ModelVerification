#include "Trie.h"
#include <iostream>
#include <string>

unsigned int MTrie::TrieCounter = 0;

MTrie::MTrie()
{
	ID = ++TrieCounter;
	isTerminal = false;
#ifdef _DEBUG
	std::wcout << L"\nMTrie(). Object with ID = " << ID << L" was created.\n";
#endif // _DEBUG
}

MTrie::MTrie(const MTrie & source)
{
	ID = ++TrieCounter;
	isTerminal = source.isTerminal;
	nextV = source.nextV;
#ifdef _DEBUG
	std::wcout << L"\nMTrie(). Object with ID = " << ID << L" was copied from obj with ID = " << source.ID << L".\n";
#endif // _DEBUG
}

MTrie::MTrie(const std::wstring & initword)
{
	ID = ++TrieCounter;
	isTerminal = false;
#ifdef _DEBUG
	std::wcout << L"\nMTrie(). Object with ID = " << ID << L" was created. Init word = " << initword << L'\n';
#endif // _DEBUG
}

void MTrie::addWord(const std::wstring & word)
{
	MTrie *path = this;
	unsigned int i = 0;
	std::map<wchar_t, MTrie>::iterator it;

	while (i < word.length()) {
		it = path->nextV.find(word.at(i));
		if (it == path->nextV.end())
		{
			path->nextV.insert(std::pair<wchar_t, MTrie>(word.at(i), MTrie()));
			path = &(path->nextV.at(word.at(i)));
		}
		else
			path = &path->nextV.at(word.at(i));
		i++;
	}

	path->isTerminal = true;

#ifdef _DEBUG
	//std::wcout << L"\nMTrie::addWord(). Word: '" << word << "'. Successful insertion.\n";
#endif // _DEBUG
}

void MTrie::addWord(const std::wstring & word, const std::wstring & main_word)
{
	MTrie *path = this;
	unsigned int i = 0;
	std::map<wchar_t, MTrie>::iterator it;

	while (i < word.length()) {
		it = path->nextV.find(word.at(i));
		if (it == path->nextV.end())
		{
			path->nextV.insert(std::pair<wchar_t, MTrie>(word.at(i), MTrie()));
			path = &(path->nextV.at(word.at(i)));
		}
		else
			path = &path->nextV.at(word.at(i));
		i++;
	}

	path->isTerminal = true;
	path->main_word = main_word;
}

bool MTrie::find(const std::wstring & key) const
{
	const MTrie *path = this;
	unsigned int i = 0;
	std::map<wchar_t, MTrie>::const_iterator it;

	while (i < key.length()) {
		it = path->nextV.find(key.at(i));
		if (it == path->nextV.end())
			return false;
		else
			path = &path->nextV.at(key.at(i));
		i++;
	}
	if (path->isTerminal)
		return true;
	else return false;
}

std::wstring MTrie::find_with_mainform(const std::wstring & key) const
{
	const MTrie *path = this;
	unsigned int i = 0;
	std::map<wchar_t, MTrie>::const_iterator it;

	while (i < key.length()) {
		it = path->nextV.find(key.at(i));
		if (it == path->nextV.end())
			return false;
		else
			path = &path->nextV.at(key.at(i));
		i++;
	}
	if (path->isTerminal)
		return std::wstring(path->main_word);
	else return std::wstring();
}

MTrie& MTrie::operator= (const MTrie & rhs)
{
	if (*this == rhs)
		return *this;

	isTerminal = rhs.isTerminal;
	nextV.clear();
	nextV = rhs.nextV;

	return *this;
}

MTrie::~MTrie()
{
	if (nextV.size() != 0)
		nextV.clear();
#ifdef _DEBUG
	std::wcout << L"\nMTrie(). Object with ID = " << ID << L" was destroyed.\n";
#endif // _DEBUG
}

bool operator== (const MTrie & lhs, const MTrie & rhs)
{
	if (lhs.ID == rhs.ID)
		return true;
	else
		if (lhs.isTerminal == rhs.isTerminal && lhs.nextV == rhs.nextV)
			return true;
	return false;
}