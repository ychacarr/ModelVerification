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
		parentEnt = "";
		childEnt = "";
		#ifdef _DEBUG
			std::cout << "\nRelation(). Object with ID = " << ID << " was created.\n";
		#endif // _DEBUG
	}

	Relation::~Relation()
	{
		parentEnt.clear();
		childEnt.clear();
		#ifdef _DEBUG
			std::cout << "~Relation(). Object with ID = " << ID << " was destroyed.\n";
		#endif // _DEBUG
	}

	void Relation::setStrField(const char fname, const std::string &newvalue)
	{
		if (newvalue.length() == 0)
			ERROR::throwError("Error in Relation::setField(). New field value can't be empty.", ID);
		switch (fname)
		{
			case 'P':
				if (parentEnt.length() != 0)
					parentEnt.clear();
				parentEnt.append(newvalue);
			break;
			case 'C':
				if (childEnt.length() != 0)
					childEnt.clear();
				childEnt.append(newvalue);
			break;
			default:
				ERROR::throwError("Error in Relation::setStrField(). Incorrect field name.", ID);
			break;
		}
	}

	unsigned int Relation::getID() const
	{
		return ID;
	}

	char Relation::getType() const
	{
		return type;
	}

	void Relation::setType(const char newtype)
	{
		if (newtype != IDENT && newtype != NONIDENT && newtype != MANYTOMANY)
			ERROR::throwError("Error in Relation::setType(). Unknown type.", ID);
		type = newtype;
	}

	std::string Relation::getParentEnt() const
	{
		return std::string(parentEnt);
	}

	std::string Relation::getChildEnt() const
	{
		return std::string(childEnt);
	}

	void Relation::setParentEnt(const std::string &newparent)
	{
		setStrField('P', newparent);
	}

	void Relation::setChildEnt(const std::string &newchild)
	{
		setStrField('C', newchild);
	}

}