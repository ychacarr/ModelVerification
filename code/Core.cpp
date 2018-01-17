#include "stdafx.h"
#include "Core.h"
#include "Errors.h"
#include "Relation.h"
#include "TextAnalyzer.h"
#include "UsefulFunctions.h"

namespace VCORE
{
	unsigned int VerificationCore::Counter = 0;

	VerificationCore::VerificationCore()
	{
		ID = ++Counter;
		themename = "";
		#ifdef _DEBUG
			std::cout << "\nVerificationCore(). Object with ID = " << ID << " was created.\n";
		#endif // _DEBUG
	}

	VerificationCore::~VerificationCore()
	{
		corrtable.clear();
		paramarr.clear();
		themename.clear();
		namesuffix.clear();
		#ifdef _DEBUG
			std::cout << "~VerificationCore(). Object with ID = " << ID << " was destroyed.\n";
		#endif // _DEBUG
	}

	unsigned int VerificationCore::getID() const
	{
		return ID;
	}

	void VerificationCore::setThemeName(const std::string & nthname)
	{
		if (nthname.length() == 0)
			ERROR::throwError("Error in VerificationCore::setThemeName(). New theme name can't be empty!", ID);
		if (themename.length() != 0)
			themename.clear();
		themename.append(nthname);
		if (corrtable.size() != 0)
			corrtable.clear();
		if (paramarr.size() != 0)
			paramarr.clear();
	}

	std::string VerificationCore::getThemeName() const
	{
		return std::string(themename);
	}

	void VerificationCore::setnamesuffix(const std::string & nsuffix)
	{
		if (nsuffix.length() == 0)
			ERROR::throwError("Error in VerificationCore::setnamesuffix(). New name suffix can't be empty.", ID);
		if (namesuffix.length() != 0) {
			namesuffix.clear();
		}
		namesuffix.append(nsuffix);
		if (corrtable.size() != 0)
			corrtable.clear();
		if (paramarr.size() != 0)
			paramarr.clear();
	}

	std::string VerificationCore::getnamesuffix() const
	{
		return std::string(namesuffix);
	}

	void VerificationCore::start()
	{
		MODEL::Entity ReadEnt;
		MODEL::Relation *ReadRelArr;
		int EntCount = 0, RelCount = 0;
		unsigned int iparamstr = 0;
		TXTANALYZE::TxtAnalyzer Analyzer;
		CorrespLine corrbuff;
		std::string relstandname, rdrelname;
		std::vector<std::string> ParamString, StandParamString;
		unsigned int i = 0;
		bool nextFlag, stopFlag;

		std::string verfresults = "";
		
		if (themename.length() == 0)
			ERROR::throwError("Error in VerificationCore::start(). Empty theme name. Set themename first.", ID);

		iomodule.setFileName("txt files/" + namesuffix);

		iomodule.readEntCount(EntCount);
		if (EntCount <= 0)
			ERROR::throwError("Error in VerificationCore::start(). Input entity count can't be <= 0. Check input file.", ID);

		for (int i = 0; i < EntCount; i++)
		{
			iomodule.readEntity(ReadEnt);
			//TEST CODE START. 
			//TO DO: ���������� ��� � ����� ����������� ����, ��� ������ ����� ������� ������ ������� ������
			corrbuff.setVerifName(ReadEnt.getName());
			std::string buf;
			if (!Analyzer.Analyze(ReadEnt, themename, buf))
				verfresults.append("\n�� ������� ������������ ��� ��������: " + buf);

			corrbuff.setStandName(buf);
			if (buf.length() > 0)
				corrbuff.setParam(Analyzer.GenParam(buf));
			else
				corrbuff.setParam("\0");
			
			corrtable.push_back(corrbuff);
			//TEST CODE END.
		}
		#ifdef _DEBUG
			std::cout << "\n\n";
			for (unsigned int i = 0; i < corrtable.size(); i++)
				std::cout << corrtable.at(i).getVerifName() << ' ' << corrtable.at(i).getStandardName() << ' ' << corrtable.at(i).getParam() << '\n';
		#endif // _DEBUG
		iomodule.readRelCount(RelCount);

		#ifdef _DEBUG
			std::cout << "\nRelCount = " << RelCount << '\n';
		#endif // _DEBUG
		ReadRelArr = new MODEL::Relation[RelCount];

		for (int i = 0; i < RelCount; i++) {
			char rdtype;
			iomodule.readRelType(rdtype);
			ReadRelArr[i].setType(rdtype);

			iomodule.readRelParentEnt(rdrelname);
			relstandname = findStndNameTo(rdrelname, corrtable);
			if (relstandname.length() == 0)
				ReadRelArr[i].setParentEnt(rdrelname);
			else
				ReadRelArr[i].setParentEnt(relstandname);

			iomodule.readRelChildEnt(rdrelname);
			relstandname = findStndNameTo(rdrelname, corrtable);
			if (relstandname.length() == 0)
				ReadRelArr[i].setChildEnt(rdrelname);
			else
				ReadRelArr[i].setChildEnt(relstandname);
			
			#ifdef _DEBUG
				std::cout << "\nType = " << ReadRelArr[i].getType() << "\nParent = " << ReadRelArr[i].getParentEnt() << "\nChild = " << ReadRelArr[i].getChildEnt() << '\n';
			#endif // _DEBUG
		}

		for (unsigned int i = 0; i < corrtable.size(); i++) 
			ParamString.push_back(corrtable.at(i).getParam());
		for (int i = 0; i < RelCount; i++)
			ParamString.push_back(Analyzer.GenParam(ReadRelArr[i]));

		delete[] ReadRelArr;
#ifdef _DEBUG
			std::cout << "PARAM STRING:\n";
			for (unsigned int i = 0; i < ParamString.size(); i++){
				std::cout << i << ' ' << ParamString.at(i) << '\n';
			}
#endif // _DEBUG
		
		iomodule.setFileName("txt files/StandardParams_" + themename + ".txt");
		iomodule.readParamStr(StandParamString);

		if (ParamString.size() < StandParamString.size())
			verfresults.append("\n��������������! �������� ����������� ������ �� ��������� ���������� ���������� �������.");

		while (i != ParamString.size()) {
			nextFlag = stopFlag = true;
			for (unsigned int j = 0; (j < StandParamString.size()) && stopFlag; j++) {
				if (StandParamString.at(j).compare(ParamString.at(i)) == 0) {
#ifdef _DEBUG
					std::cout << "\n�������� ����. �� ����������� ������ ������� ������ �������: \n" << ParamString.at(i) << '\n';
					nextFlag = stopFlag = false;
#endif // _DEBUG
					ParamString.erase(ParamString.begin() + i);
				}
			}
			if (nextFlag)
				i++;
		}

		if (ParamString.size() != 0)
			for (unsigned int i = 0; i < ParamString.size(); i++)
				verfresults.append("\n�� ������� ��������� � ��������: " + findNameFromParam(ParamString.at(i), corrtable));
		else {
			verfresults.clear();
			verfresults.append("\n���������� ������ ����� ���������� ���������� �������.");
		}

		if (!iomodule.freset("txt files/results/VerifResults_" + namesuffix))
			ERROR::throwError("Error in VerificationCore::start(). Can't open file to write down verifications results.", ID);
		iomodule.writeLine("txt files/results/VerifResults_" + namesuffix, verfresults);

		// ���� ���������� ������
		// DONE :	1) ����������� � ������ ���������� ������� ���������� ���-�� ������ (���� ��������� �� ��� ���, ���� �� ��������� ����� �����)
		//			2) �������� ����� ��������� � ������ ���������� �� � �������� corrtable
		//			3) �������� ���������, �����������, ��������� �����, � "��������������� ������"

		// ���� ������ � ��������
		// DONE :	1) ����������� ���������� "��������������� ������" ����������� ��������� (�� corrtable)
		//			2) ����������� ������ "��������������� ������" �������
		//			3) ����������� ������ ����������� ������ � ��������, � ������� ��������������
		// TO DO 
		//	   	 
		// �����
		// DONE	:	1) ������ �� OUTPUT ������ �������������� � ������
		//
		// TO DO
	}

