#pragma once
#ifndef H_VERCORE
#define H_VERCORE
#include "ÑorrespondenceTable.h"
#include "IOModule.h"
#include <vector>

namespace VCORE
{
	class VerificationCore
	{
	private:
		unsigned int ID;
		static unsigned int Counter;
		std::vector<CorrespLine> corrtable;
		std::vector<std::wstring> paramarr;
		//std::string paramFlName;
		INPUT::IO iomodule;
		std::wstring namesuffix;
		std::wstring themename;
	public:
		VerificationCore();
		//VerificationCore(const std::string & theme, const std::string & inFile);
		~VerificationCore();
		unsigned int getID() const;
		void start();
		void genStandParam();
		void setnamesuffix(const std::wstring & nsuffix);
		std::wstring getnamesuffix() const;
		void setThemeName(const std::wstring & nthemename);
		std::wstring getThemeName() const;
	};
}
#endif // !H_VCORE
