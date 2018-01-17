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
		char type;
		std::string parentEnt;
		std::string childEnt;
		void setStrField(const char fname, const std::string & newvalue);
	public:
		Relation();
		~Relation();
		unsigned int getID() const;
		char getType() const;
		void setType(const char newtype);
		std::string getParentEnt() const;
		std::string getChildEnt() const;
		void setParentEnt(const std::string & newparent);
		void setChildEnt(const std::string & newchild);
	};
}
#endif // !H_REL