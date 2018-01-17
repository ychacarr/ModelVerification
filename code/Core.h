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
		std::string paramFlName;
		INPUT::IO iomodule;
	public:
		VerificationCore();
		//VerificationCore(const std::string & theme, const std::string & inFile);
		~VerificationCore();
		unsigned int getID() const;
		void setParamFlName(const std::string & newparamfl);
		std::string getParamFlName() const;
		void reload(const std::string & inFile);
		std::string getFilename() const;
		void setFilename(const std::string & nfilename);
		void start();
		void genStandParam(const std::string & outFileName);
	};
}
#endif // !H_VCORE
