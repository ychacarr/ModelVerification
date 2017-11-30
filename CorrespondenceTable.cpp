#include "stdafx.h"
#include "ÑorrespondenceTable.h"
#include "Errors.h"

namespace VCORE
{
	unsigned int CorrespLine::Counter = 0;

	CorrespLine::CorrespLine()
	{
		ID = ++Counter;
		verifName = "";
		standName = "";
		param = "";
		#ifdef _DEBUG
			std::cout << "\nCorrespLine(). Object with ID = " << ID << " was created.\n";
		#endif // _DEBUG
	}

	CorrespLine::CorrespLine(const std::string & verifname, const std::string & standardname, const std::string & paramval)
	{
		ID = ++Counter;
		if (verifname.length() * standardname.length() * paramval.length() == 0)
			ERROR::throwError("Error in CorrespLine::CorrespLine(). Input values can't be empty.", ID);
		verifName.append(verifname);
		standName.append(standardname);
		param.append(paramval);
		#ifdef _DEBUG
			std::cout << "\nCorrespLine(). Object with ID = " << ID << " was created.\n";
		#endif // _DEBUG
	}

	CorrespLine::~CorrespLine()
	{
		verifName.clear();
		standName.clear();
		param.clear();
		#ifdef _DEBUG
			std::cout << "~CorrespLine. Object with ID = " << ID << " was destroyed.\n";
		#endif // _DEBUG
	}

	void CorrespLine::setField(const char fname, const std::string & newvalue)
	{
		if (newvalue.length() == 0)
			ERROR::throwError("Error in CorrespLine::setField(). Set value can't be empty.", ID);
		switch (fname)
		{
			case 'V':
				if (verifName.length() != 0)
					verifName.clear();
				verifName.append(newvalue);
			break;
			case 'S':
				if (standName.length() != 0)
					standName.clear();
				standName.append(newvalue);
			break;
			case 'P':
				if (param.length() != 0)
					param.clear();
				param.append(newvalue);
			break;
			default:
				ERROR::throwError("Error in CorrespLine::setField(). Unknown field name.", ID);
			break;
		}
	}

	unsigned int CorrespLine::getID() const
	{
		return ID;
	}

	std::string CorrespLine::getVerifName() const
	{
		return std::string(verifName);
	}

	void CorrespLine::setVerifName(const std::string & nverifname)
	{
		setField('V', nverifname);
	}

	std::string CorrespLine::getStandardName() const
	{
		return std::string(standName);
	}

	void CorrespLine::setStandName(const std::string & nstandname)
	{
		setField('S', nstandname);
	}

	std::string CorrespLine::getParam() const
	{
		return std::string(param);
	}

	void CorrespLine::setParam(const std::string & nparamval)
	{
		setField('P', nparamval);
	}

	void CorrespLine::setAll(const std::string & nvername, const std::string & nstandname, const std::string & nparam)
	{
		setVerifName(nvername);
		setStandName(nstandname);
		setParam(nparam);
	}

	CorrespLine & CorrespLine::operator=(const CorrespLine & rhs)
	{
		setField('V', rhs.verifName);
		setField('S', rhs.standName);
		setField('P', rhs.param);
		return *this;
	}

	std::string VCORE::findStndNameTo(const std::string & veriname, const std::vector<CorrespLine> & intable)
	{
		for (unsigned int i = 0; i < intable.size(); i++) {
			if (veriname == intable.at(i).getVerifName()) 
				return std::string(intable.at(i).getStandardName());
		}
		return std::string();
	}

	std::string VCORE::findVerNameTo(const std::string & standname, const std::vector<CorrespLine> & intable)
	{
		for (unsigned int i = 0; i < intable.size(); i++) {
			if (standname == intable.at(i).getStandardName())
				return std::string(intable.at(i).getVerifName());
		}
		return std::string();
	}

	//CorrespTable::CorrespTable()
	//{
	//	ID = ++Counter;
	//	#ifdef _DEBUG
	//		std::cout << "\nCorrespLine(). Object with ID = " << ID << " was created.\n";
	//	#endif // _DEBUG
	//}

	//CorrespTable::~CorrespTable()
	//{
	//	linearr.clear();
	//	#ifdef _DEBUG
	//		std::cout << "~CorrespTable. Object with ID = " << ID << " was destroyed.\n";
	//	#endif // _DEBUG
	//}

}