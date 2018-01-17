#include "stdafx.h"
#include "Errors.h"

namespace ERROR {

	char ErrorMSG[160];

	void throwError(const char *EMSG, unsigned int inID)
	{
		if (inID != 0)
			snprintf(ErrorMSG, 160, "%s Object ID = %d", EMSG, inID);
		else
			snprintf(ErrorMSG, 160, "%s", EMSG);
		throw ErrorMSG;
	}

	void throwError(const std::string &EMSG, unsigned int inID)
	{
		if (inID != 0)
			snprintf(ErrorMSG, 160, "%s Object ID = %d.", EMSG.c_str(), inID);
		else
			snprintf(ErrorMSG, 160, "%s", EMSG.c_str());
		throw ErrorMSG;
	}
}