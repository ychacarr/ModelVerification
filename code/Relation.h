#pragma once
#ifndef H_REL
#define H_REL

#include "RelationTypes.h"
namespace MODEL
{
	class Relation
	{
	private:
		unsigned int ID;
		static unsigned int Counter;
		wchar_t type;
		std::wstring parentEnt;
		std::wstring childEnt;
		void setStrField(const wchar_t fname, const std::wstring & newvalue);
	public:
		Relation();
		~Relation();
		unsigned int getID() const;
		wchar_t getType() const;
		void setType(const wchar_t newtype);
		std::wstring getParentEnt() const;
		std::wstring getChildEnt() const;
		void setParentEnt(const std::wstring & newparent);
		void setChildEnt(const std::wstring & newchild);
	};
}
#endif // !H_REL