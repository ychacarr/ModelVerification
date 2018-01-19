#include "stdafx.h"
#include "Errors.h"
#include "Relation.h"

namespace MODEL
{
	unsigned int Relation::Counter = 0;

	Relation::Relation()
	{
		ID = ++Counter;
		type = 0;
		parentEnt = L"";
		childEnt = L"";
		#ifdef _DEBUG
			std::wcout << L"\nRelation(). Object with ID = " << ID << L" was created.\n";
		#endif // _DEBUG
	}

	Relation::~Relation()
	{
		parentEnt.clear();
		childEnt.clear();
		#ifdef _DEBUG
			std::wcout << L"~Relation(). Object with ID = " << ID << L" was destroyed.\n";
		#endif // _DEBUG
	}

	void Relation::setStrField(const wchar_t fname, const std::wstring &newvalue)
	{
		if (newvalue.length() == 0)
			ERROR::throwError(L"Error in Relation::setField(). New field value can't be empty.", ID);
		switch (fname)
		{
			case L'P':
				if (parentEnt.length() != 0)
					parentEnt.clear();
				parentEnt.append(newvalue);
			break;
			case L'C':
				if (childEnt.length() != 0)
					childEnt.clear();
				childEnt.append(newvalue);
			break;
			default:
				ERROR::throwError(L"Error in Relation::setStrField(). Incorrect field name.", ID);
			break;
		}
	}

	unsigned int Relation::getID() const
	{
		return ID;
	}

	wchar_t Relation::getType() const
	{
		return type;
	}

	void Relation::setType(const wchar_t newtype)
	{
		if (newtype != IDENT && newtype != NONIDENT && newtype != MANYTOMANY)
			ERROR::throwError(L"Error in Relation::setType(). Unknown type.", ID);
		type = newtype;
	}

	std::wstring Relation::getParentEnt() const
	{
		return std::wstring(parentEnt);
	}

	std::wstring Relation::getChildEnt() const
	{
		return std::wstring(childEnt);
	}

	void Relation::setParentEnt(const std::wstring &newparent)
	{
		setStrField(L'P', newparent);
	}

	void Relation::setChildEnt(const std::wstring &newchild)
	{
		setStrField(L'C', newchild);
	}

}