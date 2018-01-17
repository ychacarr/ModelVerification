#pragma once
#ifndef H_ENTITY
#define H_ENTITY

namespace MODEL {
	class Entity
	{
	private:
		unsigned int ID;
		static unsigned int Counter;
		std::string Name;
		std::string Determ;
		void setField(const char fname, const std::string &newvalue);
	public:
		Entity();
		~Entity();
		std::string getName() const;
		void setName(const std::string &newname);
		std::string getDeterm() const;
		void setDeterm(const std::string &newdeterm);
		unsigned int getID() const;
	};
}
#endif // !H_ENTITY