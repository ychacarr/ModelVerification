#include "stdafx.h"
#include "Errors.h"

namespace ERROR {

	wchar_t ErrorMSG[160];

	void throwError(const wchar_t *EMSG, unsigned int inID)
	{
		if (inID != 0)
			_snwprintf_s(ErrorMSG, 160, L"%s Object ID = %d", EMSG, inID);
		else
			_snwprintf_s(ErrorMSG, 160, L"%s", EMSG);
		throw ErrorMSG;
	}

	void throwError(const std::wstring &EMSG, unsigned int inID)
	{
		if (inID != 0)
			_snwprintf_s(ErrorMSG, 160, L"%s Object ID = %d.", EMSG.c_str(), inID);
		else
			_snwprintf_s(ErrorMSG, 160, L"%s", EMSG.c_str());
		throw ErrorMSG;
	}
}