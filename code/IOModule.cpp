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
	// Done:	Õ¿œ»—¿“‹ Ã≈“Œƒ ƒÀﬂ ÕŒ–Ã¿À‹ÕŒ√Œ —◊»“€¬¿Õ»ﬂ » ‘Œ–Ã»–Œ¬¿Õ»ﬂ string ËÁ ‘¿…À¿. ◊ÚÓ ÚÓ ‚Ó‰Â rawread(std::string rstr)
	//
	//

	IO::IO()
	{
		ID = ++Counter;
		ReadFileName = "";
		FilePos = 0;
		RelCount = 0;
#ifdef _DEBUG
		std::cout << "\nIO(). Object with ID = " << ID << " was created.\n";
#endif // _DEBUG
	}

	IO::IO(const std::string & fname)
	{
		ID = ++Counter;
		ReadFileName = fname;
		FilePos = 0;
		RelCount = 0;
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
		std::ifstream i(ReadFileName);
		std::string TEST;
		//i >> TEST;
		i >> j;
		i.close();
	}

	int IO::getFilePos() const
	{
		return FilePos;
	}

	unsigned int IO::getID() const
	{
		return ID;
	}

	std::string IO::getJSONEntFromID(unsigned int & EntID)
	{
		std::string entName;
		entName = j["nodes"][EntID]["entityname"].dump();
		//entName.erase(entName.begin());
		//entName.erase(entName.end() - 1);
		return std::string(entName);
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
		readstr = j["nodes"][FilePos]["entityname"].dump();
		//readstr.erase(readstr.begin());
		//readstr.erase(readstr.end() - 1);
		inEnt.setName(readstr);
		readstr = j["nodes"][FilePos]["description"].dump();
		//readstr.erase(readstr.begin());
		//readstr.erase(readstr.end() - 1);
		inEnt.setDeterm(readstr);
		FilePos++;
	}

	void IO::readRelType(char & rdtype)
	{
		//rdtype = j["connects"][RelCount]["type"].dump();
		/*std::string wstr;
		read('T', wstr);
		if (wstr.length() != 0)
			rdtype = wstr.at(0);*/
		rdtype = 'i';
	}

	void IO::readRelParentEnt(std::string & rdparent)
	{
		std::string s, s2 = "";
		s = j["connects"][RelCount]["source"].dump();
		s2 += s[7];
		unsigned int variable = USEFUNC::strTouint(s2);
		rdparent = getJSONEntFromID(variable);
	}

	void IO::readRelChildEnt(std::string & rdchild)
	{
		std::string s, s2 = "";
		s = j["connects"][RelCount]["target"].dump();
		s2 += s[7];
		unsigned int variable = USEFUNC::strTouint(s2);
		rdchild = getJSONEntFromID(variable);
		RelCount++;
	}

	void IO::readRelation(MODEL::Relation & inRel)
	{
		std::string readstr;
		//char readchar;
		//readRelType(readchar);
		inRel.setType('i');
		readRelParentEnt(readstr);
		inRel.setParentEnt(readstr);
		readRelChildEnt(readstr);
		inRel.setChildEnt(readstr);
	}

	void IO::readEntCount(int & rdcount)
	{
		rdcount = j["nodes"].size();
	}

	void IO::readRelCount(int & rdcount)
	{
		rdcount = j["connects"].size();
		/*opentest();
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

		InFile.close();*/
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
		OutFile.open(FileName, std::ios_base::out | std::ios_base::app);
		if (!OutFile.is_open())
			ERROR::throwError("Error in IO::writeParamString(). Can't open the file.", ID);

		OutFile << strline << '\n';

		OutFile.close();
	}

	bool IO::freset(const std::string &FileName) const
	{
		std::ofstream File;
		File.open(FileName, std::ios_base::out | std::ios_base::trunc);
		if (!File.is_open())
			return false;
		File.close();
		return true;
	}
}