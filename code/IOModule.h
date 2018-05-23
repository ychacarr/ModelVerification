#pragma once
#ifndef H_IOMODULE
#define H_IOMODULE

#include "Entity.h"
#include "Relation.h"
#include <vector>
#include "json.hpp"
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
		std::wstring ReadFileName;
		int FilePos;
		std::wstreampos trueFilePos;
		int RelCount;
		void opentest() const;
		void rawread(std::wstring &rstr, wchar_t stop = '\n');
		void read(const wchar_t datatype, std::wstring &rdstring);
		using json = nlohmann::json;
		json j;
	public:
		IO();
		IO(const std::wstring &fname);
		~IO();
		std::wstring getFileName() const;
		void setFileName(const std::wstring &fname);
		int getFilePos() const;
		unsigned int getID() const;
		std::wstring getJSONEntFromID(unsigned int &EntID);
		bool checkEOF() const;
		void resetFilePos();
		void readEntity(MODEL::Entity &inEnt);
		void readEntName(std::wstring &rdname);
		void readEntDeterm(std::wstring &rddeterm);
		void readEntCount(int &rdcount);
		void readRelation(MODEL::Relation &inRel);
		void readRelType(wchar_t &rdtype);
		void readRelParentEnt(std::wstring &rdparent);
		void readRelChildEnt(std::wstring &rdchild);
		void readRelCount(int & rdcount);
		void readParamStr(std::vector<std::wstring> &paramstr);
		void writeParamString(const std::wstring &FileName, const std::vector<std::wstring> &paramstr) const;
		void writeLine(const std::wstring &FileName, const std::wstring &strline);
		bool freset(const std::wstring &FileName) const;
		std::wstring fnd_main_form(const std::wstring & inKey);
	};
}
namespace OUTPUT {

}

#endif // !H_IOMODULE