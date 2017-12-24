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
		std::vector<std::string> paramarr;
		//std::string paramFlName;
		INPUT::IO iomodule;
		std::string namesuffix;
		std::string themename;
	public:
		VerificationCore();
		//VerificationCore(const std::string & theme, const std::string & inFile);
		~VerificationCore();
		unsigned int getID() const;
		void start();
		void genStandParam();
		void setnamesuffix(const std::string & nsuffix);
		std::string getnamesuffix() const;
		void setThemeName(const std::string & nthemename);
		std::string getThemeName() const;
	};
}
#endif // !H_VCORE
