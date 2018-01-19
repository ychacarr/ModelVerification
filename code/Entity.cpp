#include "stdafx.h"
#include "Entity.h"
#include "Errors.h"

namespace MODEL {

	unsigned int Entity::Counter = 0;

	Entity::Entity()
	{
		ID = ++Counter;
		Name = L"";
		Determ = L"";
		#ifdef _DEBUG
			std::wcout << L"\nEntity(). Object with ID = " << ID << L" was created.\n";
		#endif // _DEBUG
	}

	Entity::~Entity()
	{
		Name.clear();
		Determ.clear();
		#ifdef _DEBUG
			std::wcout << L"~Entity(). Object with ID = " << ID << L" was destroyed.\n";
		#endif // _DEBUG
	}

	void Entity::setField(const wchar_t fname, const std::wstring &newvalue)
	{
		if (newvalue.length() == 0)
			ERROR::throwError(L"Error in Entity::setField(). New field value can't be empty.", ID);
		switch (fname)
		{
			case L'N':
				if (Name.length() != 0)
					Name.clear();
				Name.append(newvalue);
			break;
			case L'D':
				if (Determ.length() != 0)
					Determ.clear();
				Determ.append(newvalue);
			break;
			default:
				ERROR::throwError(L"Error in Entity::setField(). Incorrect filed name.", ID);
			break;
		}
	}

	std::wstring Entity::getName() const
	{
		return std::wstring(Name);
	}

	void Entity::setName(const std::wstring &newname)
	{
		setField(L'N', newname);
	}

	std::wstring Entity::getDeterm() const
	{
		return std::wstring(Determ);
	}

	void Entity::setDeterm(const std::wstring &newdeterm)
	{
		setField(L'D', newdeterm);
	}

	unsigned int Entity::getID() const
	{
		return ID;
	}
}
