#pragma once
#ifndef H_ENTITY
#define H_ENTITY

namespace MODEL {
	class Entity
	{
	private:
		unsigned int ID;
		static unsigned int Counter;
		std::wstring Name;
		std::wstring Determ;
		void setField(const wchar_t fname, const std::wstring &newvalue);
	public:
		Entity();
		~Entity();
		std::wstring getName() const;
		void setName(const std::wstring &newname);
		std::wstring getDeterm() const;
		void setDeterm(const std::wstring &newdeterm);
		unsigned int getID() const;
	};
}
#endif // !H_ENTITY