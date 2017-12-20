#pragma once
#ifndef H_IOMODULE
#define H_IOMODULE

#include "Entity.h"
#include "Relation.h"
#include <vector>

	/*
	TO DO:
		1. Модуль вывода информации. Либо изменить текущий модуль INPUT для возможности ввода и вывода информации
	*/

namespace INPUT {
	class IO
	{
	private:
		unsigned int ID;
		static unsigned int Counter;
		std::string ReadFileName;
		int FilePos;
		void opentest() const;
		void rawread(std::string &rstr, char stop = '\n');
		void read(const char datatype, std::string &rdstring);
	public:
		IO();
		IO(const std::string &fname);
		~IO();
		std::string getFileName() const;
		void setFileName(const std::string &fname);
		int getFilePos() const;
		unsigned int getID() const;
		bool checkEOF() const;
		void resetFilePos();
		void readEntity(MODEL::Entity &inEnt);
		void readEntName(std::string &rdname);
		void readEntDeterm(std::string &rddeterm);
		void readEntCount(int &rdcount);
		void readRelation(MODEL::Relation &inRel);
		void readRelType(char &rdtype);
		void readRelParentEnt(std::string &rdparent);
		void readRelChildEnt(std::string &rdchild);
		void readRelCount(int & rdcount);
		void readParamStr(std::vector<std::string> &paramstr);
		void writeParamString(const std::string &FileName, const std::vector<std::string> &paramstr) const;
		void writeLine(const std::string &FileName, const std::string &strline);
		void freset(const std::string &FileName) const;
	};
}
namespace OUTPUT {

}

#endif // !H_IOMODULE