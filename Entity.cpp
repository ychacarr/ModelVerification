#include "stdafx.h"
#include "Entity.h"
#include "Errors.h"

namespace MODEL {

	unsigned int Entity::Counter = 0;

	Entity::Entity()
	{
		ID = ++Counter;
		Name = "";
		Determ = "";
		#ifdef _DEBUG
			std::cout << "\nEntity(). Object with ID = " << ID << " was created.\n";
		#endif // _DEBUG
	}

	Entity::~Entity()
	{
		Name.clear();
		Determ.clear();
		#ifdef _DEBUG
			std::cout << "~Entity(). Object with ID = " << ID << " was destroyed.\n";
		#endif // _DEBUG
	}

	void Entity::setField(const char fname, const std::string &newvalue)
	{
		if (newvalue.length() == 0)
			ERROR::throwError("Error in Entity::setField(). New field value can't be empty.", ID);
		switch (fname)
		{
			case 'N':
				if (Name.length() != 0)
					Name.clear();
				Name.append(newvalue);
			break;
			case 'D':
				if (Determ.length() != 0)
					Determ.clear();
				Determ.append(newvalue);
			break;
			default:
				ERROR::throwError("Error in Entity::setField(). Incorrect filed name.", ID);
			break;
		}
	}

	std::string Entity::getName() const
	{
		return std::string(Name);
	}

	void Entity::setName(const std::string &newname)
	{
		setField('N', newname);
	}

	std::string Entity::getDeterm() const
	{
		return std::string(Determ);
	}

	void Entity::setDeterm(const std::string &newdeterm)
	{
		setField('D', newdeterm);
	}

	unsigned int Entity::getID() const
	{
		return ID;
	}
}
