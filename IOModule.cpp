#include "stdafx.h"
#include "IOModule.h"
#include "Errors.h"
#include "UsefulFunctions.h"
#include <fstream>
#include <vector>

namespace INPUT {

	unsigned int IO::Counter = 0;

	//
	//
	// ÍÀÏÈÑÀÒÜ ÌÅÒÎÄ ÄËß ÍÎĞÌÀËÜÍÎÃÎ Ñ×ÈÒÛÂÀÍÈß È ÔÎĞÌÈĞÎÂÀÍÈß string èç ÔÀÉËÀ. ×òî òî âğîäå rawread(std::string rstr)
	//
	//

	IO::IO() 
	{
		ID = ++Counter;
		ReadFileName = "";
		FilePos = 0;
		#ifdef _DEBUG
			std::cout << "\nIO(). Object with ID = " << ID << " was created.\n";
		#endif // _DEBUG
	}

	IO::IO(const std::string & fname)
	{
		ID = ++Counter;
		ReadFileName = fname;
		FilePos = 0;
		#ifdef _DEBUG
			std::cout << "\nIO(). Object with ID = " << ID << " was created.\n";
		#endif // _DEBUG		
	}

	IO::~IO() 
	{
		ReadFileName.clear();
		#ifdef _DEBUG
			std::cout << "\n~IO(). Object with ID = " << ID << " was destroyed.\n";
		#endif // _DEBUG
	}

	std::string IO::getFileName() const 
	{
		return std::string(ReadFileName);
	}

	void IO::setFileName(const std::string & fname)
	{
		if (FilePos != 0) {
			ReadFileName.clear();
			FilePos = 0;
		}
		ReadFileName.append(fname);
	}

	int IO::getFilePos() const 
	{
		return FilePos;
	}

	unsigned int IO::getID() const 
	{
		return ID;
	}

	bool IO::checkEOF() const 
	{
		if (FilePos == -1)
			return true;
		return false;
	}

	void IO::resetFilePos()
	{
		FilePos = 0;
	}

	void IO::opentest() const
	{
		if (ReadFileName.length() == 0)
			ERROR::throwError("Error in IO. ReadFileName is empty.", ID);
		if (checkEOF())
			ERROR::throwError("Error in IO. Unexpected end of file.", ID);
		std::ifstream InFile;
		InFile.open(ReadFileName);
		if (!InFile.is_open())
			ERROR::throwError("Error in IO. Can't open the file.", ID);
		InFile.close();
	}

	void IO::rawread(std::string & rstr, char stop)
	{
		opentest();
		std::ifstream InFile;
		char symb;
		InFile.open(ReadFileName);

		if (FilePos != 0)
			InFile.seekg(FilePos, std::ios::beg);

		rstr = "";
		InFile.get(symb);

		while (symb != stop && !InFile.eof()) {
			rstr += symb;
			InFile.get(symb);
		}
		if (InFile.peek() != EOF)
			FilePos = (int)InFile.tellg();
		else
			FilePos = -1;
		InFile.close();
	}

	void IO::read(const char datatype, std::string & rdstring)
	{
		opentest();
		std::ifstream InFile;
		InFile.open(ReadFileName);

		char symb;
		if (FilePos != 0)
			InFile.seekg(FilePos, std::ios::beg);

		if (InFile.peek() == '_')
			InFile.get(symb);
		if (InFile.peek() == datatype)
		{
			InFile.seekg(FilePos + 4, std::ios::beg);
			rdstring.clear();
			InFile.get(symb);
			while (symb != '\n') {
				rdstring += symb;
				InFile.get(symb);
			}
			#ifdef _DEBUG
				std::cout << "\nTEST. IO::read(). rdstring = " << rdstring << "\n";
			#endif // _DEBUG
		}
		else
			ERROR::throwError("Error in IO::read(). Incorrect file structure.", ID);
		if (InFile.peek() != EOF)
			FilePos = (int)InFile.tellg();
		else
			FilePos = -1;
		InFile.close();
	}

