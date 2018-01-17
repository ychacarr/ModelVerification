#pragma once
#ifndef H_ERRORFUNC
#define H_ERRORFUNC

namespace ERROR {
	void throwError(const char *EMSG, unsigned int inID = 0);
	void throwError(const std::string &EMSG, unsigned int inID = 0);
}
#endif // !H_ERRORFUNC