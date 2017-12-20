#include "stdafx.h"
#include "Core.h"
#include "Errors.h"
#include "Relation.h"
#include "TextAnalyzer.h"

namespace VCORE
{
	unsigned int VerificationCore::Counter = 0;

	VerificationCore::VerificationCore()
	{
		ID = ++Counter;
		paramFlName = "";
		#ifdef _DEBUG
			std::cout << "\nVerificationCore(). Object with ID = " << ID << " was created.\n";
		#endif // _DEBUG
	}

	VerificationCore::~VerificationCore()
	{
		corrtable.clear();
		paramarr.clear();
		paramFlName.clear();
		#ifdef _DEBUG
			std::cout << "~VerificationCore(). Object with ID = " << ID << " was destroyed.\n";
		#endif // _DEBUG
	}

	unsigned int VerificationCore::getID() const
	{
		return ID;
	}

	void VerificationCore::setParamFlName(const std::string & newparamFl)
	{
		if (paramFlName.length() != 0) {
			paramFlName.clear();
			paramFlName = "";
		}
		paramFlName.append(newparamFl);
	}

	std::string VerificationCore::getParamFlName() const
	{
		return std::string(paramFlName);
	}

	void VerificationCore::reload(const std::string & inFile)
	{
		if (paramFlName.length() != 0)
			paramFlName.clear();
		iomodule.setFileName(inFile);
		corrtable.clear();
		paramarr.clear();
	}

	std::string VerificationCore::getFilename() const
	{
		return std::string(iomodule.getFileName());
	}

	void VerificationCore::setFilename(const std::string & nfilename)
	{
		iomodule.setFileName(nfilename);
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

		iomodule.readEntCount(EntCount);
		if (EntCount <= 0)
			ERROR::throwError("Error in VerificationCore::start(). Input entity count can't be <= 0. Check input file.", ID);

		for (int i = 0; i < EntCount; i++)
		{
			iomodule.readEntity(ReadEnt);
			//TEST CODE START. 
			//TO DO: ���������� ��� � ����� ����������� ����, ��� ������ ����� ������� ������ ������� ������
			corrbuff.setVerifName(ReadEnt.getName());
			std::string buf = Analyzer.Analyze(ReadEnt);
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
		
		iomodule.setFileName(paramFlName);
		iomodule.readParamStr(StandParamString);

		while (i != StandParamString.size()) {
			nextFlag = stopFlag = true;
			for (unsigned int j = 0; (j < ParamString.size()) && stopFlag; j++) {
				if (StandParamString.at(i).compare(ParamString.at(j)) == 0) {
					StandParamString.erase(StandParamString.begin() + i);
#ifdef _DEBUG
					std::cout << "\n�������� ����. �� ����������� ������ ������� ������ �������: \n" << ParamString.at(j) << '\n';
					nextFlag = stopFlag = false;
#endif // _DEBUG
				}
			}
			if (nextFlag)
				i++;
		}		

		if (StandParamString.size() != 0) {
			iomodule.freset("txt files/results/TestVerificationResults.txt");
			for (unsigned int i = 0; i < StandParamString.size(); i++) {
				iomodule.writeLine("txt files/results/TestVerificationResults.txt", "�� ������������ � ����������� ������: " + findNameFromParam(StandParamString.at(i), corrtable));
			}
		}
		else
			iomodule.writeLine("txt files/results/TestVerificationResults.txt", "� ���������� ������ ������������ ��� �������� � ����� �������.");

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

	void VerificationCore::genStandParam(const std::string & outFileName)
	{
		MODEL::Entity ReadEnt;
		MODEL::Relation *ReadRelArr;
		int EntCount = 0, RelCount = 0;
		TXTANALYZE::TxtAnalyzer Analyzer;
		std::string rdrelname;
		std::vector<std::string> ParamString;

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

		iomodule.writeParamString(outFileName, ParamString);
	}
}