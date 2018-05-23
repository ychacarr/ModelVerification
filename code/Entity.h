#pragma once
#ifndef H_ENTITY
#define H_ENTITY
#include <vector>

namespace MODEL {
	class Entity
	{
	private:
		unsigned int ID;
		static unsigned int Counter;
		std::wstring Name;
		std::wstring Determ;
		std::vector<std::wstring> keys;
		std::vector<std::wstring> attribs;
		void setField(const wchar_t fname, const std::wstring &newvalue);
	public:
		Entity();
		~Entity();
		std::wstring getName() const;
		void setName(const std::wstring &newname);
		std::wstring getDeterm() const;
		void setDeterm(const std::wstring &newdeterm);
		void setKeys(const std::vector<std::wstring> & in_keys);
		std::vector<std::wstring> getKeys() const;
		void setAttribs(const std::vector<std::wstring> & in_attribs);
		std::vector<std::wstring> getAttribs() const;
		unsigned int getID() const;
	};
}
#endif // !H_ENTITY