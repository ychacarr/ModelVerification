#pragma once
#ifndef H_CORRESPTABLE
#define H_CORRESPTABLE
#include <vector>

namespace VCORE
{
	//class CorrespLine;

	//class CorrespTable
	//{
	//private:
	//	unsigned int ID;
	//	static unsigned int Counter;
	//	std::vector<CorrespLine> linearr;
	//public:
	//	CorrespTable();
	//	CorrespLine operator[] (unsigned int index) const;
	//	CorrespLine& operator[] (unsigned int index);
	//	unsigned int getID() const;
	//	~CorrespTable();
	//};

	class CorrespLine
	{
	private:
		unsigned int ID;
		static unsigned int Counter;
		std::string verifName;
		std::string standName;
		std::string param;
		void setField(const char fname, const std::string & newvalue);
	public:
		CorrespLine();
		CorrespLine(const std::string & verifname, const std::string & standardname, const std::string & paramval);
		~CorrespLine();
		unsigned int getID() const;
		std::string getVerifName() const;
		void setVerifName(const std::string & nverifname);
		std::string getStandardName() const;
		void setStandName(const std::string & nstandname);
		std::string getParam() const;
		void setParam(const std::string & nparamval);
		void setAll(const std::string & nvername, const std::string & nstandname, const std::string & nparam);
		CorrespLine & operator= (const CorrespLine & rhs);
		friend std::string findStndNameTo(const std::string & veriname, const std::vector<CorrespLine> & intable);
		friend std::string findVerNameTo(const std::string & standname, const std::vector<CorrespLine> & intable);
	};


}

#endif // !H_CORRESPTABLE