	void IO::readEntName(std::string & rdname)
	{
		read('N', rdname);
	}

	void IO::readEntDeterm(std::string & rddeterm)
	{
		read('D', rddeterm);
	}

	void IO::readEntity(MODEL::Entity & inEnt)
	{
		std::string readstr;
		readEntName(readstr);
		inEnt.setName(readstr);
		readEntDeterm(readstr);
		inEnt.setDeterm(readstr);
	}

	void IO::readRelType(char & rdtype)
	{
		std::string wstr;
		read('T', wstr);
		if (wstr.length() != 0)
			rdtype = wstr.at(0);
	}

	void IO::readRelParentEnt(std::string & rdparent)
	{
		read('P', rdparent);
	}

	void IO::readRelChildEnt(std::string & rdchild)
	{
		read('C', rdchild);
	}

	void IO::readRelation(MODEL::Relation & inRel)
	{
		std::string readstr;
		char readchar;
		readRelType(readchar);
		inRel.setType(readchar);
		readRelParentEnt(readstr);
		inRel.setParentEnt(readstr);
		readRelChildEnt(readstr);
		inRel.setChildEnt(readstr);
	}

	void IO::readEntCount(int & rdcount)
	{
		opentest();
		std::ifstream InFile;
		InFile.open(ReadFileName);
		std::string readstr;
		char symb;
		InFile.get(symb);
		while (symb != '\n') {
			readstr += symb;
			InFile.get(symb);
		}
		rdcount = USEFUNC::strToint(readstr);
		FilePos = (int)InFile.tellg();
		InFile.close();
	}

	void IO::readRelCount(int & rdcount)
	{
		opentest();
		std::ifstream InFile;
		InFile.open(ReadFileName);
		std::string readstr;
		char symb;

		if (FilePos != 0)
			InFile.seekg(FilePos, std::ios::beg);

		InFile.get(symb);
		while (symb != '\n') {
			readstr += symb;
			InFile.get(symb);
		}
		rdcount = USEFUNC::strToint(readstr);
		
		if (InFile.peek() != EOF)
			FilePos = (int)InFile.tellg();
		else
			FilePos = -1;

		InFile.close();
	}

	void IO::readParamStr(std::vector<std::string> & paramstr)
	{
		std::string readstring;
		std::string readcount = "";
		rawread(readcount);
		for (int i = 0; i < USEFUNC::strToint(readcount); i++) {
			rawread(readstring);
			paramstr.push_back(readstring);
			#ifdef _DEBUG
				std::cout << "\nTEST. IO::readParamStr(). rdstring = " << readstring << "\n";
			#endif // _DEBUG
		}
	}

	void IO::writeParamString(const std::string & FileName, const std::vector<std::string> & paramstr) const
	{
		if (FileName.length() == 0)
			ERROR::throwError("Error in IO::writeParamString(). Incorrect output file name.", ID);
		if (paramstr.size() == 0)
			ERROR::throwError("Error in IO::writeParamString(). paramstring is empty.", ID);

		std::ofstream OutFile;
		OutFile.open(FileName, std::ios_base::out | std::ios_base::trunc);
		if (!OutFile.is_open())
			ERROR::throwError("Error in IO::writeParamString(). Can't open the file.", ID);

		OutFile << paramstr.size() << '\n';
		for (unsigned int i = 0; i < paramstr.size(); i++)
			OutFile << paramstr.at(i) << '\n';

		OutFile.close();
	}

	void IO::writeLine(const std::string & FileName, const std::string & strline)
	{
		if (FileName.length() == 0)
			ERROR::throwError("Error in IO::writeParamString(). Incorrect output file name.", ID);
		if (strline.size() == 0)
			ERROR::throwError("Error in IO::writeParamString(). paramstring is empty.", ID);

		std::ofstream OutFile;
		OutFile.open(FileName, std::ios_base::out | std::ios_base::trunc);
		if (!OutFile.is_open())
			ERROR::throwError("Error in IO::writeParamString(). Can't open the file.", ID);

		OutFile << strline << '\n';

		OutFile.close();
	}
}