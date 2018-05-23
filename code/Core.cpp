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
		themename = L"";
		#ifdef _DEBUG
			std::wcout << L"\nVerificationCore(). Object with ID = " << ID << L" was created.\n";
		#endif // _DEBUG
	}

	VerificationCore::~VerificationCore()
	{
		corrtable.clear();
		paramarr.clear();
		themename.clear();
		namesuffix.clear();
		#ifdef _DEBUG
			std::wcout << L"~VerificationCore(). Object with ID = " << ID << L" was destroyed.\n";
		#endif // _DEBUG
	}

	unsigned int VerificationCore::getID() const
	{
		return ID;
	}

	void VerificationCore::setThemeName(const std::wstring & nthname)
	{
		if (nthname.length() == 0)
			ERROR::throwError(L"Error in VerificationCore::setThemeName(). New theme name can't be empty!", ID);
		if (themename.length() != 0)
			themename.clear();
		themename.append(nthname);
		if (corrtable.size() != 0)
			corrtable.clear();
		if (paramarr.size() != 0)
			paramarr.clear();
	}

	std::wstring VerificationCore::getThemeName() const
	{
		return std::wstring(themename);
	}

	void VerificationCore::setnamesuffix(const std::wstring & nsuffix)
	{
		if (nsuffix.length() == 0)
			ERROR::throwError(L"Error in VerificationCore::setnamesuffix(). New name suffix can't be empty.", ID);
		if (namesuffix.length() != 0) {
			namesuffix.clear();
		}
		namesuffix.append(nsuffix);
		if (corrtable.size() != 0)
			corrtable.clear();
		if (paramarr.size() != 0)
			paramarr.clear();
	}

	std::wstring VerificationCore::getnamesuffix() const
	{
		return std::wstring(namesuffix);
	}

	void VerificationCore::start()
	{
		MODEL::Entity ReadEnt;
		MODEL::Relation *ReadRelArr;
		int EntCount = 0, RelCount = 0;
		unsigned int iparamstr = 0;
		TXTANALYZE::TxtAnalyzer Analyzer;
		CorrespLine corrbuff;
		std::wstring relstandname, rdrelname;
		std::vector<std::wstring> ParamString, StandParamString;
		unsigned int i = 0;
		bool nextFlag, stopFlag;

		std::wstring verfresults = L"";
		
		if (themename.length() == 0)
			ERROR::throwError(L"Error in VerificationCore::start(). Empty theme name. Set themename first.", ID);

		iomodule.setFileName(L"txt files/" + namesuffix);

		iomodule.readEntCount(EntCount);
		if (EntCount <= 0)
			ERROR::throwError(L"Error in VerificationCore::start(). Input entity count can't be <= 0. Check input file.", ID);

		for (int i = 0; i < EntCount; i++)
		{
			iomodule.readEntity(ReadEnt);
			//TEST CODE START. 
			//TO DO: переписать код к более приемлемому виду, как только будет написан модуль анализа текста
			corrbuff.setVerifName(ReadEnt.getName());
			std::wstring buf;
			if (!Analyzer.Analyze(iomodule, ReadEnt, buf))
				verfresults.append(L"\nНе найдено соответствие для сущности: " + buf);

			corrbuff.setStandName(buf);
			if (buf.length() > 0)
				corrbuff.setParam(Analyzer.GenParam(buf));
			else
				corrbuff.setParam(L"\0");
			
			corrtable.push_back(corrbuff);
			//TEST CODE END.
		}
		#ifdef _DEBUG
			std::wcout << L"\n\n";
			for (unsigned int i = 0; i < corrtable.size(); i++)
				std::wcout << corrtable.at(i).getVerifName() << ' ' << corrtable.at(i).getStandardName() << ' ' << corrtable.at(i).getParam() << L'\n';
		#endif // _DEBUG
		iomodule.readRelCount(RelCount);

		#ifdef _DEBUG
			std::wcout << L"\nRelCount = " << RelCount << L'\n';
		#endif // _DEBUG
		ReadRelArr = new MODEL::Relation[RelCount];

		for (int i = 0; i < RelCount; i++) {
			wchar_t rdtype;
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
				std::wcout << L"\nType = " << ReadRelArr[i].getType() << L"\nParent = " << ReadRelArr[i].getParentEnt() << L"\nChild = " << ReadRelArr[i].getChildEnt() << L'\n';
			#endif // _DEBUG
		}

		for (unsigned int i = 0; i < corrtable.size(); i++) 
			ParamString.push_back(corrtable.at(i).getParam());
		for (int i = 0; i < RelCount; i++)
			ParamString.push_back(Analyzer.GenParam(ReadRelArr[i]));

		delete[] ReadRelArr;
#ifdef _DEBUG
			std::wcout << L"PARAM STRING:\n";
			for (unsigned int i = 0; i < ParamString.size(); i++){
				std::wcout << i << L' ' << ParamString.at(i) << L'\n';
			}
#endif // _DEBUG
		
		iomodule.setFileName(L"txt files/StandardParams_" + themename + L".txt");
		iomodule.readParamStr(StandParamString);

		if (ParamString.size() < StandParamString.size())
			verfresults.append(L"\nПредупреждение! Возможно проверяемая модель не полностью отображает предметную область.");

		while (i != ParamString.size()) {
			nextFlag = stopFlag = true;
			for (unsigned int j = 0; (j < StandParamString.size()) && stopFlag; j++) {
				if (StandParamString.at(j).compare(ParamString.at(i)) == 0) {
#ifdef _DEBUG
					std::wcout << L"\nИтоговый цикл. Из проверяемой модели успешно удален элемент: \n" << ParamString.at(i) << L'\n';
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
				verfresults.append(L"\nНе удалось соотнести с эталоном: " + findNameFromParam(ParamString.at(i), corrtable));
		else {
			verfresults.clear();
			verfresults.append(L"\nПроверямая модель точно отображает предметную область.");
		}

		if (!iomodule.freset(L"txt files/results/VerifResults_" + namesuffix))
			ERROR::throwError(L"Error in VerificationCore::start(). Can't open file to write down verifications results.", ID);
		iomodule.writeLine(L"txt files/results/VerifResults_" + namesuffix, verfresults);

		// БЛОК СЧИТЫВАНИЯ СВЯЗЕЙ
		// DONE :	1) реализовать в модуле считывания функцию считывания кол-ва связей (либо считывать до тех пор, пока не встретили конец файла)
		//			2) считывая имена сущностей в связях соотносить их с таблицей corrtable
		//			3) заносить параметры, описывающие, считанные связи, в "параметрическую модель"

		// БЛОК СВЕРКИ С ЭТАЛОНОМ
		// DONE :	1) реализовать заполнение "параметрической модели" параметрами сущностей (из corrtable)
		//			2) реализовать запрос "параметрической модели" эталона
		//			3) реализовать сверку проверяемой модели с эталоном, с записью несоответствий
		// TO DO 
		//	   	 
		// КОНЕЦ
		// DONE	:	1) Подать на OUTPUT список несоответствий и ошибок
		//
		// TO DO
	}

	void VerificationCore::genStandParam()
	{
		MODEL::Entity ReadEnt;
		MODEL::Relation *ReadRelArr;
		int EntCount = 0, RelCount = 0;
		TXTANALYZE::TxtAnalyzer Analyzer;
		std::wstring rdrelname;
		std::vector<std::wstring> ParamString;

		if (themename.length() == 0)
			ERROR::throwError(L"Error in VerificationCore::genStandParam(). Empty theme name. Set themename first.", ID);

		iomodule.setFileName(L"txt files/" + namesuffix);

		iomodule.readEntCount(EntCount);
		if (EntCount <= 0)
			ERROR::throwError(L"Error in VerificationCore::genStandParam(). Input entity count can't be <= 0. Check input file.", ID);

		for (int i = 0; i < EntCount; i++)
		{
			iomodule.readEntity(ReadEnt);
			ParamString.push_back(Analyzer.GenParam(ReadEnt.getName()));
		}

		iomodule.readRelCount(RelCount);

		std::wcout << L"\nRelCount = " << RelCount << L'\n';

		ReadRelArr = new MODEL::Relation[RelCount];

		for (int i = 0; i < RelCount; i++) {
			wchar_t rdtype;
			iomodule.readRelType(rdtype);
			ReadRelArr[i].setType(rdtype);

			iomodule.readRelParentEnt(rdrelname);
			ReadRelArr[i].setParentEnt(rdrelname);
			iomodule.readRelChildEnt(rdrelname);
			ReadRelArr[i].setChildEnt(rdrelname);

			std::wcout << L"\nType = " << ReadRelArr[i].getType() << L"\nParent = " << ReadRelArr[i].getParentEnt() << L"\nChild = " << ReadRelArr[i].getChildEnt() << L'\n';
			ParamString.push_back(Analyzer.GenParam(ReadRelArr[i]));
		}
		delete[] ReadRelArr;

		/*
			TO DO:

			DONE:
				1. Запись строки параметров в файл
		*/

		std::wcout << L"PARAM STRING:\n";
		for (unsigned int i = 0; i < ParamString.size(); i++) 
			std::wcout << i << L' ' << ParamString.at(i) << L'\n';
		
		iomodule.writeParamString(L"txt files/StandardParams_" + themename + L".txt", ParamString);
	}
}