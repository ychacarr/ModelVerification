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
		std::wstring verifName;
		std::wstring standName;
		std::wstring param;
		void setField(const wchar_t fname, const std::wstring & newvalue);
	public:
		CorrespLine();
		CorrespLine(const std::wstring & verifname, const std::wstring & standardname, const std::wstring & paramval);
		~CorrespLine();
		unsigned int getID() const;
		std::wstring getVerifName() const;
		void setVerifName(const std::wstring & nverifname);
		std::wstring getStandardName() const;
		void setStandName(const std::wstring & nstandname);
		std::wstring getParam() const;
		void setParam(const std::wstring & nparamval);
		void setAll(const std::wstring & nvername, const std::wstring & nstandname, const std::wstring & nparam);
		CorrespLine & operator= (const CorrespLine & rhs);
		friend std::wstring findStndNameTo(const std::wstring & veriname, const std::vector<CorrespLine> & intable);
		friend std::wstring findVerNameTo(const std::wstring & standname, const std::vector<CorrespLine> & intable);
		friend std::wstring findNameFromParam(const std::wstring & inparam, const std::vector<CorrespLine> & intable);
	};


}

#endif // !H_CORRESPTABLE