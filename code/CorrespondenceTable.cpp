#include "stdafx.h"
#include "СorrespondenceTable.h"
#include "Errors.h"

namespace VCORE
{
	unsigned int CorrespLine::Counter = 0;

	CorrespLine::CorrespLine()
	{
		ID = ++Counter;
		verifName = L"";
		standName = L"";
		param = L"";
		#ifdef _DEBUG
			std::wcout << L"\nCorrespLine(). Object with ID = " << ID << L" was created.\n";
		#endif // _DEBUG
	}

	CorrespLine::CorrespLine(const std::wstring & verifname, const std::wstring & standardname, const std::wstring & paramval)
	{
		ID = ++Counter;
		if (verifname.length() * standardname.length() * paramval.length() == 0)
			ERROR::throwError(L"Error in CorrespLine::CorrespLine(). Input values can't be empty.", ID);
		verifName.append(verifname);
		standName.append(standardname);
		param.append(paramval);
		#ifdef _DEBUG
			std::wcout << L"\nCorrespLine(). Object with ID = " << ID << L" was created.\n";
		#endif // _DEBUG
	}

	CorrespLine::~CorrespLine()
	{
		verifName.clear();
		standName.clear();
		param.clear();
		#ifdef _DEBUG
			std::wcout << L"~CorrespLine. Object with ID = " << ID << L" was destroyed.\n";
		#endif // _DEBUG
	}

	void CorrespLine::setField(const wchar_t fname, const std::wstring & newvalue)
	{
		if (newvalue.length() == 0)
			ERROR::throwError(L"Error in CorrespLine::setField(). Set value can't be empty.", ID);
		switch (fname)
		{
			case L'V':
				if (verifName.length() != 0)
					verifName.clear();
				verifName.append(newvalue);
			break;
			case L'S':
				if (standName.length() != 0)
					standName.clear();
				standName.append(newvalue);
			break;
			case L'P':
				if (param.length() != 0)
					param.clear();
				param.append(newvalue);
			break;
			default:
				ERROR::throwError(L"Error in CorrespLine::setField(). Unknown field name.", ID);
			break;
		}
	}

	unsigned int CorrespLine::getID() const
	{
		return ID;
	}

	std::wstring CorrespLine::getVerifName() const
	{
		return std::wstring(verifName);
	}

	void CorrespLine::setVerifName(const std::wstring & nverifname)
	{
		setField(L'V', nverifname);
	}

	std::wstring CorrespLine::getStandardName() const
	{
		return std::wstring(standName);
	}

	void CorrespLine::setStandName(const std::wstring & nstandname)
	{
		setField(L'S', nstandname);
	}

	std::wstring CorrespLine::getParam() const
	{
		return std::wstring(param);
	}

	void CorrespLine::setParam(const std::wstring & nparamval)
	{
		setField(L'P', nparamval);
	}

	void CorrespLine::setAll(const std::wstring & nvername, const std::wstring & nstandname, const std::wstring & nparam)
	{
		setVerifName(nvername);
		setStandName(nstandname);
		setParam(nparam);
	}

	CorrespLine & CorrespLine::operator=(const CorrespLine & rhs)
	{
		setField(L'V', rhs.verifName);
		setField(L'S', rhs.standName);
		setField(L'P', rhs.param);
		return *this;
	}

	std::wstring VCORE::findStndNameTo(const std::wstring & veriname, const std::vector<CorrespLine> & intable)
	{
		for (unsigned int i = 0; i < intable.size(); i++) {
			if (veriname == intable.at(i).getVerifName()) 
				return std::wstring(intable.at(i).getStandardName());
		}
		return std::wstring();
	}

	std::wstring VCORE::findVerNameTo(const std::wstring & standname, const std::vector<CorrespLine> & intable)
	{
		for (unsigned int i = 0; i < intable.size(); i++) {
			if (standname == intable.at(i).getStandardName())
				return std::wstring(intable.at(i).getVerifName());
		}
		return std::wstring();
	}

	std::wstring findNameFromParam(const std::wstring & inparam, const std::vector<CorrespLine> & intable)
	{
		if (inparam.find('!') != std::wstring::npos) {
			unsigned int icycl = 0;
			std::wstring result = L"Связь между ";

			while (icycl < inparam.length()) {
				std::wstring paramPiece = L"";

				while (icycl < inparam.length() && inparam.at(icycl) != '!')
				{
					paramPiece += inparam.at(icycl);
					icycl++;
				}
				// что бы обойти ! переходим к след. символу в строке
				icycl++;

				result += findNameFromParam(paramPiece, intable);
				if (icycl < inparam.length())
					result += L" и ";
			}
			return result + L".";
		}
		else {
			for (unsigned int i = 0; i < intable.size(); i++)
				if (inparam == intable.at(i).getParam())
					return std::wstring(intable.at(i).getStandardName());
		}
		return std::wstring(inparam);
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