	void VerificationCore::genStandParam()
	{
		MODEL::Entity ReadEnt;
		MODEL::Relation *ReadRelArr;
		int EntCount = 0, RelCount = 0;
		TXTANALYZE::TxtAnalyzer Analyzer;
		std::string rdrelname;
		std::vector<std::string> ParamString;

		if (themename.length() == 0)
			ERROR::throwError("Error in VerificationCore::genStandParam(). Empty theme name. Set themename first.", ID);

		iomodule.setFileName("txt files/" + namesuffix);

		iomodule.readEntCount(EntCount);
		if (EntCount <= 0)
			ERROR::throwError("Error in VerificationCore::genStandParam(). Input entity count can't be <= 0. Check input file.", ID);

		for (int i = 0; i < EntCount; i++)
		{
			iomodule.readEntity(ReadEnt);
			ParamString.push_back(Analyzer.GenParam(ReadEnt.getName()));
		}

		iomodule.readRelCount(RelCount);

		std::cout << "\nRelCount = " << RelCount << '\n';

		ReadRelArr = new MODEL::Relation[RelCount];

		for (int i = 0; i < RelCount; i++) {
			char rdtype;
			iomodule.readRelType(rdtype);
			ReadRelArr[i].setType(rdtype);

			iomodule.readRelParentEnt(rdrelname);
			ReadRelArr[i].setParentEnt(rdrelname);
			iomodule.readRelChildEnt(rdrelname);
			ReadRelArr[i].setChildEnt(rdrelname);

			std::cout << "\nType = " << ReadRelArr[i].getType() << "\nParent = " << ReadRelArr[i].getParentEnt() << "\nChild = " << ReadRelArr[i].getChildEnt() << '\n';
			ParamString.push_back(Analyzer.GenParam(ReadRelArr[i]));
		}
		delete[] ReadRelArr;

		/*
			TO DO:

			DONE:
				1. ������ ������ ���������� � ����
		*/

		std::cout << "PARAM STRING:\n";
		for (unsigned int i = 0; i < ParamString.size(); i++) 
			std::cout << i << ' ' << ParamString.at(i) << '\n';
		
		iomodule.writeParamString("txt files/StandardParams_" + themename + ".txt", ParamString);
	}
}